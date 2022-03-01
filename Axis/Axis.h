#pragma once

#ifdef AXIS_EXPORTS
#define AXIS_API __declspec(dllexport)
#else
#define AXIS_API __declspec(dllimport)
#endif

#include <functional>

#include "Common/Exception/Basis.h"
#include "Common/Version/Basis.h"
#include "Common/Axis/Setting.h"
#include "Common/Axis/Status.h"

namespace Standard
{
	namespace Axis
	{
		// バージョンを取得
		AXIS_API Version::CObject GetVersion() throw(...);

		// 例外を通知する関数の登録
		AXIS_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// 例外を通知する関数の解除
		AXIS_API void DetachFunctionException();

		// 入力の変化を通知する関数の登録
		AXIS_API void AttachFunctionInput(std::function<void(const Status::CInput&)> object);

		// 入力の変化を通知する関数の解除
		AXIS_API void DetachFunctionInput();

		// 初期化
		AXIS_API void Initialize(const Setting::CDefault& object) throw(...);

		// 破棄
		AXIS_API void Destroy() throw(...);

		// 接続
		AXIS_API void Connect(const Setting::CConnect& object) throw(...);

		// 切断
		AXIS_API void Disconnect() throw(...);

		// サーボを制御
		AXIS_API void Servo(const Status::Output::CServo& object) throw(...);

		// 原点復帰を実行
		AXIS_API void MoveOrigin(const Status::Output::CMoveOrigin& object) throw(...);

		// ジョグを開始
		AXIS_API void StartJog(const Status::Output::CStartJog& object) throw(...);

		// ジョグを停止
		AXIS_API void StopJog(const Status::Output::CStopJog& object) throw(...);

		// 移動を開始
		AXIS_API void StartMove() throw(...);

		// 移動を停止
		AXIS_API void StopMove() throw(...);
	}
}
