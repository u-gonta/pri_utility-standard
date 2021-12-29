#pragma once

#ifdef GRABBER_EXPORTS
#define GRABBER_API __declspec(dllexport)
#else
#define GRABBER_API __declspec(dllimport)
#endif

#include <functional>

#include "Common/Exception/Basis.h"
#include "Common/Version/Basis.h"
#include "Common/Image/Basis.h"
#include "Common/Grabber/Condition.h"
#include "Common/Grabber/Setting.h"

namespace Standard
{
	namespace Grabber
	{
		// �o�[�W�������擾
		GRABBER_API Version::CObject GetVersion() throw(...);

		// ��O��ʒm����֐��̓o�^
		GRABBER_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// ��O��ʒm����֐��̉���
		GRABBER_API void DetachFunctionException();

		// ������
		GRABBER_API void Initialize(const Setting::CDefault& object) throw(...);

		// �j��
		GRABBER_API void Destroy() throw(...);

		// �ڑ�
		GRABBER_API void Connect(const Setting::CConnect& object) throw(...);

		// �ؒf
		GRABBER_API void Disconnect() throw(...);

		// ��荞�݂̏����N���X���擾
		GRABBER_API Condition::CCapture GetCaptureCondition() throw(...);

		// �A�i���O�Q�C����ǂݍ���
		GRABBER_API double ReadAnalogGain() throw(...);

		// �A�i���O�Q�C������������
		GRABBER_API void WriteAnalogGain(double value) throw(...);

		// �f�W�^���Q�C����ǂݍ���
		GRABBER_API double ReadDigitalGain() throw(...);

		// �f�W�^���Q�C������������
		GRABBER_API void WriteDigitalGain(double value) throw(...);

		// �I�����Ԃ�ǂݍ���
		GRABBER_API double ReadExposureTime() throw(...);

		// �I�����Ԃ���������
		GRABBER_API void WriteExposureTime(double value) throw(...);

		// ��荞��
		GRABBER_API Image::CObject Capture() throw(...);
	}
}