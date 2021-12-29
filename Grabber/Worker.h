#pragma once

#include "Common/Image/Basis.h"
#include "Common/Grabber/Condition.h"
#include "Common/Grabber/Setting.h"
#include "Device/Worker.h"

namespace Standard
{
	namespace Grabber
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      �J�����̐�����s���e���v���[�g�̃N���X
			///				�� �f�o�C�X�̐�����s���e���v���[�g�̃N���X����h��
			/// @param[in]	CDefault	�f�t�H���g�Ȑݒ�N���X���w��
			/// @param[in]	CConnect	�ڑ��̐ݒ�N���X���w��
			////////////////////////////////////////////////////////////////////////////////
			class CTemplate
				: virtual public Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>()
				{
					// ���̂��X�V
					SetName("Grabber::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��荞�݂̏����N���X���擾
				/// @return			��荞�݂̏����N���X
				////////////////////////////////////////////////////////////////////////////////
				Condition::CCapture GetCaptureCondition()
				{
					Condition::CCapture ret;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncCaptureCondition);

					// ��荞�݂̏����N���X���擾
					ret = m_captureCondition;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�A�i���O�Q�C����ǂݍ���
				/// @return			�A�i���O�Q�C���l
				////////////////////////////////////////////////////////////////////////////////
				double ReadAnalogGain() throw(...)
				{
					double ret = 0;

					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ReadAnalogGain()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// ���䂪�\���m�F
					IsControllable();

					// �A�i���O�Q�C����ǂݍ���
					ret = ReadAnalogGainDerived();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�A�i���O�Q�C������������
				/// @param[in]		value	�A�i���O�Q�C���l
				////////////////////////////////////////////////////////////////////////////////
				void WriteAnalogGain(double value) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WriteAnalogGain()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// ���䂪�\���m�F
					IsControllable();

					// �A�i���O�Q�C������������
					WriteAnalogGainDerived(value);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�W�^���Q�C����ǂݍ���
				/// @return			�f�W�^���Q�C���l
				////////////////////////////////////////////////////////////////////////////////
				double ReadDigitalGain() throw(...)
				{
					double ret = 0;

					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ReadDigitalGain()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// ���䂪�\���m�F
					IsControllable();

					// �f�W�^���Q�C����ǂݍ���
					ret = ReadDigitalGainDerived();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�W�^���Q�C������������
				/// @param[in]		value	�f�W�^���Q�C���l
				////////////////////////////////////////////////////////////////////////////////
				void WriteDigitalGain(double value) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WriteDigitalGain()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// ���䂪�\���m�F
					IsControllable();

					// �f�W�^���Q�C������������
					WriteDigitalGainDerived(value);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�I�����Ԃ�ǂݍ���
				/// @return			�I������
				////////////////////////////////////////////////////////////////////////////////
				double ReadExposureTime() throw(...)
				{
					double ret = 0;

					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ReadExposureTime()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// ���䂪�\���m�F
					IsControllable();

					// �I�����Ԃ�ǂݍ���
					ret = ReadExposureTimeDerived();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�I�����Ԃ���������
				/// @param[in]		value	�I������
				////////////////////////////////////////////////////////////////////////////////
				void WriteExposureTime(double value) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WriteExposureTime()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// ���䂪�\���m�F
					IsControllable();

					// �I�����Ԃ���������
					WriteExposureTimeDerived(value);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��荞��
				/// @return			�摜�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				Image::CObject Capture() throw(...)
				{
					Image::CObject ret;

					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Capture()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// ���䂪�\���m�F
					IsControllable();

					// ��荞��
					ret = CaptureDerived();

					return ret;
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��荞�݂̏����N���X���X�V
				/// @param[in]		object	��荞�݂̏����N���X
				////////////////////////////////////////////////////////////////////////////////
				void SetCaptureCondition(const Condition::CCapture& object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncCaptureCondition);

					// ��荞�݂̏����N���X���X�V
					m_captureCondition = object;
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ς݂̗�O�𔭖C
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionInitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionInitialized()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeGrabberInitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���ς݂̗�O�𔭖C
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionDiscarded() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionDiscarded()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeGrabberDiscarded, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���������̗�O�𔭖C
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionUninitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionUninitialized()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeGrabberUninitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�A�i���O�Q�C����ǂݍ���
				///	@detail			���h����ŋL�q����
				/// @return			�A�i���O�Q�C���l
				////////////////////////////////////////////////////////////////////////////////
				virtual double ReadAnalogGainDerived() throw(...)
				{
					double ret = 0;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�A�i���O�Q�C������������
				///	@detail			���h����ŋL�q����
				/// @param[in]		value	�A�i���O�Q�C���l
				////////////////////////////////////////////////////////////////////////////////
				virtual void WriteAnalogGainDerived(double value) throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�W�^���Q�C����ǂݍ���
				///	@detail			���h����ŋL�q����
				/// @return			�f�W�^���Q�C���l
				////////////////////////////////////////////////////////////////////////////////
				virtual double ReadDigitalGainDerived() throw(...)
				{
					double ret = 0;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�W�^���Q�C������������
				///	@detail			���h����ŋL�q����
				/// @param[in]		value	�f�W�^���Q�C���l
				////////////////////////////////////////////////////////////////////////////////
				virtual void WriteDigitalGainDerived(double value) throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�I�����Ԃ�ǂݍ���
				///	@detail			���h����ŋL�q����
				/// @return			�I������
				////////////////////////////////////////////////////////////////////////////////
				virtual double ReadExposureTimeDerived() throw(...)
				{
					double ret = 0;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�I�����Ԃ���������
				///	@detail			���h����ŋL�q����
				/// @param[in]		value	�I������
				////////////////////////////////////////////////////////////////////////////////
				virtual void WriteExposureTimeDerived(double value) throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��荞��
				///	@detail			���h����ŋL�q����
				/// @return			�摜�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				virtual Image::CObject CaptureDerived() throw(...)
				{
					Image::CObject ret;

					return ret;
				}

			private:
				//! �r������N���X(��荞�݂̏����N���X�p)
				std::recursive_mutex m_asyncCaptureCondition;

				//! ��荞�݂̏����N���X
				Condition::CCapture m_captureCondition;
			};
		}
	}
}

