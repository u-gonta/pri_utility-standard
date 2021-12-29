#pragma once

namespace Standard
{
	namespace Exception
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @enum			例外コード
		////////////////////////////////////////////////////////////////////////////////
		enum EnumCode
		{
			CodeSuccess = 0,							//! 正常
#pragma region バージョン(0x7000～)
			CodeVersion = 0x7000,						//! バージョンの取得に失敗
#pragma endregion
#pragma region ログ(0x7100～0x71FF)
			CodeTransferInitialized = 0x7100,			//! ログ転送の初期化が実施済み
			CodeTransferDiscarded = 0x7101,				//! ログ転送の破棄が実施済み
#pragma endregion
#pragma region カメラ(0x7200～0x72FF)
			CodeGrabberDeviceLost = 0x7200,				//! カメラの切断
			CodeGrabberInitialized = 0x7201,			//! カメラの初期化が実施済み
			CodeGrabberUninitialized = 0x7202,			//! カメラの未初期化
			CodeGrabberDiscarded = 0x7203,				//! カメラの破棄が実施済み
			CodeGrabberInterface = 0x7204,				//! カメラのインターフェースなし
			CodeGrabberDataStream = 0x7205,				//! カメラのデータストリームなし
			CodeGrabberDevice = 0x7206,					//! カメラの接続なし
			CodeGrabberInitialize = 0x7210,				//! カメラの初期化に失敗
			CodeGrabberDestroy = 0x7220,				//! カメラの破棄に失敗
			CodeGrabberConnect = 0x7230,				//! カメラの接続に失敗
			CodeGrabberDisconnect = 0x7240,				//! カメラの切断に失敗
			CodeGrabberParameterGet = 0x7250,			//! カメラのパラメータ取得に失敗
			CodeGrabberParameterSet = 0x7260,			//! カメラのパラメータ更新に失敗
			CodeGrabberCapture = 0x7270,				//! カメラの取り込みに失敗
			CodeGrabberCaptureNotStart = 0x7271,		//! カメラの取り込みが未実施
			CodeGrabberCaptureTimeout = 0x7272,			//! カメラの取り込みタイムアウト
			CodeGrabberCaptureNotImage = 0x7273,		//! カメラの取り込みで画像データなし
			CodeGrabberCaptureNotFormat = 0x7274,		//! カメラの取り込みで画像フォーマット未対応
#pragma endregion
#pragma region IO(0x7300～0x73FF)
			CodeIoInitialized = 0x7301,					//! IOの初期化が実施済み
			CodeIoUninitialized = 0x7302,				//! IOの未初期化
			CodeIoDiscarded = 0x7303,					//! IOの破棄が実施済み
			CodeIoInitialize = 0x7310,					//! IOの初期化に失敗
			CodeIoDestroy = 0x7320,						//! IOの破棄に失敗
			CodeIoConnect = 0x7330,						//! IOの接続に失敗
			CodeIoDisconnect = 0x7340,					//! IOの切断に失敗
			CodeIoRead = 0x7350,						//! IOの読み込みに失敗
			CodeIoWrite = 0x7360,						//! IOの書き込みに失敗
#pragma endregion
#pragma region 軸(0x7400～0x74FF)
			CodeAxisInitialized = 0x7401,				//! 軸の初期化が実施済み
			CodeAxisUninitialized = 0x7402,				//! 軸の未初期化
			CodeAxisDiscarded = 0x7403,					//! 軸の破棄が実施済み
			CodeAxisInitialize = 0x7410,				//! 軸の初期化に失敗
			CodeAxisDestroy = 0x7420,					//! 軸の破棄に失敗
			CodeAxisConnect = 0x7430,					//! 軸の接続に失敗
			CodeAxisDisconnect = 0x7440,				//! 軸の切断に失敗
			CodeAxisRead = 0x7450,						//! 軸の状態確認に失敗
			CodeAxisWrite = 0x7460,						//! 軸の制御に失敗
#pragma endregion
#pragma region Plc(0x7500～0x75FF)
			CodePlcInitialized = 0x7501,				//! PLCの初期化が実施済み
			CodePlcUninitialized = 0x7502,				//! PLCの未初期化
			CodePlcDiscarded = 0x7503,					//! PLCの破棄が実施済み
			CodePlcInitialize = 0x7510,					//! PLCの初期化に失敗
			CodePlcDestroy = 0x7520,					//! PLCの破棄に失敗
			CodePlcConnect = 0x7530,					//! PLCの接続に失敗
			CodePlcDisconnect = 0x7540,					//! PLCの切断に失敗
			CodePlcRead = 0x7550,						//! PLCの状態確認に失敗
			CodePlcWrite = 0x7560,						//! PLCの制御に失敗
#pragma endregion
#pragma region 画像(0x9000～0x901F)
			CodeImageUpdateDepth = 0x9000,				//! 画像の更新に失敗(ビット深度が異常)
			CodeImageUpdateHeight = 0x9001,				//! 画像の更新に失敗(高さが異常)
			CodeImageUpdateWidth = 0x9002,				//! 画像の更新に失敗(幅が異常)
			CodeImageUpdateMat = 0x9003,				//! 画像の更新に失敗(Matが空)
			CodeImageConvertEmpty = 0x9004,				//! 画像の変換に失敗(画像データが空)
#pragma endregion
#pragma region Udp(0x9020～0x902F)
			CodeUdpConnect = 0x9020,					//! Udpの接続に失敗
			CodeUdpWrite = 0x9021,						//! Udpの送信に失敗
#pragma endregion
			CodeUnknown = 0x9999						//! 不明なエラー
		};

		////////////////////////////////////////////////////////////////////////////////
		/// @enum			機器の例外コード
		////////////////////////////////////////////////////////////////////////////////
		enum EnumDeviceCode
		{
			DeviceCodeSuccess = 0,			//! 正常
		};
	}
}