#pragma once

#ifdef PLC_EXPORTS
#define PLC_API __declspec(dllexport)
#else
#define PLC_API __declspec(dllimport)
#endif

#include "boost/function.hpp"

#include "Common/Exception/Basis.h"
#include "Common/Version/Basis.h"
#include "Common/Plc/Setting.h"

namespace Standard
{
	namespace Plc
	{
		// �o�[�W�������擾
		PLC_API Version::CObject GetVersion() throw(...);

		// ��O��ʒm����֐��̓o�^
		PLC_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// ��O��ʒm����֐��̉���
		PLC_API void DetachFunctionException();

		// ������
		PLC_API void Initialize(const Setting::CDefault& object) throw(...);

		// �j��
		PLC_API void Destroy() throw(...);

		// �ڑ�
		PLC_API void Connect(const Setting::CConnect& object) throw(...);

		// �ؒf
		PLC_API void Disconnect() throw(...);
	}
}