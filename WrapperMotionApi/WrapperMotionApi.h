#pragma once

#ifdef MOTION_API_EXPORTS
#define MOTION_API __declspec(dllexport)
#else
#define MOTION_API __declspec(dllimport)
#endif

#include "SettingController.h"
#include "SettingRegister.h"
#include "SettingAxis.h"
#include "SettingDevice.h"
#include "Result.h"

namespace Standard
{
	namespace MotionApi
	{
		// コントローラに接続
		MOTION_API Controller::Result::COpen OpenController(const Controller::Setting::COpen& object) throw(...);

		// Apiのタイムアウトを設定
		MOTION_API void SetApiTimeout(const Controller::Setting::CApiTimeout& object) throw(...);

		// コントローラと切断
		MOTION_API void CloseController(const Controller::Setting::CClose& object) throw(...);

		// レジスタのハンドルを取得
		MOTION_API Register::Result::CDeclare DeclareRegister(const Register::Setting::CDeclare& object) throw(...);

		// レジスタの値を取得
		MOTION_API Register::Result::CContent GetRegisterData(const Register::Setting::CGetContent& object) throw(...);

		// レジスタの値を更新
		MOTION_API void SetRegisterData(const Register::Setting::CSetContent& object) throw(...);

		// 軸のハンドルを消去
		MOTION_API void ClearAllAxis() throw(...);

		// 軸のハンドルを作成
		MOTION_API Axis::Result::CDeclare DeclareAxis(const Axis::Setting::CDeclare& object) throw(...);

		// 軸のハンドルをクリア
		MOTION_API void ClearAxis(const Axis::Setting::CClear& object) throw(...);

		// デバイスのハンドルを作成
		MOTION_API Device::Result::CDeclare DeclareDevice(const Device::Setting::CDeclare& object);

		// デバイスのハンドルをクリア
		MOTION_API void ClearDevice(const Device::Setting::CClear& object);

		// 軸のサーボを制御
		MOTION_API void ServoControl(const Device::Setting::CServo& object);

		// 軸のジョグ送りを実行
		MOTION_API void StartJog(const Device::Setting::CStartJog& object);

		// 軸のジョグ送りを停止
		MOTION_API void StopJog(const Device::Setting::CStopJog& object);
	}
}