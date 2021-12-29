#pragma once

#include <string>
#include <deque>
#include <functional>

#include "boost/function.hpp"

#include "Common/Exception/Basis.h"
#include "Common/Logging/Basis.h"
#include "Common/Tool/Text.h"
#include "Common/Tool/Exception.h"
#include "Common/Utility/SequentialAsync.h"

namespace Standard
{
	namespace Communication
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			Shift-JIS �� UTF8�֕ϊ� ���C�����C���֐�
		/// @param[in]		value	���f�[�^
		/// @return         UTF8�ɕϊ����ꂽ������
		////////////////////////////////////////////////////////////////////////////////
		inline std::string ShiftJisToUtf8(const std::string& value)
		{
			std::string ret = "";

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

			// Shift-JIS �� UTF8�֕ϊ�
			ret = converter.to_bytes(Text::Convert(value));

			return ret;
		}

		namespace Udp
		{
			using namespace Standard;

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      UDP�ʐM���s���N���X
			/// @detail		���M�f�[�^�̓o�^�A�񓯊��ő��M���s��
			////////////////////////////////////////////////////////////////////////////////
			class CWorker
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CWorker() :
					  m_sequentialAsync(m_io_service, 1)
					, m_strand(m_io_service)
					, m_socket(m_io_service)
				{
					_FunctionException = nullptr;
					m_buffers.clear();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CWorker()
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << "Communication::Udp::~CWorker()" << Logging::ConstSeparator;

					try
					{
						// �j��
						postscript << "Destroy()";
						Destroy();
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� �f�o�b�O�o��
						Exception::OutputDebug(logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j��
				////////////////////////////////////////////////////////////////////////////////
				void Destroy()
				{
					// �����񓯊��������~
					m_sequentialAsync.Stop();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O��ʒm����֐��̓o�^
				/// @detail			��O�̔������ɌĂяo���ꂽ���֐����w�肷��
				/// @param[in]		object	�Ăяo���ꂽ���֐�
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionException(std::function<void(const Exception::CObject&)> object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncException);

					// ��O�̒ʒm��o�^
					_FunctionException = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O��ʒm����֐��̉���
				/// @detail			��O�̔������ɌĂяo�����֐�����������
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionException()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncException);

					// ��O�̒ʒm������
					_FunctionException = nullptr;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���M��֐ڑ�
				/// @detail			�ڑ�����֐���񓯊��ŌĂяo��
				/// @param[in]		address	IP�A�h���X
				/// @param[in]		port	�|�[�g(�ڑ���)
				////////////////////////////////////////////////////////////////////////////////
				void AsyncConnect(const std::string& address, unsigned short port)
				{
					// �ڑ�
					m_strand.post(boost::bind(&CWorker::Connect, this, address, port));
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���M�f�[�^��ǉ�
				/// @detail			�ǉ�����֐���񓯊��ŌĂяo��
				/// @param[in]		value	���b�Z�[�W
				////////////////////////////////////////////////////////////////////////////////
				void AsyncMessagePush(const std::string& value)
				{
					// ���M�f�[�^��ǉ�
					m_strand.post(boost::bind(&CWorker::MessagePush, this, value));
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O�̒ʒm��]��
				/// @detail			�ʒm�̓]���͔񓯊��̊֐����g�p���鎖
				/// @param[in]		object	��O�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void TransferException(const Exception::CObject& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << "Communication::CUdp::TransferException()" << Logging::ConstSeparator;

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncException);

						postscript << "_FunctionException()";
						if (_FunctionException != nullptr)
						{
							// ��O��ʒm����֐����L�� �� ��O��ʒm
							_FunctionException(object);
						}
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� �f�o�b�O�o��
						Exception::OutputDebug(logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O�̒ʒm��]��
				/// @param[in]		errorCode	�G���[�R�[�h
				/// @param[in]		logging	���O�̃N���X
				/// @param[in]		postscript	�ǉ��̕�����
				/// @param[in]		e	��O
				////////////////////////////////////////////////////////////////////////////////
				void TransferException(Exception::EnumCode errorCode, const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
				{
					Exception::CObject object;

					std::stringstream message;

					object.ErrorCode = errorCode;
					message << logging.Message.str() << postscript << Logging::ConstSeparator << e.what();
					object.Message = message.str();

					// ��O�̒ʒm��]��
					TransferException(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���M��֐ڑ�
				/// @detail			���M��ւ̐ڑ��͔񓯊��̊֐����g�p���鎖
				/// @param[in]		address		IP�A�h���X
				/// @param[in]		port	�|�[�g(�ڑ���)
				////////////////////////////////////////////////////////////////////////////////
				void Connect(const std::string& address, unsigned short port)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << "Communication::CUdp::AsyncConnect()" << Logging::ConstSeparator;

					try
					{
						// �ڑ����m�F
						postscript << "m_socket.is_open()";
						if (m_socket.is_open())
						{
							postscript.str("");
							postscript << "m_socket.close()";
							m_socket.close();
						}
						postscript.str("");

						boost::system::error_code error;

						// �ڑ�
						postscript << "m_socket.connet()";
						m_socket.connect(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(address), port), error);

						// �G���[���m�F
						if (error)
						{
							std::stringstream message;
							message << "Error:" << error;

							// ��O�𔭖C
							throw std::runtime_error(message.str());
						}
						postscript.str("");
					}
					catch (const std::exception & e)
					{
						// ��O�̏��� �� ��O��]��
						TransferException(Exception::CodeUdpConnect, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���M�f�[�^��ǉ�
				/// @detail			���M�f�[�^�̒ǉ��͔񓯊��̊֐����g�p���鎖
				/// @param[in]		value	���b�Z�[�W
				////////////////////////////////////////////////////////////////////////////////
				void MessagePush(const std::string& value)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << "Communication::CUdp::AsyncPush()" << Logging::ConstSeparator;

					try
					{
						// ���M�f�[�^�𕪊����鐔���v�Z
						postscript << "size()";
						size_t size = m_buffers.size();
						size_t block = 10000;
						size_t split = value.size() / block;
						size_t remainder = value.size() % block;
						size_t last = 0;
						postscript.str("");

						// ���M�f�[�^�𕪊����đ���
						postscript << "m_buffers.push_back()";
						for (size_t index = 0; index < split; index++)
						{
							// ���M�f�[�^�𕪊����ăo�b�t�@�֒ǉ�
							m_buffers.push_back(value.substr(last, block));
							last += block;
						}

						// ���M�f�[�^�̗]����o�b�t�@�Ɋi�[
						if (remainder)
						{
							// �o�b�t�@�[�֒ǉ�
							m_buffers.push_back(value.substr(last, remainder));
						}
						postscript.str("");

						do
						{
							// �����M�̃o�b�t�@�[�����邩�m�F
							if (0 < size)
							{
								break;
							}

							// ���M�̎葱��
							postscript << "AsyncWriteProdure()";
							AsyncWriteProcedure();
							postscript.str("");
						} while (false);
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O��]��
						TransferException(Exception::CodeUdpWrite, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���M�̎葱��
				/// @detail			���M�̎葱����񓯊��ŌĂяo��
				////////////////////////////////////////////////////////////////////////////////
				void AsyncWriteProcedure()
				{
					// �擪�̃o�b�t�@���擾
					const std::string& value = ShiftJisToUtf8(m_buffers.front());

					// �񓯊��ő��M
					m_socket.async_send(boost::asio::buffer(value.c_str(), value.size()), m_strand.wrap(boost::bind(&CWorker::OnWriteCompleted, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���M��̃C�x���g
				/// @detail			���M�����������^�C�~���O�Ŕ�������
				/// @param[in]		error	�G���[
				/// @param[in]		size	���M�T�C�Y
				////////////////////////////////////////////////////////////////////////////////
				void OnWriteCompleted(const boost::system::error_code& error, const size_t size)
				{
					Logging::CObject logging;

					logging.Message << "Communication::CUdp::OnWriteCompleted()" << Logging::ConstSeparator;

					// �o�b�t�@�[�̐擪���폜
					m_buffers.pop_front();

					do
					{
						// �G���[���m�F
						if (error)
						{
							logging.Level = Logging::LevelError;
							logging.Message << "Error:" << error;

							// ��O��]��
							TransferException(Exception::CObject(Exception::CodeUdpWrite, logging.Message.str()));
							break;
						}

						// �����M�̃o�b�t�@�[�����邩�m�F
						if (m_buffers.empty())
						{
							break;
						}

						// ���M�̎葱��
						AsyncWriteProcedure();
					} while (false);
				}

			private:
				//! �r������N���X(��O��ʒm����֐��p)
				std::recursive_mutex m_asyncException;

				//! ��O��ʒm����֐�
				std::function<void(const Exception::CObject&)> _FunctionException;

				//! IO�T�[�r�X
				boost::asio::io_service m_io_service;

				//! �����񓯊������̃N���X
				Utility::CSequentialAsync m_sequentialAsync;

				//! �����r������̃N���X
				boost::asio::io_service::strand m_strand;

				//! �\�P�b�g�̃N���X
				boost::asio::ip::udp::socket m_socket;

				//! ���M���IP�A�h���X
				std::string m_address;

				//! ���M������̃o�b�t�@
				std::deque<std::string> m_buffers;
			};
		}
	}
}