// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"

#include "Common/Dll/Basis.h"

using namespace Standard;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	// アプリケーションのエントリポイントを処理
	Dll::Main(hModule, ul_reason_for_call, lpReserved);

	return TRUE;
}