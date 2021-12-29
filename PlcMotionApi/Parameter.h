#pragma once

#include "Common/Iterator/Basis.h"

namespace Standard
{
	namespace Plc
	{
		//! パラメータの定義
		enum EnumParameter
		{
			ParameterOpenController,		// コントローラを開く設定クラス
			ParameterApiTimeout,			// Apiタイムアウトの設定クラス
			ParameterDeclareAxis,			// 軸のハンドルを作成する設定クラス
			ParameterServoTimeout,			// サーボ制御のタイムアウト[ms]
			ParameterStartJog,				// 軸のジョグを実行する設定クラス
			ParameterStopJog				// 軸のジョグを停止する設定クラス
		};
	}
}
