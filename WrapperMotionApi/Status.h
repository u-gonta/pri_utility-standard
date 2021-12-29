#pragma once

#include "Common/Iterator/Basis.h"

// 1バイトアラインメントに変更
#pragma pack(1)

namespace Standard
{
	namespace MotionApi
	{
		namespace Status
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @enum			運転ステータス
			////////////////////////////////////////////////////////////////////////////////
			enum EnumRunning
			{
				RunningReady = 0x00,				// 運転準備完了
				RunningServo = 0x01,				// 運転中(サーボON)
				RunningServoReady = 0x03,			// サーボレディ
				RunningOrigin = 0xFF				// 原点復帰完了
			};
			typedef Iterator::CWorker<EnumRunning, RunningReady, RunningServoReady> RunningIterator;

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			ワーニング
			////////////////////////////////////////////////////////////////////////////////
			enum EnumWarning
			{
				WarningDeviation = 0x00,			// 偏差異常
				WarningSettingParameter = 0x01,		// 設定パラメータ異常
				WarningFixParameter = 0x02,			// 固定パラメータ異常
				WarningDriver = 0x03,				// サーボドライバ異常
				WarningCommand = 0x04,				// モーションコマンド設定異常
				WarningServoUnfinished = 0x08,		// サーボON未完
				WarningServoCommunication = 0x09,	// サーボドライバ通信警告
				WarningServoStop = 0x0A				// サーボドライバ停止信号入力中
			};
			typedef Iterator::CWorker<EnumWarning, WarningDeviation, WarningServoStop> WarningIterator;

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			アラーム
			////////////////////////////////////////////////////////////////////////////////
			enum EnumAlarm
			{
				AlarmDriver = 0x00,					// サーボドライバ異常
				AlarmPositiveSoftLimit = 0x03,		// 正方向ソフトリミット
				AlarmNegativeSoftLimit = 0x04,		// 負方向ソフトリミット
				AlarmServoOff = 0x05,				// サーボOFF
				AlarmPositionTimeover = 0x06,		// 位置決めタイムオーバー
				AlarmExcessivePosition = 0x07,		// 位置決め移動量過大
				AlarmExcessiveSpeed = 0x08,			// 速度過大
				AlarmDeviation = 0x09				// 偏差異常
			};
			typedef Iterator::CWorker<EnumAlarm, AlarmDriver, AlarmDeviation> AlarmIterator;

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			位置
			////////////////////////////////////////////////////////////////////////////////
			enum EnumPosition
			{
				PositionPayoutCompleted = 0x00,		// 払い出し完了
				PositionCompleted = 0x01,			// 位置決め完了
				PositionOriginCompleted = 0x05		// 原点復帰完了
			};
			typedef Iterator::CWorker<EnumPosition, PositionPayoutCompleted, PositionOriginCompleted> PositionIterator;
		}
	}
}

// バイトアラインメントを戻す
#pragma pack()