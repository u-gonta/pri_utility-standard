#include "Poling.h"
#include "Common/Tool/Variant.h"

#include "WrapperMotionApi/Status.h"
#include "PlcMotionApi/Parameter.h"
#include "PlcMotionApi/Controller.h"

namespace Standard
{
	namespace Axis
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
				Exception::EnumCode errorCode = Exception::CodeAxisConnect;
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
				Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CInitialize trigger;

				logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

				try
				{
					Poling::CSetting polingSetting = GetSetting();
					Plc::Controller::Setting::CInitialize settingController;

					// 名称を更新
					settingController.Name = GetName();

					// コントローラを開く設定を取得
					postscript << "OpenController";
					boost::any buffer = polingSetting.Connect.Other.GetValue<Plc::EnumParameter>(Plc::ParameterOpenController);

					// コントローラを開く設定が有効か確認
					if (Variant::IsValue<std::string>(buffer))
					{
						// コントローラを開く設定を更新
						settingController.Open.Input(Variant::Convert<std::string>(buffer));
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
						settingController.ApiTimeout.Input(Variant::Convert<std::string>(buffer));
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
					}

					// コントローラを初期化
					postscript << "Plc::InitializeController()";
					auto resultInitialize = Plc::InitializeController(settingController);
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncController);

					// コントローラの情報を更新
					m_controller = resultInitialize.Controller;

					// 軸を破棄
					postscript << "Clear()";
					Clear();
					postscript.str("");

					// 軸を初期化
					for (auto& driver : polingSetting.Connect.Drivers)
					{
						postscript << "識別No:" << driver.first;
						postscript << Logging::ConstSeparator << "名称:" << driver.second.Name;

						// 軸のハンドルを作成する設定を取得
						buffer = driver.second.Other.GetValue<Plc::EnumParameter>(Plc::ParameterDeclareAxis);

						// 軸のハンドルを作成する設定が有効か確認
						if (Variant::IsValue<MotionApi::Axis::Setting::CDeclare>(buffer) == false)
						{
							// 軸のハンドルを作成する設定が無効
							throw std::exception("設定なし");
						}
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						Plc::Axis::Setting::CInitialize settingAxis;
						CStatus status;

						// 名称を更新
						settingAxis.Name = GetName();

						// 軸のハンドルを作成するクラスに変換
						postscript << "Variant::Convert()";
						settingAxis.Declare = Variant::Convert<MotionApi::Axis::Setting::CDeclare>(buffer);
						postscript.str("");

						// 軸を初期化
						postscript << "Plc::InitializeAxis()";
						auto result = Plc::InitializeAxis(settingAxis);
						postscript.str("");

						// 軸の名称を更新
						status.Name = driver.second.Name;

						// 軸のハンドルを更新
						status.Handle = result.Axis.Handle;

						// 先頭アドレスを更新
						status.Address = settingAxis.Declare.Address;

						// 分解能を更新
						status.Resolution = settingAxis.Declare.Resolution;

						// 軸を追加
						postscript << "Addition()";
						Addition(driver.first, status);
						postscript.str("");
					}

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
				Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
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

						Plc::Axis::Setting::CDestroy settingAxis;

						// 名称を更新
						settingAxis.Name = GetName();

						// 軸を解放
						postscript << "Clear()";
						Clear();
						postscript.str("");

						Plc::Controller::Setting::CDestroy settingController;

						// 名称を更新
						settingController.Name = GetName();

						// ハンドルを更新
						settingController.Close.Handle = m_controller.Handle;

