#pragma once

#include <deque>

#include "Thread/Worker.h"

namespace Standard
{
	namespace Notice
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      �ʒm����e���v���[�g�̃N���X
			///				�� �X���b�h�̐�����s���e���v���[�g�̃N���X����h��
			/// @param[in]	FCalled	�ʒm���Ăяo���֐��|�C���^(�e���v���[�g�Ŏw��)
			/// @param[in]	CNotice	�ʒm������̃N���X(�e���v���[�g�Ŏw��)
			////////////////////////////////////////////////////////////////////////////////
			template <typename FCalled, typename CNotice>
			class CTemplate
				: virtual public Thread::Worker::CTemplate
			{
			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @enum			�����̏���
				////////////////////////////////////////////////////////////////////////////////
				enum EnumOrder
				{
					OrderWait,			// �ҋ@
					OrderInitialize,	// ������
					OrderDestroy,		// �j��
					OrderNotice			// �ʒm
				};

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Thread::Worker::CTemplate()
				{
					m_order = OrderWait;
					m_requestInitialize = false;
					m_requestDestroy = false;
					_FunctionCalled = nullptr;

					// ���̂��X�V
					SetName("Notice::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					// �ʒm����֐��̉���
					DetachFunctionCalled();
				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm����֐��̓o�^
				/// @param[in]		object	�ʒm����֐�
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionCalled(std::function<void(const CNotice&)> object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFcalled);

					_FunctionCalled = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm����֐��̉���
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionCalled()
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFcalled);

					_FunctionCalled = nullptr;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��������v��
				////////////////////////////////////////////////////////////////////////////////
				void RequestInitialize()
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "RequestInitialize()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestInitialize);

						// �������̗v����ݒ�
						m_requestInitialize = true;

						// ����v�����N��
						WakeupRequestAction();
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j����v��
				////////////////////////////////////////////////////////////////////////////////
				void RequestDestroy()
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "RequestDestroy()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestDestroy);

						// �j���̗v����ݒ�
						m_requestDestroy = true;

						// ����v�����N��
						WakeupRequestAction();
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm��v��
				/// @param[in]		object	�ʒm������̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void RequestNotice(const CNotice object)
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "RequestNotice()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncNotice);

						// �ʒm������̃N���X��ǉ�
						m_notices.push_back(object);
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					// ����v�����N��
					WakeupRequestAction();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���C���֐��̏���
				/// @return			�^�C���A�E�g����[ms]
				////////////////////////////////////////////////////////////////////////////////
				int MainAction() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "MainAction()" << Logging::ConstSeparator;

					int ret = 0;

					// ����̊֐�
					ret = Thread::Worker::CTemplate::MainAction();

					try
					{
						switch (GetOrder())
						{
						case OrderWait:
							// �ҋ@ �� �������̗v�����m�F
							if (IsRequestInitialize())
							{
								// �^�]�����X�V
								SetRunning(true);

								// �������ֈڍs
								SetOrder(OrderInitialize);

								// �^�C���A�E�g��������
								ret = 0;
								break;
							}

							// �^�]��������
							SetRunning(false);
							break;

						case OrderInitialize:
							// ������ �� �ʒm�ֈڍs
							SetOrder(OrderNotice);

							// �^�C���A�E�g��������
							ret = 0;
							break;

						case OrderDestroy:
							// �j�� �� �ҋ@�ֈڍs
							SetOrder(OrderWait);

							// �^�C���A�E�g��������
							ret = 0;
							break;

						case OrderNotice:
							// �ʒm �� �ʒm����f�[�^�̗L�����m�F
							if (IsNotice())
							{
								// �ʒm����f�[�^���� �� �ʒm
								Notice();

								// �^�C���A�E�g��������
								ret = 0;
								break;
							}

							// ��~�̗v�� or �j���̗v�����m�F
							if (GetRequestStop() || IsRequestDestroy())
							{
								// ��~�v�� �� �j���ֈڍs
								SetOrder(OrderDestroy);

								// �^�C���A�E�g��������
								ret = 0;
								break;
							}

							// �ʒm����f�[�^�Ȃ� �� ���C���֐��̎����Ń^�C���A�E�g��ݒ�
							ret = GetCycle();
							break;
						}
					}
					catch (const Exception::CObject& e)
					{
						// ��O�̏��� �� ���O�̃N���X�ɕϊ����ďo��
						Transfer::Output(Exception::Convert::Logging(logging, postscript.str(), e));
						postscript.str("");

						switch (GetOrder())
						{
						case OrderDestroy:
							// �j�� �� �ҋ@�ֈڍs
							SetOrder(OrderWait);
							break;

						default:
							// ���̑� �� �j���ֈڍs
							SetOrder(OrderDestroy);
							break;
						}

						// �^�C���A�E�g��������
						ret = 0;
					}

					return ret;
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�����̏������擾
				/// @return			�����̏���
				////////////////////////////////////////////////////////////////////////////////
				EnumOrder GetOrder()
				{
					EnumOrder ret = OrderWait;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "GetOrder()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncOrder);

						ret = m_order;
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�����̏������X�V
				/// @param[in]		value	�����̏���
				////////////////////////////////////////////////////////////////////////////////
				void SetOrder(EnumOrder value)
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "SetOrder()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncOrder);

						m_order = value;
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������̗v�����m�F
				/// @detail			�m�F��ɏ������̗v���͉�������
				/// @return			true:��������v�� / false:�������͖��v��
				////////////////////////////////////////////////////////////////////////////////
				bool IsRequestInitialize()
				{
					bool ret = false;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "IsRequestInitialize()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestInitialize);

						// �������̗v�����擾
						ret = m_requestInitialize;

						// �������̗v��������
						m_requestInitialize = false;
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���̗v�����m�F
				/// @detail			�m�F��ɔj���̗v���͉�������
				/// @return			true:�j����v�� / false:�j���͖��v��
				////////////////////////////////////////////////////////////////////////////////
				bool IsRequestDestroy()
				{
					bool ret = false;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "IsRequestDestroy()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestDestroy);

						// �j���̗v�����擾
						ret = m_requestDestroy;

						// �j���̗v��������
						m_requestDestroy = false;
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm����f�[�^�̗L�����m�F
				/// @return			true:�ʒm����f�[�^���� / false:�ʒm����f�[�^�Ȃ�
				////////////////////////////////////////////////////////////////////////////////
				bool IsNotice()
				{
					bool ret = false;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "IsNotice()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncNotice);

						// �ʒm������̃N���X�����m�F
						if (0 < m_notices.size())
						{
							// �ʒm�����񂠂�
							ret = true;
						}
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm������̃N���X�Ő擪�̃f�[�^���擾
				/// @return			�ʒm������̃N���X
				////////////////////////////////////////////////////////////////////////////////
				std::shared_ptr<CNotice> GetsNoticeFront()
				{
					std::shared_ptr<CNotice> ret = nullptr;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "GetsNoticeFront()";

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncNotice);

						// �ʒm������̃N���X�����m�F
						if (0 < m_notices.size())
						{
							// �ʒm������̃N���X���� �� �擪�̒ʒm������̃N���X���擾
							ret = std::make_shared<CNotice>(m_notices.front());
						}
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm���Ăяo��
				/// @param[in]		object	�ʒm������̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void Call(CNotice object)
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "Call()";

					try
					{
						do
						{
							// �r������
							std::lock_guard<std::recursive_mutex> lock(m_asyncFcalled);

							// �ʒm����֐����m�F
							if (_FunctionCalled == nullptr)
							{
								// �ʒm����֐��Ȃ�
								break;
							}

							// �ʒm���Ăяo��
							_FunctionCalled(object);
						} while (false);
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ʒm�����s
				////////////////////////////////////////////////////////////////////////////////
				void Notice()
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "Notice()";

					try
					{
						do
						{
							// �ʒm������̃N���X�Ő擪�̃f�[�^���擾
							std::shared_ptr<CNotice> notice = GetsNoticeFront();

							// �ʒm������̃N���X���m�F
							if (notice == nullptr)
							{
								// �ʒm������̃N���X�Ȃ�
								break;
							}

							// �ʒm���Ăяo��
							Call(*notice);

							// �r������
							std::lock_guard<std::recursive_mutex> lock(m_asyncNotice);

							// �ʒm������̃N���X�����m�F
							if (0 < m_notices.size())
							{
								// �ʒm������̃N���X���� �� �擪�̒ʒm������̃N���X���폜
								m_notices.erase(m_notices.begin());
							}
						} while (false);
					}
					catch (const std::exception& e)
					{
						// ��O�̏���
						postscript << Logging::ConstSeparator << e.what();

						// ��O�𔭖C
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

			private:
				//! �r������N���X(�����̏����p)
				std::recursive_mutex m_asyncOrder;

				//! �����̏���
				EnumOrder m_order;

				//! �r������N���X(�������̗v���p)
				std::recursive_mutex m_asyncRequestInitialize;

				//! �������̗v��
				bool m_requestInitialize;

				//! �r������N���X(�j���̗v���p)
				std::recursive_mutex m_asyncRequestDestroy;

				//! �j���̗v��
				bool m_requestDestroy;

				//! �r������N���X(�ʒm����֐��p)
				std::recursive_mutex m_asyncFcalled;

				//! �ʒm����֐�
				std::function<void(const CNotice&)> _FunctionCalled;

				//! �r������N���X(�ʒm������̃N���X�p)
				std::recursive_mutex m_asyncNotice;

				//! �ʒm������̃N���X
				std::deque<CNotice> m_notices;
			};
		}
	}
}