#include "pch.h"

#include "Grabber/Grabber.h"
#include "Common/Version/Manager.h"
#include "Worker.h"

namespace Standard
{
	namespace Grabber
	{
		//! �J�����̐�����s����b�N���X
		static CWorker _device;

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�o�[�W�����̃N���X���擾
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API Version::CObject GetVersion()
		{
			Version::CObject ret;

			Version::CManager& versionManager = Version::CManager::Instance();

			// �o�[�W�����̃N���X���擾
			ret = versionManager.GetVersion();

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��O��ʒm����֐��̓o�^
		/// @detail			��O�̔������ɌĂяo���ꂽ���֐����w�肷��
		/// @param[in]		object	�Ăяo���ꂽ���֐�
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object)
		{
			// ��O��ʒm����֐��̓o�^
			_device.AttachFunctionException(object);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��O��ʒm����֐��̉���
		/// @detail			��O�̔������ɌĂяo�����֐�����������
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void DetachFunctionException()
		{
			// ��O��ʒm����֐��̉���
			_device.DetachFunctionException();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			������
		/// @param[in]		object	�f�t�H���g�Ȑݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void Initialize(const Setting::CDefault& object)
		{
			// ������
			_device.Initialize(object);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�j��
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void Destroy()
		{
			// �j��
			_device.Destroy();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ڑ�
		/// @param[in]		object	�ڑ��̐ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void Connect(const Setting::CConnect& object)
		{
			// �ڑ�
			_device.Connect(object);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ؒf
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void Disconnect()
		{
			// �ؒf
			_device.Disconnect();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��荞�݂̏����N���X���擾
		/// @return			��荞�݂̏����N���X
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API Condition::CCapture GetCaptureCondition()
		{
			Condition::CCapture ret;

			// ��荞�݂̏����N���X���擾
			ret = _device.GetCaptureCondition();

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�A�i���O�Q�C����ǂݍ���
		/// @return			�A�i���O�Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API double ReadAnalogGain()
		{
			double ret = 0;

			// �A�i���O�Q�C����ǂݍ���
			ret = _device.ReadAnalogGain();

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�A�i���O�Q�C������������
		/// @param[in]		value	�A�i���O�Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void WriteAnalogGain(double value)
		{
			// �A�i���O�Q�C������������
			_device.WriteAnalogGain(value);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�W�^���Q�C����ǂݍ���
		/// @return			�f�W�^���Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API double ReadDigitalGain()
		{
			double ret = 0;

			// �f�W�^���Q�C����ǂݍ���
			ret = _device.ReadDigitalGain();

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�W�^���Q�C������������
		/// @param[in]		value	�f�W�^���Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void WriteDigitalGain(double value)
		{
			// �f�W�^���Q�C������������
			_device.WriteDigitalGain(value);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�I�����Ԃ�ǂݍ���
		/// @return			�I������
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API double ReadExposureTime()
		{
			double ret = 0;

			// �I�����Ԃ�ǂݍ���
			ret = _device.ReadExposureTime();

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�I�����Ԃ���������
		/// @param[in]		value	�I������
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API void WriteExposureTime(double value)
		{
			// �I�����Ԃ���������
			_device.WriteExposureTime(value);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��荞��
		/// @return			�摜�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		GRABBER_API Image::CObject Capture()
		{
			Image::CObject ret;

			// ��荞��
			ret = _device.Capture();

			return ret;
		}
	}
}