						// コントローラを破棄
						postscript << "Plc::DestroyController()";
						Plc::DestroyController(settingController);
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
			///	@detail			リトライの判定を派生先で記述する
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
			/// @brief			リトライの遅延時間を取得
			///	@detail			リトライの遅延時間を取得する
			///					★継承した関数
			/// @return			リトライの遅延時間[ms]
			////////////////////////////////////////////////////////////////////////////////
			int CWorker::GetDelayRetry()
			{
				int ret = 0;

				// リトライの遅延時間
				ret = 1000;

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
				Exception::EnumCode errorCode = Exception::CodeAxisRead;
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

					Axis::Status::CInput input;

					// 全軸を取得
					postscript << "Get()";
					auto statuses = Get();
					postscript.str("");

					// 全軸を走査
					for (const auto& status : statuses)
					{
						Plc::Register::Setting::CGetContent content;

						// 名称を更新
						content.Name = GetName();

						// レジスタの種別を確定
						content.Category = Plc::Register::ConstCategoryI;
						content.Category += Plc::Register::ConstTypeWord;
						// レジスタの番号を確定
						content.Index = status.second.Address;
						// レジスタの範囲を確定
						content.Size = 0x80;

						// レジスタの読み込み
						postscript << "Plc::GetRegister()";
						auto result = Plc::GetRegister(content);
						postscript.str("");

						Plc::Register::CSpecify specify(content.Category);
						Axis::Status::Input::CDriver driver;
						unsigned long warning = 0;
						unsigned long alarm = 0;
						signed long coordinate = 0;

						// 名称を更新
						driver.Name = status.second.Name;

						// レジスタの値を格納
						for (const auto& numeric : result.Words)
						{
							Io::Specify::CAddress address;

							address.Category = specify.GetAddress();
							address.Index = content.Index;

							// アドレスの変換を確定
							address.Hex = true;
							address.Margin = specify.GetMargin();

							// アドレスを確定
							address.Index += numeric.first;

							switch (numeric.first)
							{
							case 0:
								// 運転ステータス ⇒ 運転ステータスの信号を走査
								for (const auto& iterator : MotionApi::Status::RunningIterator())
								{
									// 運転ステータスを更新
									driver.Running[iterator] = (numeric.second >> iterator & 0x01 ? true : false);
								}
								break;

							case 2:
								// ワーニング ⇒ 下位を更新
								warning = numeric.second & 0xFF;
								break;

							case 3:
								// ワーニング ⇒ 上位を更新
								warning |= ((unsigned long)numeric.second << 16 & 0xFFFF0000);

								// ワーニングの信号を走査
								for (const auto& iterator : MotionApi::Status::WarningIterator())
								{
									// ワーニングを更新
									driver.Warning[iterator] = (numeric.second >> iterator & 0x01 ? true : false);
								}
								break;

							case 4:
								// アラーム ⇒ 下位を更新
								alarm = numeric.second;
								break;

							case 5:
								// アラーム ⇒ 上位を更新
								alarm |= ((unsigned long)numeric.second << 16 & 0xFFFF0000);

								// アラームの信号を走査
								for (const auto& iterator : MotionApi::Status::AlarmIterator())
								{
									// アラームを更新
									driver.Alarm[iterator] = (numeric.second >> iterator & 0x01 ? true : false);
								}
								break;

							case 12:
								// 位置 ⇒ 位置の信号を走査
								for (const auto& iterator : MotionApi::Status::PositionIterator())
								{
									// 位置を更新
									driver.Position[iterator] = (numeric.second >> iterator & 0x01 ? true : false);
								}
								break;

							case 16:
								// 機械座標系フィードバック位置(APOS) ⇒ 下位を更新
								coordinate = (signed long)numeric.second;
								break;

							case 17:
								// 機械座標系フィードバック位置(APOS) ⇒ 上位を更新
								coordinate |= (signed long)numeric.second << 16 & 0xFFFF0000;

								// 位置の単位を変換
								driver.Coordinate[Status::Input::CoordinateCurrent] = (double)coordinate * status.second.Resolution;
								break;
							}
						}

						// 機器の状態を保持するクラスを更新
						input.Drivers[status.first] = driver;
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
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
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

						// 軸の指令を判定
						if (write->Output.Command.type() == typeid(Status::Output::CServo))
						{
							// サーボを制御
							Servo(*write);
						}
						else if (write->Output.Command.type() == typeid(Status::Output::CStartJog))
						{
							// ジョグを実行
							StartJog(*write);
						}
						else if (write->Output.Command.type() == typeid(Status::Output::CStopJog))
						{
							// ジョグを停止
							StopJog(*write);
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

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸を開放
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::Clear()
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "Clear()" << Logging::ConstSeparator;

				Plc::Axis::Setting::CDestroy setting;

				// 名称を更新
				setting.Name = GetName();

				// 軸を破棄
				postscript << "Plc::DestroyAxis()";
				Plc::DestroyAxis(setting);
				postscript.str("");

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// 軸の状態をクリア
				m_axisStatuses.clear();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸を追加
			/// @param[in]		id		軸の識別子
			/// @param[in]		status	軸の状態クラス
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::Addition(int id, const CStatus& status)
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "Addition()" << Logging::ConstSeparator;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// 軸の状態を追加
				postscript << "ID:" << id;
				postscript << Logging::ConstSeparator << "名称:" << status.Name;
				postscript << Logging::ConstSeparator << "ハンドル:" << Plc::FormatHandle(status.Handle);
				postscript << Logging::ConstSeparator << "アドレス:" << Text::FormatX(4, status.Address);
				postscript << Logging::ConstSeparator << "分解能:" << status.Resolution;
				m_axisStatuses[id] = status;
				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸を取得
			/// @param[in]		id	軸の識別子
			/// @return			軸の状態クラス
			////////////////////////////////////////////////////////////////////////////////
			CStatus CWorker::Get(int id)
			{
				CStatus ret;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// 軸の状態を確認
				if (m_axisStatuses.count(id) <= 0)
				{
					// 軸の状態なし
					std::stringstream message;
					message << "軸の識別子が範囲外:" << id;
					throw std::exception(message.str().c_str());
				}

				// 軸の状態を取得
				ret = m_axisStatuses.at(id);

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			全軸を取得
			/// @return			全軸の状態クラス
			////////////////////////////////////////////////////////////////////////////////
			std::map<int, CStatus> CWorker::Get()
			{
				std::map<int, CStatus> ret;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// 軸の状態を取得
				ret = m_axisStatuses;

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			サーボを制御
			/// @param[in]		object	制御を監視する設定のクラス
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::Servo(const Surveillance::CSetting& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Surveillance::CFinish finish;

				logging.Message << GetName() << "Servo()" << Logging::ConstSeparator;

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

					// サーボの制御か確認
					if (object.Output.Command.type() != typeid(Status::Output::CServo))
					{
						// サーボの制御ではない
						postscript << "軸の制御クラスと不一致：" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// 軸のサーボ状態(出力)を指定するクラスに変換
					auto output = boost::any_cast<Status::Output::CServo>(object.Output.Command);

					Plc::Device::Setting::CServoControl setting;

					// 名称を更新
					setting.Name = GetName();

					// 軸を走査
					for (const auto& driver : output.Drivers)
					{
						// 軸のハンドルを追加 ⇒ 軸を取得
						setting.Device.Handles.emplace_back(Get(driver.first).Handle);
					}

					// 制御を更新
					setting.Status.Mode = output.Mode;

					// タイムアウト[ms]を取得
					auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterServoTimeout);

					// タイムアウト[ms]が有効か確認
					if (Variant::IsIntValue(buffer))
					{
						// タイムアウト[ms]を更新
						setting.Status.Timeout = Variant::ConvertInt(buffer);
					}

					// サーボを制御
					postscript << "Plc::AxisServo()";
					Plc::AxisServo(setting);
					postscript.str("");

					// 処理の完了を起床
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 処理の完了をエラー
					finish.Suspension = true;
					finish.Message = e.what();
					// 処理の完了を起床
					WakeupFinish(finish, object.Wakeup);

					// 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ジョグを実行
			/// @param[in]		object	制御を監視する設定のクラス
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::StartJog(const Surveillance::CSetting& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Surveillance::CFinish finish;

				logging.Message << GetName() << "StartJog()" << Logging::ConstSeparator;

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

					// ジョグの実行か確認
					if (object.Output.Command.type() != typeid(Status::Output::CStartJog))
					{
						// ジョグの実行ではない
						postscript << "軸の制御クラスと不一致：" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// 軸のジョグ実行を指定するクラスに変換
					auto output = boost::any_cast<Status::Output::CStartJog>(object.Output.Command);

					Plc::Device::Setting::CStartJog setting;

					// 名称を更新
					setting.Name = GetName();

					// 軸を走査
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Motion::Jog::CStart jog;

						// 軸のジョグ実行を行う設定クラスを取得
						auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterStartJog);

						// 軸のジョグ実行を行う設定クラスが有効か確認
						if (Variant::IsValue<MotionApi::Device::Setting::Motion::Jog::CStart>(buffer))
						{
							// 軸のジョグ実行を行う設定クラスを更新
							jog = Variant::Convert<MotionApi::Device::Setting::Motion::Jog::CStart>(buffer);
						}

						// 軸のハンドルを更新
						jog.Handle = Get(driver.first).Handle;

						// 軸の設定を追加
						setting.Device.Motions.emplace_back(jog);
					}

					// ジョグを実行
					postscript << "Plc::AxisStartJog()";
					Plc::AxisStartJog(setting);
					postscript.str("");

					// 処理の完了を起床
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 処理の完了をエラー
					finish.Suspension = true;
					finish.Message = e.what();
					// 処理の完了を起床
					WakeupFinish(finish, object.Wakeup);

					// 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ジョグを停止
			/// @param[in]		object	制御を監視する設定のクラス
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::StopJog(const Surveillance::CSetting& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Surveillance::CFinish finish;

				logging.Message << GetName() << "StopJog()" << Logging::ConstSeparator;

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

					// ジョグの停止か確認
					if (object.Output.Command.type() != typeid(Status::Output::CStopJog))
					{
						// ジョグの停止ではない
						postscript << "軸の制御クラスと不一致：" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// 軸のジョグ停止を指定するクラスに変換
					auto output = boost::any_cast<Status::Output::CStopJog>(object.Output.Command);

					Plc::Device::Setting::CStopJog setting;

					// 名称を更新
					setting.Name = GetName();

					// 軸を走査
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Motion::Jog::CStop jog;

						// 軸のジョグ停止を行う設定クラスを取得
						auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterStopJog);

						// 軸のジョグ停止を行う設定クラスが有効か確認
						if (Variant::IsValue<std::string>(buffer))
						{
							// 軸のジョグ停止を行う設定クラスを更新
							jog.Input(Variant::Convert<std::string>(buffer));
						}

						// 軸のハンドルを更新
						jog.Handle = Get(driver.first).Handle;

						// 軸の設定を追加
						setting.Device.Motions.emplace_back(jog);
					}

					// ジョグを停止
					postscript << "Plc::AxisStopJog()";
					Plc::AxisStopJog(setting);
					postscript.str("");

					// 処理の完了を起床
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 処理の完了をエラー
					finish.Suspension = true;
					finish.Message = e.what();
					// 処理の完了を起床
					WakeupFinish(finish, object.Wakeup);

					// 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}
		}
	}
}