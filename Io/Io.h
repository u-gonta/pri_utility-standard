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
		// バージョンを取得
		IO_API Version::CObject GetVersion() throw(...);

		// 例外を通知する関数の登録
		IO_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// 例外を通知する関数の解除
		IO_API void DetachFunctionException();

		// 入力の変化を通知する関数の登録
		IO_API void AttachFunctionInput(std::function<void(const Status::CInput&)> object);

		// 入力の変化を通知する関数の解除
		IO_API void DetachFunctionInput();

		// 初期化
		IO_API void Initialize(const Setting::CDefault& object) throw(...);

		// 破棄
		IO_API void Destroy() throw(...);

		// 接続
		IO_API void Connect(const Setting::CConnect& object) throw(...);

		// 切断
		IO_API void Disconnect() throw(...);

		// 入力
		IO_API Status::CInput ReadInput(const Setting::CInput& object) throw(...);

		// 出力
		IO_API void WriteOutput(const Status::COutput& object) throw(...);

		// 入力のトリガ待ち
		IO_API void TriggerInputPending(const Setting::CInputPending& object) throw(...);
	}
}