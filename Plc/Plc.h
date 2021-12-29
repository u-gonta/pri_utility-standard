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
		// バージョンを取得
		PLC_API Version::CObject GetVersion() throw(...);

		// 例外を通知する関数の登録
		PLC_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// 例外を通知する関数の解除
		PLC_API void DetachFunctionException();

		// 初期化
		PLC_API void Initialize(const Setting::CDefault& object) throw(...);

		// 破棄
		PLC_API void Destroy() throw(...);

		// 接続
		PLC_API void Connect(const Setting::CConnect& object) throw(...);

		// 切断
		PLC_API void Disconnect() throw(...);
	}
}