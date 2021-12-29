#include "Poling.h"

namespace Standard
{
	namespace Axis
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
			/// @brief			������ʒm
			/// @param[in]		object	�ʒm������̃N���X(�e���v���[�g�Ŏw��)
			/// @param[in]		remove	true:�ʒm��ɍ폜 / false:�ʒm��ɍ폜���Ȃ�
			////////////////////////////////////////////////////////////////////////////////
			template <typename CWrite, typename CArgument>
			void CWorker::NotifyComplete(std::shared_ptr<CArgument> object, bool remove/*=true*/)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeUnknown;//@@@
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "NotifyComplete(" << typeid(CWrite).name() << ")" << Logging::ConstSeparator;

				try
				{
					//// �r������
					//std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//// �ʒm�����Ď����Ă��鎲�̏��N���X
					//std::vector<std::vector<std::shared_ptr<Write::CSuper>>::iterator> notifieds;

					//// �Ď����Ă��鎲�𑖍�
					//for (std::vector<std::shared_ptr<Write::CSuper>>::iterator surveillance = m_surveillances.begin(); surveillance != m_surveillances.end(); surveillance++)
					//{
					//	// ���䂷����N���X���m�F
					//	if (typeid(CWrite) != typeid(surveillance))
					//	{
					//		// ���䂷����N���X���s��v
					//		continue;
					//	}

					//	// �ʒm���Ăяo��
					//	if ((*surveillance)->Subject.CallingFunction(object))
					//	{
					//		// �Ăяo���𐬌�
					//		notifieds.emplace_back(surveillance);
					//	}
					//}

					//// �폜�t���O���m�F
					//if (remove)
					//{
					//	for (const auto& notified : notifieds)
					//	{
					//		// �Ď����Ă��鎲���폜
					//		m_surveillances.erase(notified);
					//	}
					//}

					//// ���O�o��
					//postscript << "Count:" << notifieds.size() << Logging::ConstSeparator << stopWatch.Format(true);
					//Transfer::Output(Logging::Join(logging, postscript.str()));
					//postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O��ʒm�����C
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
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
				Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CInitialize trigger;

				logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

				try
				{
					//@@@ �^���I�ȑ҂�����
					std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
				Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CDestroy trigger;

				logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

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
			/// @detail			���̏�Ԃ��m�F����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::ReadDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisRead;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "ReadDerived()" << Logging::ConstSeparator;

				try
				{
					size_t index = 0;

					//// �r������
					//std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//// �Ď����Ă��鎲�̏��𑖍�
					//while (index < m_surveillances.size())
					//{
					//	// �����t���O
					//	bool complete = false;

					//	// ���𐧌䂷����̃X�[�p�[�N���X
					//	std::shared_ptr<Write::CSuper> super = m_surveillances.at(index);

					//	// ���̃T�[�{�𐧌䂷���b�N���X�ɕϊ�
					//	auto basis = std::dynamic_pointer_cast<Write::CBasis>(super);

					//	try
					//	{
					//		// �����䂷��ݒ�N���X���m�F
					//		if (typeid(Write::CServo) == typeid(*basis))
					//		{
					//			// �T�[�{�𐧌䂷��ݒ�
					//			MotionApi::Setting::CServoControl servoSetting;

					//			// �f�o�C�X�̃n���h����ݒ�
					//			servoSetting.DeviceHandle = basis->DeviceHandle;

					//			// �T�[�{�𐧌�
					//			postscript << "Plc::Servo()" << Logging::ConstSeparator << "�f�o�C�X�̃n���h��:" << basis->DeviceHandle;
					//			Plc::Servo(GetName(), servoSetting);
					//			postscript.str("");

					//			// �����t���O��ݒ�
					//			complete = true;
					//		}
					//	}
					//	catch (const Exception::CObject& e)
					//	{
					//		// ��O�̏��� �� �f�o�C�X�̃n���h����j��
					//		postscript << "GetDeviceHandle()";
					//		Plc::DestroyDevice(GetName(), basis->DeviceHandle);
					//		postscript.str("");

					//		deviceErrorCode = e.DeviceErrorCode;

					//		// ��O�𔭖C
					//		throw std::exception(e.what());
					//	}

					//	// �����t���O���m�F
					//	if (complete)
					//	{
					//		// �Ď����Ă��鎲�̏����폜 ���擪
					//		m_surveillances.erase(m_surveillances.begin());
					//		continue;
					//	}

					//	// ���̃f�o�C�X��
					//	index += 1;
					//}
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O��ʒm�����C
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			��������
			/// @detail			���̐�����J�n����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::WriteDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "WriteDerived()" << Logging::ConstSeparator;

				try
				{
					//bool run = true;

					//// �������݉\�Ȏ���������
					//while (run)
					//{
					//	// �������݉\�Ȏ��̃X�[�p�[�N���X���擾
					//	auto super = GetWrite(true);

					//	// �������݉\�Ȏ��̃X�[�p�[�N���X���m�F
					//	if (super == nullptr)
					//	{
					//		// �Y���Ȃ�
					//		run = false;
					//		continue;
					//	}

					//	//// ���̃T�[�{�𐧌䂷���b�N���X�ɕϊ�
					//	//auto basis = std::dynamic_pointer_cast<Write::CBasis>(super);

					//	//// �f�o�C�X�̃n���h�����擾
					//	//postscript << "GetDeviceHandle()";
					//	//basis->DeviceHandle = GetDeviceHandle(basis->Indexs);
					//	//postscript.str("");

					//	//// �r������
					//	//std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//	//// �Ď����Ă��鎲�̏��N���X�ɒǉ�
					//	//m_surveillances.push_back(basis);
					//}
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