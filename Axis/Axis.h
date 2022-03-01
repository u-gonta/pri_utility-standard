#pragma once

#ifdef AXIS_EXPORTS
#define AXIS_API __declspec(dllexport)
#else
#define AXIS_API __declspec(dllimport)
#endif

#include <functional>

#include "Common/Exception/Basis.h"
#include "Common/Version/Basis.h"
#include "Common/Axis/Setting.h"
#include "Common/Axis/Status.h"

namespace Standard
{
	namespace Axis
	{
		// �o�[�W�������擾
		AXIS_API Version::CObject GetVersion() throw(...);

		// ��O��ʒm����֐��̓o�^
		AXIS_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// ��O��ʒm����֐��̉���
		AXIS_API void DetachFunctionException();

		// ���͂̕ω���ʒm����֐��̓o�^
		AXIS_API void AttachFunctionInput(std::function<void(const Status::CInput&)> object);

		// ���͂̕ω���ʒm����֐��̉���
		AXIS_API void DetachFunctionInput();

		// ������
		AXIS_API void Initialize(const Setting::CDefault& object) throw(...);

		// �j��
		AXIS_API void Destroy() throw(...);

		// �ڑ�
		AXIS_API void Connect(const Setting::CConnect& object) throw(...);

		// �ؒf
		AXIS_API void Disconnect() throw(...);

		// �T�[�{�𐧌�
		AXIS_API void Servo(const Status::Output::CServo& object) throw(...);

		// ���_���A�����s
		AXIS_API void MoveOrigin(const Status::Output::CMoveOrigin& object) throw(...);

		// �W���O���J�n
		AXIS_API void StartJog(const Status::Output::CStartJog& object) throw(...);

		// �W���O���~
		AXIS_API void StopJog(const Status::Output::CStopJog& object) throw(...);

		// �ړ����J�n
		AXIS_API void StartMove() throw(...);

		// �ړ����~
		AXIS_API void StopMove() throw(...);
	}
}
