#include "pch.h"

#include <mutex>

#include "Transfer.h"
#include "Common/Version/Manager.h"
#include "Common/Communication/Udp.h"

namespace Standard
{
	namespace Transfer
	{
		//! �I�[�R�[�h ��ETX(�e�L�X�g�I��)
		const char ConstTerminum = 0x03;

		//! �r������N���X(�t���O�p)
		std::recursive_mutex _asyncFlag;

		//! �����t���O
		static bool _initialize = false;

		//! �j���t���O
		static bool _destroy = false;

		//! �]�����s���ݒ�N���X
		static Setting::CObject _setting;

		//! Udp�ʐM�̃N���X
		static Communication::Udp::CWorker _udp;

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�o�[�W�����̃N���X���擾
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API Version::CObject GetVersion()
		{
			Version::CObject ret;

			Version::CManager& versionManager = Version::CManager::Instance();

			// �o�[�W�����̃N���X���擾
			ret = versionManager.GetVersion();

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��O�̒ʒm����֐���o�^
		/// @detail			��O�̔������ɌĂяo���ꂽ���֐����w�肷��
		/// @param[in]		object	�Ăяo���ꂽ���֐�
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object)
		{
			// ��O�̒ʒm����֐���o�^
			_udp.AttachFunctionException(object);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��O�̒ʒm����֐�������
		/// @detail			��O�̔������ɌĂяo�����֐�����������
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void DetachFunctionException()
		{
			// ��O�̒ʒm����֐�������
			_udp.DetachFunctionException();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			������
		/// @detail			�]����ւ̐ڑ��܂ōs��
		/// @param[in]		object	�]�����s���ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void Initialize(const Setting::CObject& object)
		{
			// �r������
			std::lock_guard<std::recursive_mutex> lock(_asyncFlag);

			// �j���t���O���m�F
			if (_destroy)
			{
				// ��O�𔭖C
				throw Exception::CObject(Exception::CodeTransferDiscarded);
			}

			// �����t���O���m�F
			if (_initialize)
			{
				// ��O�𔭖C
				throw Exception::CObject(Exception::CodeTransferInitialized);
			}

			// �]���̐ݒ�N���X���R�s�[
			_setting = object;

			// ���M��֐ڑ�
			_udp.AsyncConnect(_setting.IpAddress, _setting.Port);

			// �����t���O��ݒ�
			_initialize = true;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�j��
		/// @detail			�]����Ɛؒf�܂ōs��
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void Destroy()
		{
			// �r������
			std::lock_guard<std::recursive_mutex> lock(_asyncFlag);

			do
			{
				// �����t���O���m�F
				if (_initialize == false)
				{
					break;
				}

				// �j���t���O���m�F
				if (_destroy)
				{
					break;
				}

				// �j��
				_udp.Destroy();

				// �����t���O������
				_initialize = false;

				// �j���t���O��ݒ�
				_destroy = true;
			} while (false);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���O��]��
		/// @detail			�o�͂��鉺�����x���ȏ�̂ݓ]������
		///					�擪�ɔN���� �����b��t������
		/// @param[in]		object	���O�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void Output(const Logging::CObject& object)
		{
			// �r������
			std::lock_guard<std::recursive_mutex> lock(_asyncFlag);

			// �j���t���O���m�F
			if (_destroy)
			{
				// ��O�𔭖C
				throw Exception::CObject(Exception::CodeTransferDiscarded);
			}

			do
			{
				// �o�͂��郌�x�����m�F
				if (object.Level < _setting.Lower)
				{
					// �����l���Ⴂ �� �]�������Ȃ�
					break;
				}

				std::stringstream message;
				SYSTEMTIME time;

				// ���ݎ������擾
				::GetLocalTime(&time);

				// �N
				message << Text::Format(4, time.wYear);
				// ��
				message << "-" << Text::Format(2, time.wMonth);
				// ��
				message << "-" << Text::Format(2, time.wDay);
				// ��
				message << "/" << Text::Format(2, time.wHour);
				// ��
				message << ":" << Text::Format(2, time.wMinute);
				// �b
				message << ":" << Text::Format(2, time.wSecond);
				// �~���b
				message << "." << Text::Format(3, time.wMilliseconds);

				// ���x��
				message << Logging::ConstSeparator << object.Level;

				// ���b�Z�[�W
				message << Logging::ConstSeparator << object.Message.str();

				// �I�[�R�[�h ��ETX(�e�L�X�g�I��)
				message << (char)0x03;

				// �o��
				_udp.AsyncMessagePush(message.str());
				// �f�o�b�O�o��
				message << "\n";
				::OutputDebugString(Text::Convert(message.str()).c_str());
			} while (false);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���O��]��
		/// @param[in]		object	���O�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void Output(Logging::EnumLevel level, const std::string& message)
		{
			// ���O��]��
			Output(Logging::CObject(level, message));
		}
	}
}