#include "Standard.h"

#include "Common/Dll/Basis.h"
#include "Common/Version/Manager.h"

namespace Standard
{
	////////////////////////////////////////////////////////////////////////////////
	/// @brief			アプリケーションのエントリポイントを処理
	/// @param[in]		hModule	ハンドル
	/// @param[in]		ul_reason_for_call	呼び出し
	/// @param[in]		lpReserved	戻り値
	////////////////////////////////////////////////////////////////////////////////
	STANDARD_API void EntryPoint(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
	{
		// アプリケーションのエントリポイントを処理
		Dll::Main(hModule, ul_reason_for_call, lpReserved);
	}

	////////////////////////////////////////////////////////////////////////////////
	/// @brief			バージョンのクラスを取得
	////////////////////////////////////////////////////////////////////////////////
	STANDARD_API Version::CObject GetVersion()
	{
		Version::CObject ret;

		Version::CManager& versionManager = Version::CManager::Instance();

		// バージョンのクラスを取得
		ret = versionManager.GetVersion();

		return ret;
	}
}