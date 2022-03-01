#pragma once

#include "Execution.h"
#include "SettingController.h"
#include "Result.h"

namespace Standard
{
	namespace Plc
	{
		namespace Controller
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コントローラを初期化
			/// @param[in]		object コントローラを初期化する設定クラス
			/// @return			コントローラを初期化した結果を保持するクラス
			////////////////////////////////////////////////////////////////////////////////
			inline Result::CInitialize Initialize(const Setting::CInitialize& object)
			{
				Result::CInitialize ret;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodePlcInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Initialize()" << Logging::ConstSeparator;

				try
				{
					// コントローラを開く設定クラス
					MotionApi::Controller::Setting::COpen setting = object.Open;

					// コントローラと接続
					postscript << "MotionApi::OpenController()";
					ret.Controller = MotionApi::OpenController(setting);
					// ログ出力
					postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(ret.Controller.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// Apiタイムアウトの設定クラス
					MotionApi::Controller::Setting::CApiTimeout apiTimeout = object.ApiTimeout;

					// Apiのタイムアウトを設定
					postscript << "MotionApi::SetApiTimeout()" << Logging::ConstSeparator << "タイムアウト:" << apiTimeout.Value;
					MotionApi::SetApiTimeout(apiTimeout);
					// ログ出力
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コントローラを破棄
			/// @param[in]		object コントローラを破棄する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			inline void Destroy(const Setting::CDestroy& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodePlcDestroy;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Destroy()" << Logging::ConstSeparator;

				try
				{
					do
					{
						// コントローラを閉じる設定クラス
						MotionApi::Controller::Setting::CClose setting = object.Close;

						// ハンドルを確認
						if (setting.Handle == MotionApi::ConstDisconnected)
						{
							// ログ出力
							postscript << Logging::ConstSeparator << "未初期化";
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
							break;
						}

						// コントローラと切断
						postscript << "MotionApi::CloseController()";
						MotionApi::CloseController(setting);
						// ログ出力
						postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(setting.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					} while (false);
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}
		}
	}
}