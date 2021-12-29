#include "Poling.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			CWorker::CWorker() : Worker::CTemplate()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�f�X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			CWorker::~CWorker()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			������
			/// @detail			���C���X���b�h���珉�����̃^�C�~���O�ŌĂяo�����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::InitializeDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CInitialize trigger;

				logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					Poling::CSetting setting = GetSetting();

					//@@@ �^���I�ȑ҂�����
					std::this_thread::sleep_for(std::chrono::milliseconds(500));

					// ���K�{
					//@@@ �ڑ����鏈���Ȃ񂩂��L�q����

					// ���K�{
					// �������̊������N��
					postscript << "WakeupInitializeComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupInitializeComplete(trigger);
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �������̊������N��
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupInitializeComplete(trigger);

					// ��O��ʒm�����C
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�j��
			/// @detail			���C���X���b�h����j���̃^�C�~���O�ŌĂяo�����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::DestroyDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoDestroy;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CDestroy trigger;

				logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					//@@@ �^���I�ȑ҂�����
					std::this_thread::sleep_for(std::chrono::milliseconds(700));

					// ���K�{
					//@@@ �ؒf���鏈���Ȃ񂩂��L�q����

					// ���K�{
					// �j���̊������N��
					postscript << "WakeupDestroyComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupDestroyComplete(trigger);
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �j���̊������N��
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupDestroyComplete(trigger);

					// ��O��ʒm�����C
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�ǂݍ���
			/// @detail			���C���X���b�h����ǂݍ��݂̃^�C�~���O�ŌĂяo�����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::ReadDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoRead;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "ReadDerived()" << Logging::ConstSeparator;

				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					Poling::CSetting setting = GetSetting();

					//@@@ �^���I�ȑ҂�����
					std::this_thread::sleep_for(std::chrono::milliseconds(100));

					// ���K�{
					//@@@ �ǂݍ��݂��鏈���Ȃ񂩂��L�q����
					Status::CInput input;
					Setting::CAddressRange addressRange;

					// ���K�{
					// �ǂݍ��݂�ʒm
					NotifyArgument(input);

					// �ǂݍ���
					postscript << "Read()" << Logging::ConstSeparator << stopWatch.Format(true);

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O��ʒm�����C
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			��������
			/// @detail			���C���X���b�h���珑�����݂̃^�C�~���O�ŌĂяo�����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::WriteDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "WriteDerived()" << Logging::ConstSeparator;

				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					Poling::CSetting setting = GetSetting();

					//@@@ �^���I�ȑ҂�����
					std::this_thread::sleep_for(std::chrono::milliseconds(200));

					do
					{
						// ���K�{
						//@@@ �������݂��鏈���Ȃ񂩂��L�q����
						auto write = GetWriteFirst();

						// �������݃N���X���m�F
						if (write == nullptr)
						{
							break;
						}

						// ���K�{
						// �擪�ɓo�^���Ă���f�[�^���폜
						PopWriteFirst();
					} while (false);

					// ��������
					postscript << "Write()" << Logging::ConstSeparator << stopWatch.Format(true);

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O��ʒm�����C
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}
		}
	}
}