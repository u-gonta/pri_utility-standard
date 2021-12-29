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
	// アプリケーションのエントリポイントを処理
	STANDARD_API void EntryPoint(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) throw(...);

	// バージョンを取得
	STANDARD_API Version::CObject GetVersion() throw(...);
}