#pragma once

#include <mutex>

#include "Common/Exception/Basis.h"
#include "Common/Logging/Basis.h"
#include "Common/Tool/Text.h"
#include "Common/Tool/Logging.h"
#include "Transfer/Transfer.h"

#pragma comment(lib, "Transfer.lib")

#include "Notice/Worker.h"

namespace Standard
{
	namespace Exception
	{
		namespace Worker
		{
			//! ��O����M����֐��̒�`
			typedef void(OnException)(const Exception::CObject&);

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      ��O�̐�����s���e���v���[�g�̃N���X
			////////////////////////////////////////////////////////////////////////////////
			class CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate()
				{
					// ���̂��X�V
					m_noticeException.SetName("Exception::Notice::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{

				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O�̖��̂��X�V
				/// @param[in]		name	��O�̖���
				////////////////////////////////////////////////////////////////////////////////
				void SetNameException(const std::string& name)
				{
					// ���̂��X�V
					m_noticeException.SetName(name);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O��ʒm����֐��̓o�^
				/// @param[in]		object	��O��ʒm����֐�
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionException(std::function<OnException> object)
				{
					// ��O��ʒm����֐��̓o�^
					m_noticeException.AttachFunctionCalled(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O��ʒm����֐��̉���
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionException()
				{
					// ��O��ʒm����֐��̉���
					m_noticeException.DetachFunctionCalled();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O��ʒm
				/// @param[in]		object	��O�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void NotifyException(const Exception::CObject& object)
				{
					// ��O��ʒm
					m_noticeException.RequestNotice(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O�̏o��
				/// @detail			���O�̏o�͂Ɨ�O�̃N���X�𐶐�����
				/// @param[in]		errorCode	��O�̃R�[�h
				/// @param[in]		deviceErrorCode	�@��̃G���[�R�[�h
				/// @param[in]		logging	���O�̃N���X
				/// @param[in]		postscript	�ǉ��̕�����
				/// @param[in]		e	��O�̃N���X
				/// @return			��O�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				Exception::CObject OutputException(Exception::EnumCode errorCode, int deviceErrorCode, const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
				{
					Exception::CObject ret;

					Logging::CObject buffer;

					// ���O�̃N���X�ɕϊ�
					buffer = Exception::Convert::Logging(errorCode, deviceErrorCode, logging, postscript, e);

					// ��O�̃N���X���X�V
					ret.ErrorCode = errorCode;
					ret.DeviceErrorCode = deviceErrorCode;
					ret.Message = buffer.Message.str();

					// ���O�o��
					Transfer::Output(buffer);

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O�̏o��
				/// @detail			���O�̏o�͂Ɨ�O�̃N���X�𐶐�����
				/// @param[in]		errorCode	��O�̃R�[�h
				/// @param[in]		logging	���O�̃N���X
				/// @param[in]		postscript	�ǉ��̕�����
				/// @return			��O�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				Exception::CObject OutputException(Exception::EnumCode errorCode, const Logging::CObject& logging, const std::string& postscript)
				{
					// ��O�̏o��
					return OutputException(errorCode, Exception::CodeSuccess, logging, postscript, std::exception(""));
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O�̏o��
				/// @detail			���O�̏o�͂Ɨ�O�̃N���X�𐶐�����
				/// @param[in]		errorCode	��O�̃R�[�h
				/// @param[in]		deviceErrorCode	�@��̃G���[�R�[�h
				/// @param[in]		postscript	�ǉ��̕�����
				/// @return			��O�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				Exception::CObject OutputException(Exception::EnumCode errorCode, int deviceErrorCode, const std::string& postscript)
				{
					Logging::CObject logging;

					logging.Message << postscript;

					// ��O�̏o��
					return OutputException(errorCode, deviceErrorCode, logging, "", std::exception(""));
				}

			private:
				//! ��O��ʒm����N���X
				Notice::Worker::CTemplate<OnException, Exception::CObject> m_noticeException;
			};
		}
	}
}