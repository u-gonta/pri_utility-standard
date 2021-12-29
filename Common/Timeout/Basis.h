#pragma once

namespace Standard
{
	namespace Timeout
	{
		//! カメラのハートビート[ms]
		const double ConstGrabberHeatBeat = 3000000;

		//! カメラの取り込み[ms]
		const double ConstGrabberCapture = 1000;

		//! モーションApi[ms]
		const int ConstMotionApi = 3000;

		//! モーションApiのサーボ[ms]
		const unsigned short ConstMotionServo = 5000;
	}
}