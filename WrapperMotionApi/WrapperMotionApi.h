#pragma once

#ifdef MOTION_API_EXPORTS
#define MOTION_API __declspec(dllexport)
#else
#define MOTION_API __declspec(dllimport)
#endif

#include "SettingController.h"
#include "SettingRegister.h"
#include "SettingAxis.h"
#include "SettingDevice.h"
#include "Result.h"

namespace Standard
{
	namespace MotionApi
	{
		// �R���g���[���ɐڑ�
		MOTION_API Controller::Result::COpen OpenController(const Controller::Setting::COpen& object) throw(...);

		// Api�̃^�C���A�E�g��ݒ�
		MOTION_API void SetApiTimeout(const Controller::Setting::CApiTimeout& object) throw(...);

		// �R���g���[���Ɛؒf
		MOTION_API void CloseController(const Controller::Setting::CClose& object) throw(...);

		// ���W�X�^�̃n���h�����擾
		MOTION_API Register::Result::CDeclare DeclareRegister(const Register::Setting::CDeclare& object) throw(...);

		// ���W�X�^�̒l���擾
		MOTION_API Register::Result::CContent GetRegisterData(const Register::Setting::CGetContent& object) throw(...);

		// ���W�X�^�̒l���X�V
		MOTION_API void SetRegisterData(const Register::Setting::CSetContent& object) throw(...);

		// ���̃n���h��������
		MOTION_API void ClearAllAxis() throw(...);

		// ���̃n���h�����쐬
		MOTION_API Axis::Result::CDeclare DeclareAxis(const Axis::Setting::CDeclare& object) throw(...);

		// ���̃n���h�����N���A
		MOTION_API void ClearAxis(const Axis::Setting::CClear& object) throw(...);

		// �f�o�C�X�̃n���h�����쐬
		MOTION_API Device::Result::CDeclare DeclareDevice(const Device::Setting::CDeclare& object);

		// �f�o�C�X�̃n���h�����N���A
		MOTION_API void ClearDevice(const Device::Setting::CClear& object);

		// ���̃T�[�{�𐧌�
		MOTION_API void ServoControl(const Device::Setting::CServo& object);

		// ���̃W���O��������s
		MOTION_API void StartJog(const Device::Setting::CStartJog& object);

		// ���̃W���O������~
		MOTION_API void StopJog(const Device::Setting::CStopJog& object);
	}
}