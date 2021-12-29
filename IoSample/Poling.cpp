#include "Poling.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
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
			/// @detail			メインスレッドから初期化のタイミングで呼び出される
			///					★継承した関数
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::InitializeDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CInitialize trigger;

				logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					Poling::CSetting setting = GetSetting();

					//@@@ 疑似的な待ち時間
					std::this_thread::sleep_for(std::chrono::milliseconds(500));

					// ★必須
					//@@@ 接続する処理なんかを記述する

					// ★必須
					// 初期化の完了を起床
					postscript << "WakeupInitializeComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupInitializeComplete(trigger);
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 初期化の完了を起床
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupInitializeComplete(trigger);

					// 例外を通知＆発砲
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			破棄
			/// @detail			メインスレッドから破棄のタイミングで呼び出される
			///					★継承した関数
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::DestroyDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoDestroy;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CDestroy trigger;

				logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					//@@@ 疑似的な待ち時間
					std::this_thread::sleep_for(std::chrono::milliseconds(700));

					// ★必須
					//@@@ 切断する処理なんかを記述する

					// ★必須
					// 破棄の完了を起床
					postscript << "WakeupDestroyComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupDestroyComplete(trigger);
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 破棄の完了を起床
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupDestroyComplete(trigger);

					// 例外を通知＆発砲
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			読み込み
			/// @detail			メインスレッドから読み込みのタイミングで呼び出される
			///					★継承した関数
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::ReadDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoRead;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "ReadDerived()" << Logging::ConstSeparator;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					Poling::CSetting setting = GetSetting();

					//@@@ 疑似的な待ち時間
					std::this_thread::sleep_for(std::chrono::milliseconds(100));

					// ★必須
					//@@@ 読み込みする処理なんかを記述する
					Status::CInput input;
					Setting::CAddressRange addressRange;

					// ★必須
					// 読み込みを通知
					NotifyArgument(input);

					// 読み込み
					postscript << "Read()" << Logging::ConstSeparator << stopWatch.Format(true);

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
			/// @brief			書き込み
			/// @detail			メインスレッドから書き込みのタイミングで呼び出される
			///					★継承した関数
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::WriteDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "WriteDerived()" << Logging::ConstSeparator;

				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					Poling::CSetting setting = GetSetting();

					//@@@ 疑似的な待ち時間
					std::this_thread::sleep_for(std::chrono::milliseconds(200));

					do
					{
						// ★必須
						//@@@ 書き込みする処理なんかを記述する
						auto write = GetWriteFirst();

						// 書き込みクラスを確認
						if (write == nullptr)
						{
							break;
						}

						// ★必須
						// 先頭に登録してあるデータを削除
						PopWriteFirst();
					} while (false);

					// 書き込み
					postscript << "Write()" << Logging::ConstSeparator << stopWatch.Format(true);

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
		}
	}
}