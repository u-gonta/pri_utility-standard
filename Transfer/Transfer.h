#pragma once

#ifdef TRANSFER_EXPORTS
#define TRANSFER_API __declspec(dllexport)
#else
#define TRANSFER_API __declspec(dllimport)
#endif

#include <functional>

#include "Common/Exception/Basis.h"
#include "Common/Logging/Basis.h"
#include "Common/Version/Basis.h"
#include "Common/Transfer/Setting.h"

namespace Standard
{
	namespace Transfer
	{
		// �o�[�W�������擾
		TRANSFER_API Version::CObject GetVersion() throw(...);

		// ��O�̒ʒm����֐���o�^
		TRANSFER_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// ��O�̒ʒm����֐�������
		TRANSFER_API void DetachFunctionException();

		// ������
		TRANSFER_API void Initialize(const Setting::CObject& object) throw(...);

		// �j��
		TRANSFER_API void Destroy() throw(...);

		// �o��
		TRANSFER_API void Output(const Logging::CObject& value) throw(...);

		// �o��
		TRANSFER_API void Output(Logging::EnumLevel level, const std::string& message) throw(...);
	}
}