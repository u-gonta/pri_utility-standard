#pragma once

#ifdef IO_EXPORTS
#define IO_API __declspec(dllexport)
#else
#define IO_API __declspec(dllimport)
#endif

#include <functional>

#include "Common/Exception/Basis.h"
#include "Common/Version/Basis.h"
#include "Common/Io/Setting.h"
#include "Common/Io/Status.h"

namespace Standard
{
	namespace Io
	{
		// �o�[�W�������擾
		IO_API Version::CObject GetVersion() throw(...);

		// ��O��ʒm����֐��̓o�^
		IO_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// ��O��ʒm����֐��̉���
		IO_API void DetachFunctionException();

		// ���͂̕ω���ʒm����֐��̓o�^
		IO_API void AttachFunctionInput(std::function<void(const Status::CInput&)> object);

		// ���͂̕ω���ʒm����֐��̉���
		IO_API void DetachFunctionInput();

		// ������
		IO_API void Initialize(const Setting::CDefault& object) throw(...);

		// �j��
		IO_API void Destroy() throw(...);

		// �ڑ�
		IO_API void Connect(const Setting::CConnect& object) throw(...);

		// �ؒf
		IO_API void Disconnect() throw(...);

		// ����
		IO_API Status::CInput ReadInput(const Setting::CInput& object) throw(...);

		// �o��
		IO_API void WriteOutput(const Status::COutput& object) throw(...);

		// ���͂̃g���K�҂�
		IO_API void TriggerInputPending(const Setting::CInputPending& object) throw(...);
	}
}