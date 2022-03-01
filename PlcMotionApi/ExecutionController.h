#pragma once

#include "Execution.h"
#include "SettingController.h"
#include "Result.h"

namespace Standard
{
	namespace Plc
	{
		namespace Controller
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���g���[����������
			/// @param[in]		object �R���g���[��������������ݒ�N���X
			/// @return			�R���g���[�����������������ʂ�ێ�����N���X
			////////////////////////////////////////////////////////////////////////////////
			inline Result::CInitialize Initialize(const Setting::CInitialize& object)
			{
				Result::CInitialize ret;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodePlcInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Initialize()" << Logging::ConstSeparator;

				try
				{
					// �R���g���[�����J���ݒ�N���X
					MotionApi::Controller::Setting::COpen setting = object.Open;

					// �R���g���[���Ɛڑ�
					postscript << "MotionApi::OpenController()";
					ret.Controller = MotionApi::OpenController(setting);
					// ���O�o��
					postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(ret.Controller.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// Api�^�C���A�E�g�̐ݒ�N���X
					MotionApi::Controller::Setting::CApiTimeout apiTimeout = object.ApiTimeout;

					// Api�̃^�C���A�E�g��ݒ�
					postscript << "MotionApi::SetApiTimeout()" << Logging::ConstSeparator << "�^�C���A�E�g:" << apiTimeout.Value;
					MotionApi::SetApiTimeout(apiTimeout);
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const MotionApi::CException& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���g���[����j��
			/// @param[in]		object �R���g���[����j������ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			inline void Destroy(const Setting::CDestroy& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodePlcDestroy;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Destroy()" << Logging::ConstSeparator;

				try
				{
					do
					{
						// �R���g���[�������ݒ�N���X
						MotionApi::Controller::Setting::CClose setting = object.Close;

						// �n���h�����m�F
						if (setting.Handle == MotionApi::ConstDisconnected)
						{
							// ���O�o��
							postscript << Logging::ConstSeparator << "��������";
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
							break;
						}

						// �R���g���[���Ɛؒf
						postscript << "MotionApi::CloseController()";
						MotionApi::CloseController(setting);
						// ���O�o��
						postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(setting.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					} while (false);
				}
				catch (const MotionApi::CException& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}
		}
	}
}