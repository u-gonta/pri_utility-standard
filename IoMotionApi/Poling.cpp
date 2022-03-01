#include "Poling.h"
#include "Common/Tool/Variant.h"
#include "Common/Io/Setting.h"

#include "PlcMotionApi/Parameter.h"
#include "PlcMotionApi/ExecutionController.h"
#include "PlcMotionApi/ExecutionRegister.h"

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
				m_controller.Handle = MotionApi::ConstDisconnected;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CWorker::~CWorker()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			接続の状態を取得
			/// @return			true:接続済み / false:未接続
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::GetConnected()
			{
				bool ret = false;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncController);

				// ハンドルを確認
				if (m_controller.Handle != MotionApi::ConstDisconnected)
				{
					ret = true;
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			接続済みを確認
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::IsConnected()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoConnect;
				int deviceErrorCode = Exception::DeviceCodeSuccess;

				logging.Message << GetName() << "IsConnected()" << Logging::ConstSeparator;

				try
				{
					// 接続の状態を取得
					postscript << "GetConnected()";
					if (GetConnected() == false)
					{
						// 未接続 ⇒ 例外を発砲
						throw std::exception("未接続");
					}
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を通知＆発砲
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
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

				try
				{
					Poling::CSetting polingSetting = GetSetting();
					Plc::Controller::Setting::CInitialize setting;

					// 名称を更新
					setting.Name = GetName();

					// コントローラを開く設定を取得
					postscript << "OpenController";
					boost::any buffer = polingSetting.Connect.Other.GetValue<Plc::EnumParameter>(Plc::ParameterOpenController);

					// コントローラを開く設定が有効か確認
					if (Variant::IsValue<std::string>(buffer))
					{
						// コントローラを開く設定を更新
						setting.Open.Input(Variant::Convert<std::string>(buffer));
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
					}
					postscript.str("");

					// Apiのタイムアウトを取得
					postscript << "ApiTimeout";
					buffer = polingSetting.Connect.Other.GetValue<Plc::EnumParameter>(Plc::ParameterApiTimeout);

					// Apiのタイムアウトが有効か確認
					if (Variant::IsValue<std::string>(buffer))
					{
						// Apiのタイムアウトを更新
						setting.ApiTimeout.Input(Variant::Convert<std::string>(buffer));
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
					}

					// コントローラを初期化
					postscript << "Plc::Controller::Initialize()";
					auto resultInitialize = Plc::Controller::Initialize(setting);
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncController);

					// コントローラの情報を更新
					m_controller = resultInitialize.Controller;

					// 初期化の完了を起床
					postscript << "WakeupInitializeComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupInitializeComplete(trigger);
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 初期化の完了を起床
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupInitializeComplete(trigger);

					// 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
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

				try
				{
					do
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncController);

						// 接続の状態を確認
						if (GetConnected() == false)
						{
							// 未接続
							postscript << "未接続";
							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
							break;
						}

						Plc::Controller::Setting::CDestroy setting;

						// 名称を更新
						setting.Name = GetName();

						// ハンドルを更新
						setting.Close.Handle = m_controller.Handle;

						// コントローラを破棄
						postscript << "Plc::Controller::Destroy()";
						Plc::Controller::Destroy(setting);
						postscript.str("");

						// ハンドルを初期化
						m_controller.Handle = MotionApi::ConstDisconnected;
					} while (false);

					// 破棄の完了を起床
					postscript << "WakeupDestroyComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupDestroyComplete(trigger);
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 破棄の完了を起床
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupDestroyComplete(trigger);

					// 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			リトライを確認
			///	@detail			正常に読み込みが行われるまで制限回数の間はリトライを有効
			///					★継承した関数
			/// @return			true:リトライが有効 / false:リトライが無効
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::IsRetry()
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "IsRetry()" << Logging::ConstSeparator;

				Poling::CSetting polingSetting = GetSetting();
				int retry = GetRetry();

				logging.Message << "リトライ" << retry << "回目" << Logging::ConstSeparator;

				// リトライ回数を判定
				if (retry <= polingSetting.Retry)
				{
					// リトライ回数が設定以下 ⇒ リトライを有効
					ret = true;
					logging.Message << "有効";
				}
				else
				{
					// リトライ回数が設定より大きい
					logging.Message << "無効";
				}
				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				return ret;
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
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncController);

					// 接続済みを確認
					postscript << "IsConnected()";
					IsConnected();
					postscript.str("");

					Poling::CSetting setting = GetSetting();

					Status::CInput input;

					// 型の範囲を走査
					for (auto addressRange : setting.Connect.AddressRanges)
					{
						Plc::Register::CSpecify specify(addressRange.Start.Category);
						Plc::Register::Setting::CGetContent content;

						// 名称を更新
						content.Name = GetName();

						// レジスタの種別を確定
						content.Category = addressRange.Start.Category;
						// レジスタの番号を確定
						content.Index = addressRange.Start.Index;
						// レジスタの範囲を確定
						content.Size = addressRange.Size;

						// レジスタの読み込み
						postscript << "Plc::Register::Get()";
						auto result = Plc::Register::Get(content);
						postscript.str("");

						// レジスタの値を格納
						for (const auto& bit : result.Bits)
						{
							// ビット型
							Io::Specify::CAddress address = addressRange.Start;

							// アドレスの変換を確定
							address.Hex = true;
							address.Margin = specify.GetMargin() + 1;

							// アドレスを確定
							address.Index += bit.first;
							input.Values[address] = (bool)(bit.second != 0 ? true : false);
						}
						for (const auto& numeric : result.Words)
						{
							Io::Specify::CAddress address = addressRange.Start;

							// アドレスの変換を確定
							address.Hex = true;
							address.Margin = specify.GetMargin();

							// アドレスを確定
							address.Index += numeric.first;
							input.Values[address] = (__int16)numeric.second;
						}
						for (const auto& numeric : result.Longs)
						{
							Io::Specify::CAddress address = addressRange.Start;

							// アドレスの変換を確定
							address.Hex = true;
							address.Margin = specify.GetMargin();

							// アドレスを確定
							address.Index += numeric.first;
							input.Values[address] = (__int32)numeric.second;
						}
					}

					// 読み込みを通知
					NotifyArgument(input);

					// ログ出力
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
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
					std::lock_guard<std::recursive_mutex> lock(m_asyncController);

					// 接続済みを確認
					postscript << "IsConnected()";
					IsConnected();
					postscript.str("");

					do
					{
						// 書き込みクラスを取得
						auto write = GetWriteFirst();

						// 書き込みクラスを確認
						if (write == nullptr)
						{
							break;
						}

						// レジスタの値を更新
						for (const auto& entity : write->Entitys)
						{
							Plc::Register::CSpecify specify(entity.Category);
							Plc::Register::Setting::CSetContent setting;

							// 名称を更新
							setting.Name = GetName();

							// レジスタの種別を確定
							setting.Category = entity.Category;
							// レジスタの番号を確定
							setting.Index = entity.Index;
							// レジスタの値を確認
							if (entity.Value.type() == typeid(bool))
							{
								// ビット型 ⇒ レジスタのデータを確定
								setting.Values.emplace_back((short)Variant::Convert<bool>(entity.Value) ? 1 : 0);
							}
							else if (entity.Value.type() == typeid(__int16))
							{
								// 整数(2バイト)型 ⇒ レジスタのデータを確定
								setting.Values.emplace_back((short)Variant::Convert<__int16>(entity.Value));
							}
							else if (entity.Value.type() == typeid(__int32))
							{
								// 整数(4バイト)型 ⇒ レジスタのデータを確定
								setting.Values.emplace_back((short)(Variant::Convert<__int32>(entity.Value) >> 0 & 0xFFFF));
								setting.Values.emplace_back((short)(Variant::Convert<__int32>(entity.Value) >> 16 & 0xFFFF));
							}

							// レジスタを更新
							postscript << "Plc::Register::Set()";
							Plc::Register::Set(setting);
							postscript.str("");
						}

						// 先頭に登録してあるデータを削除
						PopWriteFirst();

						// ログ出力
						postscript << "Write()" << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					} while (false);
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