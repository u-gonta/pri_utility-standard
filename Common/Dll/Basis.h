#pragma once

#include "Common/Version/Manager.h"

namespace Standard
{
	namespace Dll
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			アプリケーションのエントリポイントを処理
		/// @param[in]		hModule	ハンドル
		/// @param[in]		ul_reason_for_call	呼び出し
		/// @param[in]		lpReserved	戻り値
		////////////////////////////////////////////////////////////////////////////////
		inline void Main(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
		{
			Version::CManager& versionManager = Version::CManager::Instance();

			switch (ul_reason_for_call)
			{
			case DLL_PROCESS_ATTACH:
				// DLLがプロセスのアドレス空間にマッピングされた。 ⇒ モジュールIDで初期化
				versionManager.Initialize(hModule);
				break;

			case DLL_THREAD_ATTACH:
				// スレッドが作成されようとしている。
				break;

			case DLL_THREAD_DETACH:
				// スレッドが破棄されようとしている。 ⇒ 破棄
				versionManager.Destroy();
				break;

			case DLL_PROCESS_DETACH:
				// DLLのマッピングが解除されようとしている。
				break;
			}
		}
	}
}