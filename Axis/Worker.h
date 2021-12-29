#pragma once

#include "Common/Axis/Setting.h"
#include "Common/Axis/Status.h"
#include "Device/Worker.h"
#include "Poling.h"

namespace Standard
{
	namespace Axis
	{
		namespace Worker
		{
			//! 入力の変化を通知する関数の定義
			typedef void(OnChange)(const Status::CInput&);

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      軸の制御を行うテンプレートのクラス
			///				⇒ デバイスの制御を行うテンプレートのクラスから派生
			/// @param[in]	CDefault	デフォルトな設定クラスを指定
			/// @param[in]	CConnect	接続の設定クラスを指定
			////////////////////////////////////////////////////////////////////////////////
			template <typename CPoling>
			class CTemplate
				: virtual public Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>()
				{
					// 名称を更新
					SetName("Axis::");
					m_noticeChange.SetName("Axis::Notice::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					// 入力の変化を通知する関数の解除
					DetachFunctionInput();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			状態の通知を開始
				/// @detail			通知する関数を登録する前に呼び出す
				/// @param[in]		cycle	状態の通知を監視する間隔[ms]
				////////////////////////////////////////////////////////////////////////////////
				void StartNotice(int cycle = 60000)
				{
					// 状態の通知を行う周期を設定
					m_noticeChange.SetCycle(cycle);

					// 状態の通知を開始
					m_noticeChange.Start();

					// 状態の通知で初期化を要求
					m_noticeChange.RequestInitialize();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			状態の通知を停止
				/// @detail			通知する関数を解除後に呼び出す
				////////////////////////////////////////////////////////////////////////////////
				void StopNotice()
				{
					// 状態の通知で破棄を要求
					m_noticeChange.RequestDestroy();

					// 状態の通知を停止
					m_noticeChange.Stop();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			入力の変化を通知する関数の登録
				/// @detail			入力の変化時に呼び出されたい関数を指定する
				/// @param[in]		object	呼び出されたい関数
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionInput(std::function<void(const Status::CInput&)> object)
				{
					// 状態の通知を開始
					StartNotice();

					// 入力の変化を通知する関数の登録
					m_noticeChange.AttachFunctionCalled(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			入力の変化を通知する関数の解除
				/// @detail			入力の変化時に呼び出される関数を解除する
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionInput()
				{
					// 入力の変化を通知する関数の解除
					m_noticeChange.DetachFunctionCalled();

					// 状態の通知を停止
					StopNotice();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			軸の制御を要求
				/// @param[in]		object	軸の状態(出力)を指定するクラス
				////////////////////////////////////////////////////////////////////////////////
				void Command(const Status::COutput& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisWrite;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "Command()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 制御が可能か確認
					IsControllable();

					try
					{
						Surveillance::CSetting setting;

						// 軸の状態(出力)を指定するクラスを更新
						setting.Output = object;

						// 軸を制御を登録
						postscript << "RequestWrite()";
						m_poling.RequestWrite(setting);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を通知＆発砲
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			軸の制御を待機
				/// @param[in]		object	軸の状態(出力)を指定するクラス
				////////////////////////////////////////////////////////////////////////////////
				void CommandPending(const Status::COutput& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisWrite;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "CommandPending()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 制御が可能か確認
					IsControllable();

					try
					{
						// トリガーを待機するクラス
						Signal::Observer::CTemplate<Surveillance::CFinish> observer;

						// トリガーを通知をする関数を登録
						postscript << "Subject::RegistrationFunction()";
						std::shared_ptr<std::function<Surveillance::OnWakeup>> onWakeup = std::make_shared<std::function<Surveillance::OnWakeup>>(std::bind(&Signal::Observer::CTemplate<Surveillance::CFinish>::OnCalled, &observer, std::placeholders::_1));

						// トリガーを待機するタイムアウトを更新
						observer.Setting.Timeout = object.Timeout;

						Surveillance::CSetting setting;

						// 軸の状態(出力)を指定するクラスを更新
						setting.Output = object;

						// トリガーを通知する関数を更新
						setting.Wakeup = onWakeup;

						// 軸を制御を登録
						postscript << "RequestWrite()";
						m_poling.RequestWrite(setting);
						postscript.str("");

						// トリガーを待つ
						postscript << "Observer::Wait()";
						observer.Wait();
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// 制御の結果を取得
						postscript << "Observer::GetArgument()";
						Surveillance::CFinish result = observer.GetArgument();
						postscript.str("");

						// 中断を確認
						if (result.Suspension)
						{
							// 中断
							postscript << "Observer::Trigger()" << Logging::ConstSeparator << "中断";

							// 例外を発砲
							throw std::exception();
						}
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を通知＆発砲
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			軸のサーボを制御して待機
				/// @param[in]		object	軸のサーボ状態(出力)を指定するクラス
				////////////////////////////////////////////////////////////////////////////////
				void Servo(const Status::Output::CServo& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "Servo()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					Status::COutput output;

					// 軸の状態(出力)を指定するクラスを更新

					output.Command = object;

					// 軸の制御を待機
					CommandPending(output);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			軸のジョグを実行して待機
				/// @param[in]		object	軸のジョグ実行を指定するクラス
				////////////////////////////////////////////////////////////////////////////////
				void StartJog(const Status::Output::CStartJog& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "StartJog()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					Status::COutput output;

					// 軸の状態(出力)を指定するクラスを更新
					output.Command = object;

					// 軸の制御を待機
					CommandPending(output);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			軸のジョグを停止して待機
				/// @param[in]		object	軸のジョグ停止を指定するクラス
				////////////////////////////////////////////////////////////////////////////////
				void StopJog(const Status::Output::CStopJog& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "StopJog()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					Status::COutput output;

					// 軸の状態(出力)を指定するクラスを更新
					output.Command = object;

					// 軸の制御を待機
					CommandPending(output);
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化済みの例外を発砲
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionInitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionInitialized()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeAxisInitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄済みの例外を発砲
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionDiscarded() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionDiscarded()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeAxisDiscarded, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			未初期化の例外を発砲
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionUninitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionUninitialized()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeAxisUninitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化
				/// @detail			初期化のタイミングで呼び出される
				///					※ポーリングの例外を通知する関数を登録、周期の更新など行う
				////////////////////////////////////////////////////////////////////////////////
				void InitializeDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					try
					{
						// 例外を受信する関数の登録
						postscript << "Poling::AttachFunctionException()";
						m_poling.AttachFunctionException(std::bind(&CTemplate::OnException, this, std::placeholders::_1));
						postscript.str("");

						// 入力を受信する関数の登録
						postscript << "Poling::AttachFunctionCalled()";
						m_poling.AttachFunctionCalled(std::bind(&CTemplate::OnInput, this, std::placeholders::_1));
						postscript.str("");

						// デフォルトの設定クラスを取得
						Setting::CDefault defaultSetting = GetDefaultSetting();

						// ポーリングの周期を更新
						postscript << "Poling::SetCycle(" << defaultSetting.Cycle << ")";
						m_poling.SetCycle(defaultSetting.Cycle);
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// 軸をポーリングする為の設定を取得
						postscript << "Poling::GetSetting()";
						Poling::CSetting polingSetting = m_poling.GetSetting();
						postscript.str("");

						// デフォルトの設定を更新
						polingSetting.Default = defaultSetting;

						// ポーリングのリトライ回数
						postscript << "Poling::Setting::Retry(" << polingSetting.Default.Retry << ")";
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// ポーリングのタイムアウト
						postscript << "Poling::Setting::Timeout(" << polingSetting.Default.Timeout << ")";
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// 軸をポーリングする為の設定を更新
						postscript << "Poling::SetSetting()";
						m_poling.SetSetting(polingSetting);
						postscript.str("");

						// 軸のポーリングを開始
						postscript << "Poling::Start()";
						m_poling.Start();
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
				///					※ポーリングの例外を通知する関数の解除など行う
				////////////////////////////////////////////////////////////////////////////////
				void DestroyDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					try
					{
						// 破棄
						postscript << "Poling::Destroy()";
						m_poling.Destroy();
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						postscript.str("");

						// 入力を受信する関数の解除
						postscript << "Poling::DetachFunctionCalled()";
						m_poling.DetachFunctionCalled();
						postscript.str("");

						// 例外を受信する関数の解除
						postscript << "Poling::DetachFunctionException()";
						m_poling.DetachFunctionException();
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
				///	@detail			接続のタイミングで呼び出される
				////////////////////////////////////////////////////////////////////////////////
				void ConnectDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisConnect;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					try
					{
						// 破棄
						postscript << "Poling::Destroy()";
						m_poling.Destroy();
						postscript.str("");

						// 軸をポーリングする為の設定を取得
						postscript << "Poling::GetSetting()";
						Poling::CSetting polingSetting = m_poling.GetSetting();
						postscript.str("");

						// 接続の設定クラスを取得
						Setting::CConnect connectSetting = GetConnectSetting();

						// 接続の設定を更新
						polingSetting.Connect = connectSetting;

						// 軸をポーリングする為の設定を更新
						postscript << "Poling::SetSetting()";
						m_poling.SetSetting(polingSetting);
						postscript.str("");

						// 初期化
						postscript << "Poling::Initialize()";
						m_poling.Initialize();
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
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
				///	@detail			切断のタイミングで呼び出される
				////////////////////////////////////////////////////////////////////////////////
				void DisconnectDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisDisconnect;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					try
					{
						// 破棄
						postscript << "Poling::Destroy()";
						m_poling.Destroy();
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を通知＆発砲
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外を受け取る関数
				/// @param[in]		object	例外のクラス
				////////////////////////////////////////////////////////////////////////////////
				void OnException(const Exception::CObject& object)
				{
					// 例外を通知
					NotifyException(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			信号を更新
				/// @param[in]		logging		ログのクラス
				/// @param[in]		postscript	メッセージ
				/// @param[in]		before		更新前の信号状態
				/// @param[in]		after		更新後の信号状態
				/// @param[in]		target		信号の番号
				/// @return			true:更新あり / false:更新なし
				////////////////////////////////////////////////////////////////////////////////
				bool UpdateInput(Logging::CObject& logging, std::stringstream& postscript, std::map<unsigned short, bool>& before, bool after, unsigned short target)
				{
					bool ret = false;

					do
					{
						// 信号が登録済みか確認
						if (0 < before.count(target))
						{
							// データあり ⇒ 格納済みの値と比較
							if (before[target] == after)
							{
								// 値の変化なし ⇒ 処理を抜ける
								break;
							}

							// 変化あり
							postscript << (before[target] ? "ON" : "OFF") << " → ";
						}

						// 新規or変化あり
						postscript << (after ? "ON" : "OFF");
						before[target] = after;

						ret = true;
					} while (false);

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			入力のイベント
				/// @param[in]		object	IOの状態(入力)のクラス
				////////////////////////////////////////////////////////////////////////////////
				void OnInput(const Status::CInput& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "OnInput()" << Logging::ConstSeparator;

					Status::CInput changedInput;
					bool notify = false;

					// 軸を走査
					for (const auto& driver : object.Drivers)
					{
						// 運転ステータスを走査
						for (const auto& running : driver.second.Running)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "運転ステータス:" << Text::Format(2, running.first);
							postscript << Logging::ConstSeparator;

							// 排他制御
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// 信号を更新
							if (UpdateInput(logging, postscript, m_input.Drivers[driver.first].Running, running.second, running.first) == false)
							{
								// 値の変化なし ⇒ 次の信号へ
								postscript.str("");
								continue;
							}

							// 新規or変化あり
							changedInput.Drivers[driver.first].Running[running.first] = running.second;
							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// 値あり
							notify = true;
						}

						// ワーニングを走査
						for (const auto& warning : driver.second.Warning)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "ワーニング:" << Text::Format(2, warning.first);
							postscript << Logging::ConstSeparator;

							// 排他制御
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// 信号を更新
							if (UpdateInput(logging, postscript, m_input.Drivers[driver.first].Warning, warning.second, warning.first) == false)
							{
								// 値の変化なし ⇒ 次の信号へ
								postscript.str("");
								continue;
							}

							// 新規or変化あり
							changedInput.Drivers[driver.first].Warning[warning.first] = warning.second;
							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// 値あり
							notify = true;
						}

						// アラームを走査
						for (const auto& alarm : driver.second.Alarm)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "アラーム:" << Text::Format(2, alarm.first);
							postscript << Logging::ConstSeparator;

							// 排他制御
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// 信号を更新
							if (UpdateInput(logging, postscript, m_input.Drivers[driver.first].Alarm, alarm.second, alarm.first) == false)
							{
								// 値の変化なし ⇒ 次の信号へ
								postscript.str("");
								continue;
							}

							// 新規or変化あり
							changedInput.Drivers[driver.first].Alarm[alarm.first] = alarm.second;
							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// 値あり
							notify = true;
						}

						// 位置を走査
						for (const auto& position : driver.second.Position)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "位置:" << Text::Format(2, position.first);
							postscript << Logging::ConstSeparator;

							// 排他制御
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// 信号を更新
							if (UpdateInput(logging, postscript, m_input.Drivers[driver.first].Position, position.second, position.first) == false)
							{
								// 値の変化なし ⇒ 次の信号へ
								postscript.str("");
								continue;
							}

							// 新規or変化あり
							changedInput.Drivers[driver.first].Position[position.first] = position.second;
							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// 値あり
							notify = true;
						}

						// 座標を走査
						for (const auto& coordinate : driver.second.Coordinate)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "座標:" << Text::Format(2, coordinate.first);
							postscript << Logging::ConstSeparator;

							// 排他制御
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// 座標が登録済みか確認
							if (0 < m_input.Drivers[driver.first].Coordinate.count(coordinate.first))
							{
								// データあり ⇒ 格納済みの値と比較
								if (::fabs(coordinate.second - m_input.Drivers[driver.first].Coordinate[coordinate.first]) <= 0)
								{
									// 値の変化なし ⇒ 次の位置へ
									postscript.str("");
									continue;
								}

								// 変化あり
								postscript << m_input.Drivers[driver.first].Position[coordinate.first] << " → ";
							}

							// 新規or変化あり
							postscript << coordinate.second;
							m_input.Drivers[driver.first].Coordinate[coordinate.first] = coordinate.second;
							changedInput.Drivers[driver.first].Coordinate[coordinate.first] = coordinate.second;
							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// 値あり
							notify = true;
						}
					}

					// 通知するデータを確認
					if (notify)
					{
						// 通知するデータがある ⇒ 入力値の変化を通知
						m_noticeChange.RequestNotice(changedInput);
					}
				}

			private:
				//! 排他制御クラス(データ用)
				std::recursive_mutex m_asyncData;

				//! 軸の状態(入力)のクラス
				Status::CInput m_input;

				//! 軸の監視、軸の制御、状態を通知するクラス
				CPoling m_poling;

				//! 入力の変化を通知するクラス
				Notice::Worker::CTemplate<OnChange, Status::CInput> m_noticeChange;
			};
		}
	}
}