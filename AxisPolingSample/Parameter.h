#pragma once

namespace Standard
{
	namespace Axis
	{
		//! パラメータの定義
		enum EnumParameter
		{
			ParameterConnectTimeout,		// 接続時のタイムアウト[ms]
			ParameterDisconnectTimeout,		// 切断時のタイムアウト[ms]
			ParameterWriteTimeout,			// 制御時のタイムアウト[ms]
			ParameterReadTimeout			// 監視時のタイムアウト[ms]
		};
	}
}