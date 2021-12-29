#include "Worker.h"
#include "Parameter.h"
#include "Common/Tool/Variant.h"
#include "Common/Utility/StopWatch.h"

#include "WrapperMotionApi/include/ErrorCode.h"
#include "WrapperMotionApi/WrapperMotionApi.h"

#pragma comment(lib, "WrapperMotionApi.lib")

namespace Standard
{
	namespace Plc
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
		/// @detail			�������̃^�C�~���O�ŌĂяo�����
		///					��Api�̃^�C���A�E�g�Ȃǂ�ݒ肷��
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::InitializeDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

			try
			{
				// �f�t�H���g�Ȑݒ�N���X���擾
				Setting::CDefault defaultSetting = GetDefaultSetting();

				// Api�̃^�C���A�E�g���擾
				boost::any buffer = defaultSetting.Other.GetValue<EnumParameter>(ParameterApiTimeout);

				// Api�̃^�C���A�E�g���L�����m�F
				if (Variant::IsValue<std::string>(buffer))
				{
					// Api�^�C���A�E�g�̐ݒ�
					MotionApi::Controller::Setting::CApiTimeout apiTimeout;

					// Api�̃^�C���A�E�g���擾
					apiTimeout.Input(Variant::Convert<std::string>(buffer));

					// Api�̃^�C���A�E�g��ݒ�
					postscript << "MotionApi::SetApiTimeout(" << apiTimeout.Value << ")";
					MotionApi::SetApiTimeout(apiTimeout);
				}

				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
		/// @brief			�j��
		/// @detail			�j���̃^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DestroyDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
		/// @brief			�ڑ�
		/// @detail			�ڑ��̃^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::ConnectDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcConnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

			try
			{
				// �ڑ��ݒ�N���X���擾
				Setting::CConnect connectSetting = GetConnectSetting();

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
		/// @brief			�ؒf
		/// @detail			�ؒf�̃^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DisconnectDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcDisconnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
		/// @brief			�ڑ��̏�Ԃ��擾
		///	@detail			�ڑ��̏�Ԃ��擾����^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @return			true:�ڑ��ς� / false:���ڑ�
		////////////////////////////////////////////////////////////////////////////////
		bool CWorker::GetConnected()
		{
			bool ret = false;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcConnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "GetConnected()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}
	}
}
