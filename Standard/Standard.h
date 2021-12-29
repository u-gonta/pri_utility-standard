#pragma once

#ifdef STANDARD_EXPORTS
#define STANDARD_API __declspec(dllexport)
#else
#define STANDARD_API __declspec(dllimport)
#endif

#include "pch.h"

#include "Common/Version/Basis.h"

namespace Standard
{
	// �A�v���P�[�V�����̃G���g���|�C���g������
	STANDARD_API void EntryPoint(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) throw(...);

	// �o�[�W�������擾
	STANDARD_API Version::CObject GetVersion() throw(...);
}