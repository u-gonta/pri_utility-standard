#pragma once

#ifdef GRABBER_EXPORTS
#define GRABBER_API __declspec(dllexport)
#else
#define GRABBER_API __declspec(dllimport)
#endif

#include <functional>

#include "Common/Exception/Basis.h"
#include "Common/Version/Basis.h"
#include "Common/Image/Basis.h"
#include "Common/Grabber/Condition.h"
#include "Common/Grabber/Setting.h"

namespace Standard
{
	namespace Grabber
	{
		// バージョンを取得
		GRABBER_API Version::CObject GetVersion() throw(...);

		// 例外を通知する関数の登録
		GRABBER_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object);

		// 例外を通知する関数の解除
		GRABBER_API void DetachFunctionException();

		// 初期化
		GRABBER_API void Initialize(const Setting::CDefault& object) throw(...);

		// 破棄
		GRABBER_API void Destroy() throw(...);

		// 接続
		GRABBER_API void Connect(const Setting::CConnect& object) throw(...);

		// 切断
		GRABBER_API void Disconnect() throw(...);

		// 取り込みの条件クラスを取得
		GRABBER_API Condition::CCapture GetCaptureCondition() throw(...);

		// アナログゲインを読み込み
		GRABBER_API double ReadAnalogGain() throw(...);

		// アナログゲインを書き込み
		GRABBER_API void WriteAnalogGain(double value) throw(...);

		// デジタルゲインを読み込み
		GRABBER_API double ReadDigitalGain() throw(...);

		// デジタルゲインを書き込み
		GRABBER_API void WriteDigitalGain(double value) throw(...);

		// 露光時間を読み込み
		GRABBER_API double ReadExposureTime() throw(...);

		// 露光時間を書き込み
		GRABBER_API void WriteExposureTime(double value) throw(...);

		// 取り込み
		GRABBER_API Image::CObject Capture() throw(...);
	}
}