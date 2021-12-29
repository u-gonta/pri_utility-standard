#include "Worker.h"
#include "Common/Utility/StopWatch.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CWorker::CWorker() : Worker::CTemplate()
		{
			m_connected = false;
			m_analogGain = 1;
			m_digitalGain = 5;
			m_exposureTime = 128;
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
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::InitializeDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

			try
			{
				auto defaultSetting = GetDefaultSetting();

				//@@@ �^���I�ȑ҂�����
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// ���K�{
				//@@@ ���������鏈���Ȃ񂩂��L�q����

				postscript << "�摜�̕�:" << defaultSetting.ImageWidth << Logging::ConstSeparator << "�摜�̍���:" << defaultSetting.ImageHeight;

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
		/// @brief			�j��
		/// @detail			�j���̃^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DestroyDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

			try
			{
				//@@@ �^���I�ȑ҂�����
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// ���K�{
				//@@@ �j�����鏈���Ȃ񂩂��L�q����

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
		/// @brief			�ڑ�
		/// @detail			�ڑ��̃^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::ConnectDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberConnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

			try
			{
				auto connectSetting = GetConnectSetting();

				//@@@ �^���I�ȑ҂�����
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// ���K�{
				//@@@ �ڑ����鏈���Ȃ񂩂��L�q����

				postscript << "�J������IP�A�h���X:" << connectSetting.Identifier;

				// �ڑ��̏�Ԃ�ڑ��ɍX�V
				SetConnected(true);

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
		/// @brief			�ؒf
		/// @detail			�ؒf�̃^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DisconnectDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDisconnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

			try
			{
				//@@@ �^���I�ȑ҂�����
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// ���K�{
				//@@@ �ؒf���鏈���Ȃ񂩂��L�q����

				// �ڑ��̏�Ԃ𖢐ڑ��ɍX�V
				SetConnected(false);

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
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "GetConnected()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �ڑ��̏�Ԃ��擾���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				ret = m_connected;

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

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ڑ��̏�Ԃ��X�V
		/// @param[in]		value	true:�ڑ��ς� / false:���ڑ�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::SetConnected(bool value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "SetConnected()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �ڑ��̏�Ԃ��X�V���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				m_connected = value;

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
		/// @brief			�A�i���O�Q�C����ǂݍ���
		///	@detail			���p�������֐�
		/// @return			�A�i���O�Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadAnalogGainDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadAnalogGainDerived()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �A�i���O�Q�C�����擾���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// �A�i���O�Q�C�����擾
				ret = m_analogGain;

				postscript << ret;

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

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�A�i���O�Q�C������������
		///	@detail			���p�������֐�
		/// @param[in]		value	�A�i���O�Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteAnalogGainDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteAnalogGainDerived()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �A�i���O�Q�C�����X�V���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// �A�i���O�Q�C�����X�V
				m_analogGain = value;

				postscript << m_analogGain;

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
		/// @brief			�f�W�^���Q�C����ǂݍ���
		///	@detail			���p�������֐�
		/// @return			�f�W�^���Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadDigitalGainDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadAnalogGainDerived()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �f�W�^���Q�C�����擾���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// �f�W�^���Q�C�����擾
				ret = m_digitalGain;

				postscript << ret;

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

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�W�^���Q�C������������
		///	@detail			���p�������֐�
		/// @param[in]		value	�f�W�^���Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteDigitalGainDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteDigitalGainDerived()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �f�W�^���Q�C�����X�V���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// �f�W�^���Q�C�����X�V
				m_digitalGain = value;

				postscript << m_digitalGain;

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
		/// @brief			�I�����Ԃ�ǂݍ���
		///	@detail			���p�������֐�
		/// @return			�I������
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadExposureTimeDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadExposureTimeDerived()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �I�����Ԃ��擾���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// �I�����Ԃ��擾
				ret = m_exposureTime;

				postscript << ret;

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

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�I�����Ԃ���������
		///	@detail			���p�������֐�
		/// @param[in]		value	�I������
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteExposureTimeDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteExposureTimeDerived()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �I�����Ԃ��X�V���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// �I�����Ԃ��X�V
				m_exposureTime = value;

				postscript << m_exposureTime;

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
		/// @brief			��荞��
		///	@detail			���p�������֐�
		/// @return			�摜�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		Image::CObject CWorker::CaptureDerived()
		{
			Image::CObject ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberCapture;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "CaptureDerived()" << Logging::ConstSeparator;

			try
			{
				// ���K�{
				//@@@ �摜���擾���鏈�����L�q

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// �摜���擾
				ret = m_image;

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
