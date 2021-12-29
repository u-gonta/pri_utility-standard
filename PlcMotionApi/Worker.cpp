#include "Worker.h"
#include "Parameter.h"
#include "Common/Tool/Variant.h"
#include "Common/Utility/StopWatch.h"

#include "WrapperMotionApi/include/ErrorCode.h"
#include "WrapperMotionApi/WrapperMotionApi.h"

#pragma comment(lib, "WrapperMotionApi.lib")

namespace Standard
{
	namespace Plc
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			コンストラクタ
		////////////////////////////////////////////////////////////////////////////////
		CWorker::CWorker() : Worker::CTemplate()
		{

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
		///					※Apiのタイムアウトなどを設定する
		///					★継承した関数
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::InitializeDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

			try
			{
				// デフォルトな設定クラスを取得
				Setting::CDefault defaultSetting = GetDefaultSetting();

				// Apiのタイムアウトを取得
				boost::any buffer = defaultSetting.Other.GetValue<EnumParameter>(ParameterApiTimeout);

				// Apiのタイムアウトが有効か確認
				if (Variant::IsValue<std::string>(buffer))
				{
					// Apiタイムアウトの設定
					MotionApi::Controller::Setting::CApiTimeout apiTimeout;

					// Apiのタイムアウトを取得
					apiTimeout.Input(Variant::Convert<std::string>(buffer));

					// Apiのタイムアウトを設定
					postscript << "MotionApi::SetApiTimeout(" << apiTimeout.Value << ")";
					MotionApi::SetApiTimeout(apiTimeout);
				}

				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
			Exception::EnumCode errorCode = Exception::CodePlcDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

			try
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
			Exception::EnumCode errorCode = Exception::CodePlcConnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

			try
			{
				// 接続設定クラスを取得
				Setting::CConnect connectSetting = GetConnectSetting();

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
			Exception::EnumCode errorCode = Exception::CodePlcDisconnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

			try
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
			Exception::EnumCode errorCode = Exception::CodePlcConnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "GetConnected()" << Logging::ConstSeparator;

			try
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

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
