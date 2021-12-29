#include "Worker.h"

namespace Standard
{
	namespace Axis
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CWorker::CWorker() : CTemplate()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CWorker::~CWorker()
		{

		}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			���̐���A���̊Ď��A��Ԃ�ʒm�����b�N���X�̗�O���󂯎��֐�
		///// @param[in]		object	��O�̃N���X
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::OnPolingException(const Exception::CObject& object)
		//{

		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			�f�o�C�X�����擾 ���h����ŋL�q
		///// @return			�f�o�C�X��
		//////////////////////////////////////////////////////////////////////////////////
		//std::string CWorker::GetName()
		//{
		//	std::string ret = "Axis::";

		//	return ret;
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			�ڑ��̏�Ԃ��擾
		///// @return			true:�ڑ��ς� / false:���ڑ�
		//////////////////////////////////////////////////////////////////////////////////
		//bool CWorker::GetConnected()
		//{
		//	bool ret = false;

		//	// �ڑ��̏�Ԃ��擾
		//	ret = m_poling.GetConnected();

		//	return ret;
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			������
		///// @detail			�������̃^�C�~���O�ŌĂяo����� �����̐���A���̊Ď��A��Ԃ�ʒm����N���X�̗�O��ʒm����֐���o�^�A�����̍X�V�ȂǍs��
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::InitializeDerived()
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

		//	try
		//	{
		//		// �f�t�H���g�̐ݒ�N���X���擾
		//		Setting::CDefault defaultSetting = GetDefaultSetting();

		//		// ��O��ʒm����֐��̓o�^
		//		postscript << "Poling::AttachFunctionException()";
		//		m_poling.AttachFunctionException(std::bind(&CWorker::OnPolingException, this, std::placeholders::_1));
		//		postscript.str("");

		//		// �|�[�����O�̎������X�V
		//		postscript << "Poling::SetCycle(" << defaultSetting.Cycle << ")";
		//		m_poling.SetCycle(defaultSetting.Cycle);
		//		// ���O�o��
		//		postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//		Transfer::Output(Logging::Join(logging, postscript.str()));
		//		postscript.str("");

		//		// �����|�[�����O����ׂ̐ݒ���擾
		//		postscript << "Poling::GetSetting()";
		//		Setting::CPoling polingSetting = m_poling.GetSetting();
		//		postscript.str("");

		//		// ���g���C�񐔂��X�V
		//		postscript << "Poling::Setting::Retry(" << defaultSetting.Retry << ")";
		//		polingSetting.Retry = defaultSetting.Retry;
		//		// ���O�o��
		//		postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//		Transfer::Output(Logging::Join(logging, postscript.str()));
		//		postscript.str("");

		//		// �����|�[�����O����ׂ̐ݒ���X�V
		//		postscript << "Poling::SetSetting()";
		//		m_poling.SetSetting(polingSetting);
		//		postscript.str("");

		//		// ���̃|�[�����O���J�n
		//		postscript << "Poling::Start()";
		//		m_poling.Start();
		//		postscript.str("");
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// ��O�̏��� �� ��O��ʒm�����C
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			�j��
		///// @detail			�j���̃^�C�~���O�ŌĂяo����� �����̐���A���̊Ď��A��Ԃ�ʒm����N���X�̗�O��ʒm����֐��̉����ȂǍs��
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::DestroyDerived()
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

		//	// �j��
		//	try
		//	{
		//		// ��O��ʒm����֐��̉���
		//		postscript << "Poling::DetachFunctionException()";
		//		m_poling.DetachFunctionException();
		//		// ���O�o��
		//		postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//		Transfer::Output(Logging::Join(logging, postscript.str()));
		//		postscript.str("");
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// ��O�̏��� �� ��O��ʒm�����C
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			�ڑ�
		///// @detail			�ڑ��̃^�C�~���O�ŌĂяo����� �����̐���A���̊Ď��A��Ԃ�ʒm����N���X�Őڑ�����������̂�ҋ@�ȂǍs��
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::ConnectDerived()
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisConnect;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

		//	try
		//	{
		//		do
		//		{
		//			// ���̐���A���̊Ď��A��Ԃ�ʒm����ݒ���擾
		//			postscript << "Poling::GetSetting()";
		//			Setting::CPoling polingSetting = m_poling.GetSetting();
		//			postscript.str("");

		//			// ���֐ڑ����s���ݒ�N���X���X�V
		//			postscript << "Poling::Setting::Connect()";
		//			polingSetting.Connect = GetConnectSetting();
		//			postscript.str("");

		//			// ���̐���A���̊Ď��A��Ԃ�ʒm����ݒ���X�V
		//			postscript << "Poling::SetSetting()";
		//			m_poling.SetSetting(polingSetting);
		//			postscript.str("");

		//			// ���̏������𐧌䂷����N���X
		//			std::shared_ptr<Write::CInitialize> initializeWrite = std::make_shared<Write::CInitialize>();

		//			// �ڑ���ҋ@����N���X
		//			Signal::CObserver<Plc::Result::CConnect> observer;

		//			// �ڑ���ʒm������֐���o�^
		//			postscript << "Subject::RegistrationFunctionConnect()";
		//			std::shared_ptr<std::function<Plc::OnConnect>> onCalled = std::make_shared<std::function<Plc::OnConnect>>(std::bind(&Signal::CObserver<Plc::Result::CConnect>::OnCalled, &observer, std::placeholders::_1));
		//			m_poling.RegistrationFunctionConnect(onCalled);
		//			postscript.str("");

