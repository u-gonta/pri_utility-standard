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
			ParameterSuspendTimeout,		// 制御の停止タイムアウト[ms]
			ParameterServoTimeout,			// サーボ制御のタイムアウト[ms]
			ParameterOriginAxis,			// 軸の原点復帰を設定するクラス
			ParameterMoveOriginTimeout,		// 原点復帰のタイムアウト[ms]
			ParameterStartJog,				// ジョグの開始を設定するクラス
			ParameterStopJog				// ジョグの停止を設定するクラス
		};
	}
}
