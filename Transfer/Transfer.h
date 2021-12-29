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
		// バージョンを取得
		TRANSFER_API Version::CObject GetVersion() throw(...);

		// 例外の通知する関数を登録
		TRANSFER_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// 例外の通知する関数を解除
		TRANSFER_API void DetachFunctionException();

		// 初期化
		TRANSFER_API void Initialize(const Setting::CObject& object) throw(...);

		// 破棄
		TRANSFER_API void Destroy() throw(...);

		// 出力
		TRANSFER_API void Output(const Logging::CObject& value) throw(...);

		// 出力
		TRANSFER_API void Output(Logging::EnumLevel level, const std::string& message) throw(...);
	}
}