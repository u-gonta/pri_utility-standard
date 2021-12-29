#include "Worker.h"
#include "Common/Utility/StopWatch.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			コンストラクタ
		////////////////////////////////////////////////////////////////////////////////
		CWorker::CWorker() : Worker::CTemplate()
		{
			m_connected = false;
			m_analogGain = 1;
			m_digitalGain = 5;
			m_exposureTime = 128;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			デストラクタ
		////////////////////////////////////////////////////////////////////////////////
		CWorker::~CWorker()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			初期化
		/// @detail			初期化のタイミングで呼び出される
		///					★継承した関数
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::InitializeDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

			try
			{
				auto defaultSetting = GetDefaultSetting();

				//@@@ 疑似的な待ち時間
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// ★必須
				//@@@ 初期化する処理なんかを記述する

				postscript << "画像の幅:" << defaultSetting.ImageWidth << Logging::ConstSeparator << "画像の高さ:" << defaultSetting.ImageHeight;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			破棄
		/// @detail			破棄のタイミングで呼び出される
		///					★継承した関数
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DestroyDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

			try
			{
				//@@@ 疑似的な待ち時間
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// ★必須
				//@@@ 破棄する処理なんかを記述する

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			接続
		/// @detail			接続のタイミングで呼び出される
		///					★継承した関数
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::ConnectDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberConnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

			try
			{
				auto connectSetting = GetConnectSetting();

				//@@@ 疑似的な待ち時間
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// ★必須
				//@@@ 接続する処理なんかを記述する

				postscript << "カメラのIPアドレス:" << connectSetting.Identifier;

				// 接続の状態を接続に更新
				SetConnected(true);

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			切断
		/// @detail			切断のタイミングで呼び出される
		///					★継承した関数
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DisconnectDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDisconnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

			try
			{
				//@@@ 疑似的な待ち時間
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// ★必須
				//@@@ 切断する処理なんかを記述する

				// 接続の状態を未接続に更新
				SetConnected(false);

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			接続の状態を取得
		///	@detail			接続の状態を取得するタイミングで呼び出される
		///					★継承した関数
		/// @return			true:接続済み / false:未接続
		////////////////////////////////////////////////////////////////////////////////
		bool CWorker::GetConnected()
		{
			bool ret = false;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "GetConnected()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ 接続の状態を取得する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				ret = m_connected;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			接続の状態を更新
		/// @param[in]		value	true:接続済み / false:未接続
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::SetConnected(bool value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "SetConnected()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ 接続の状態を更新する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				m_connected = value;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			アナログゲインを読み込み
		///	@detail			★継承した関数
		/// @return			アナログゲイン値
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadAnalogGainDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadAnalogGainDerived()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ アナログゲインを取得する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// アナログゲインを取得
				ret = m_analogGain;

				postscript << ret;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			アナログゲインを書き込み
		///	@detail			★継承した関数
		/// @param[in]		value	アナログゲイン値
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteAnalogGainDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteAnalogGainDerived()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ アナログゲインを更新する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// アナログゲインを更新
				m_analogGain = value;

				postscript << m_analogGain;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			デジタルゲインを読み込み
		///	@detail			★継承した関数
		/// @return			デジタルゲイン値
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadDigitalGainDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadAnalogGainDerived()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ デジタルゲインを取得する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// デジタルゲインを取得
				ret = m_digitalGain;

				postscript << ret;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			デジタルゲインを書き込み
		///	@detail			★継承した関数
		/// @param[in]		value	デジタルゲイン値
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteDigitalGainDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteDigitalGainDerived()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ デジタルゲインを更新する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// デジタルゲインを更新
				m_digitalGain = value;

				postscript << m_digitalGain;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			露光時間を読み込み
		///	@detail			★継承した関数
		/// @return			露光時間
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadExposureTimeDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadExposureTimeDerived()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ 露光時間を取得する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// 露光時間を取得
				ret = m_exposureTime;

				postscript << ret;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			露光時間を書き込み
		///	@detail			★継承した関数
		/// @param[in]		value	露光時間
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteExposureTimeDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteExposureTimeDerived()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ 露光時間を更新する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// 露光時間を更新
				m_exposureTime = value;

				postscript << m_exposureTime;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			取り込み
		///	@detail			★継承した関数
		/// @return			画像のクラス
		////////////////////////////////////////////////////////////////////////////////
		Image::CObject CWorker::CaptureDerived()
		{
			Image::CObject ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberCapture;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "CaptureDerived()" << Logging::ConstSeparator;

			try
			{
				// ★必須
				//@@@ 画像を取得する処理を記述

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncDmy);

				// 画像を取得
				ret = m_image;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}
	}
}