		//			// ��������v��
		//			postscript << "Poling::RequestInitialize()";
		//			m_poling.RequestInitialize(initializeWrite);
		//			postscript.str("");

		//			// �ڑ���҂�
		//			postscript << "Observer::Wait()";
		//			observer.Wait();
		//			postscript.str("");

		//			// �ڑ��̌��ʂ��擾
		//			postscript << "Observer::GetArgument()";
		//			std::shared_ptr<Plc::Result::CConnect> result = observer.GetArgument();
		//			postscript.str("");

		//			// ���f���m�F
		//			if (result->Suspension)
		//			{
		//				postscript << "Observer::Initialize()" << Logging::ConstSeparator << "���f";
		//				// ���O�o��
		//				postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//				Transfer::Output(Logging::Join(logging, postscript.str()));
		//				postscript.str("");
		//				break;
		//			}
		//		} while (false);
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// ��O�̏��� �� ��O��ʒm�����C
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			�ؒf
		///// @detail			�ؒf�̃^�C�~���O�ŌĂяo����� �����̐���A���̊Ď��A��Ԃ�ʒm����N���X�Őؒf����������̂�ҋ@�ȂǍs��
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::DisconnectDerived()
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisDisconnect;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

		//	try
		//	{
		//		do
		//		{
		//			// �ؒf��ҋ@����N���X
		//			Signal::CObserver<Plc::Result::CDisconnect> observer;

		//			// �ؒf��ʒm������֐���o�^
		//			postscript << "Subject::RegistrationFunctionDisconnect()";
		//			std::shared_ptr<std::function<Plc::OnDisconnect>> onCalled = std::make_shared<std::function<Plc::OnDisconnect>>(std::bind(&Signal::CObserver<Plc::Result::CDisconnect>::OnCalled, &observer, std::placeholders::_1));
		//			m_poling.RegistrationFunctionDisconnect(onCalled);
		//			postscript.str("");

		//			// ���̔j���𐧌䂷����N���X
		//			std::shared_ptr<Write::CDestroy> destroyWrite = std::make_shared<Write::CDestroy>();

		//			// �j����v��
		//			postscript << "Poling::RequestDestroy()";
		//			m_poling.RequestDestroy(destroyWrite);
		//			postscript.str("");

		//			// �j����҂�
		//			postscript << "Observer::Wait()";
		//			observer.Wait();
		//			postscript.str("");

		//			// �ڑ��̌��ʂ��擾
		//			postscript << "Observer::GetArgument()";
		//			std::shared_ptr<Plc::Result::CDisconnect> result = observer.GetArgument();
		//			postscript.str("");

		//			// ���f���m�F
		//			if (result->Suspension)
		//			{
		//				postscript << "Observer::Destroy()" << Logging::ConstSeparator << "���f";
		//				// ���O�o��
		//				postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//				Transfer::Output(Logging::Join(logging, postscript.str()));
		//				postscript.str("");
		//				break;
		//			}
		//		} while (false);
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// ��O�̏��� �� ��O��ʒm�����C
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			�T�[�{�𐧌�
		///// @param[in]		object	�T�[�{�𐧌䂷��ݒ�N���X
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::Servo(const Setting::CServo& setting)
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisWrite;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "Servo()" << Logging::ConstSeparator;

		//	try
		//	{
		//		do
		//		{
		//			// �T�[�{�̐����ҋ@����N���X
		//			Signal::CObserver<Result::CSuper> observer;

		//			// �T�[�{�𐧌䂷����N���X
		//			std::shared_ptr<Write::CServo> servoWrite = std::make_shared<Write::CServo>();

		//			// �T�[�{�̏�Ԃ��X�V
		//			servoWrite->Mode = setting.Mode;

		//			// ���̋@��𑖍�
		//			for (const auto& driver : setting.Drivers)
		//			{
		//				// ���̔ԍ����X�V
		//				servoWrite->Indexs.emplace_back(driver.first);
		//			}

		//			// �T�[�{�̐����ʒm������֐���o�^
		//			postscript << "Write::_Funtion";
		//			//std::shared_ptr<std::function<Axis::OnServo>> onCalled = std::make_shared<std::function<Axis::OnServo>>(std::bind(&Signal::CObserver<Axis::Result::CServo>::OnCalled, &observer, std::placeholders::_1));
		//			//servoWrite._Function_ = onCalled;
		//			servoWrite->Subject.RegistrationFunction(std::make_shared<std::function<OnReceive>>(std::bind(&Signal::CObserver<Result::CSuper>::OnCalled, &observer, std::placeholders::_1)));
		//			postscript.str("");

		//			// �������݂�v��
		//			postscript << "Poling::RequestWrite()";
		//			m_poling.RequestWrite(servoWrite);
		//			postscript.str("");

		//			// �����҂�
		//			postscript << "Observer::Wait()";
		//			observer.Wait();
		//			postscript.str("");

		//			// ����̌��ʂ��擾
		//			postscript << "Observer::GetArgument()";
		//			auto result = std::dynamic_pointer_cast<Result::CSuper>(observer.GetArgument());
		//			postscript.str("");

		//			// ���f���m�F
		//			if (result->Suspension)
		//			{
		//				postscript << "Observer::Write()" << Logging::ConstSeparator << "���f";
		//				// ���O�o��
		//				postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//				Transfer::Output(Logging::Join(logging, postscript.str()));
		//				postscript.str("");
		//				break;
		//			}
		//		} while (false);
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// ��O�̏��� �� ��O��ʒm�����C
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}
	}
}