#pragma once

#include "Common/Io/Setting.h"
#include "Common/Io/Status.h"
#include "Common/Tool/Variant.h"
#include "Device/Worker.h"
#include "Signal/Result.h"
#include "Signal/Subject.h"
#include "Signal/Observer.h"
#include "Poling.h"

namespace Standard
{
	namespace Io
	{
		namespace Worker
		{
			namespace Trigger
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CInput
				/// @brief      入力を監視した結果のクラス
				///				⇒ 状態を監視した結果を保持するテンプレートのクラスから継承
				////////////////////////////////////////////////////////////////////////////////
				class CInput
					: virtual public Signal::Result::CTemplate
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コンストラクタ
					////////////////////////////////////////////////////////////////////////////////
					CInput() : Signal::Result::CTemplate()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コピー代入演算子
					/// @return			自クラス
					////////////////////////////////////////////////////////////////////////////////
					CInput& operator = (const CInput& object) noexcept
					{
						// 更新
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			デストラクタ
					////////////////////////////////////////////////////////////////////////////////
					~CInput()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			自クラスを更新
					/// @param[in]		object	自クラス
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CInput& object)
					{
						this->Setting = object.Setting;
					}

				public:
					//! トリガーの設定クラス
					Setting::CTrigger Setting;
				};

				//! トリガを通知する関数の定義
				typedef void(OnWakeup)(const Trigger::CInput&);

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CSetting
				/// @brief      監視の設定クラス
				///				⇒ IOの入力が変化するのを待つ設定クラスから継承
				////////////////////////////////////////////////////////////////////////////////
				class CSetting
					: virtual public Setting::CTrigger
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コンストラクタ
					////////////////////////////////////////////////////////////////////////////////
					CSetting() : Setting::CTrigger()
					{
						Wakeup.reset();
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コピー代入演算子
					/// @return			トリガーの設定クラス
					////////////////////////////////////////////////////////////////////////////////
					CSetting& operator = (const Setting::CTrigger& object) noexcept
					{
						// 既定の関数
						Io::Setting::CTrigger::Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コピー代入演算子
					/// @return			自クラス
					////////////////////////////////////////////////////////////////////////////////
					CSetting& operator = (const CSetting& object) noexcept
					{
						// 更新
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			デストラクタ
					////////////////////////////////////////////////////////////////////////////////
					~CSetting()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			自クラスを更新
					/// @param[in]		object	自クラス
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CSetting& object)
					{
						// 既定の関数
						Io::Setting::CTrigger::Update(object);

						this->Wakeup = object.Wakeup;
					}

				public:
					//! トリガーの通知を呼び出す関数
					std::weak_ptr<std::function<OnWakeup>> Wakeup;
				};
			}

			//! 入力の変化を通知する関数の定義
			typedef void(OnChange)(const Status::CInput&);

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      Ioの制御を行うテンプレートのクラス
			///				⇒ デバイスの制御を行うテンプレートのクラスから派生
			/// @param[in]	CDefault	デフォルトの設定クラスを指定
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
					SetName("Io::");
					m_noticeChange.SetName("Io::Notice::");
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
				/// @brief			入力のトリガを待機
				/// @param[in]		object	IOの入力が変化するのを待つ設定クラス
				////////////////////////////////////////////////////////////////////////////////
				void TriggerInputPending(const Setting::CInputPending& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoRead;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "TriggerInputPending()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 制御が可能か確認
					IsControllable();

					try
					{
						// トリガーを待機するクラス
						Signal::Observer::CTemplate<Trigger::CInput> observer;

						// トリガーを通知をする関数を登録
						postscript << "Subject::RegistrationFunction()";
						std::shared_ptr<std::function<Trigger::OnWakeup>> onWakeup = std::make_shared<std::function<Trigger::OnWakeup>>(std::bind(&Signal::Observer::CTemplate<Trigger::CInput>::OnCalled, &observer, std::placeholders::_1));

						// トリガーを待機するタイムアウトを更新
						observer.Setting.Timeout = object.Timeout;

						Trigger::CSetting setting;

						// トリガーの設定をコピー
						setting = object.Trigger;

						// トリガーを通知する関数を更新
						setting.Wakeup = onWakeup;
						postscript.str("");

						// 入力を監視してトリガの通知をする関数を登録
						postscript << "RegistrationTrigger(アドレス:" << Specify::Format(setting.Address) << ",ビットの方向:" << setting.Direction << ")";
						RegistrationTrigger(setting);
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// 動作要求を起床
						postscript << "WakeupRequestAction()";
						m_poling.WakeupRequestAction();
						postscript.str("");

						// トリガーを待つ
						postscript << "Observer::Wait()";
						observer.Wait();
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// 入力の結果を取得
						postscript << "Observer::GetArgument()";
						Trigger::CInput result = observer.GetArgument();
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
				/// @brief			入力を読み込み
				/// @param[in]		object	入力の設定クラス
				/// @return			IOの状態(入力)クラス
				////////////////////////////////////////////////////////////////////////////////
				Status::CInput ReadInput(const Setting::CInput& object) throw(...)
				{
					Status::CInput ret;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoRead;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "ReadInput()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 制御が可能か確認
					IsControllable();

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncData);

						// アドレスを走査
						for (const auto& address : object.Addresses)
						{
							// アドレスを確認
							if (m_input.Values.count(address) <= 0)
							{
								// アドレスなし ⇒ 次のアドレスへ
								continue;
							}

							// 値を確定
							ret.Values[address] = m_input.Values.at(address);
						}
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を通知＆発砲
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			出力を書き込み
				/// @param[in]		object	IOの状態(出力)クラス
				////////////////////////////////////////////////////////////////////////////////
				void WriteOutput(const Status::COutput& object) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WriteOutput()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 制御が可能か確認
					IsControllable();

					// 出力を更新
					SetOutput(object);

					// 出力を書き込み
					m_poling.RequestWrite(object);
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化済みの例外を発砲
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionInitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionInitialized()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeIoInitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄済みの例外を発砲
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionDiscarded() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionDiscarded()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeIoDiscarded, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			未初期化の例外を発砲
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionUninitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionUninitialized()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeIoUninitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化
				/// @detail			初期化のタイミングで呼び出される
				///					※ポーリングの例外を通知する関数を登録、周期の更新など行う
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void InitializeDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoInitialize;
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

						// Ioをポーリングする為の設定を取得
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

						// Ioをポーリングする為の設定を更新
						postscript << "Poling::SetSetting()";
						m_poling.SetSetting(polingSetting);
						postscript.str("");

						// Ioのポーリングを開始
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
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void DestroyDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoDestroy;
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
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void ConnectDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoConnect;
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

						// Ioをポーリングする為の設定を取得
						postscript << "Poling::GetSetting()";
						Poling::CSetting polingSetting = m_poling.GetSetting();
						postscript.str("");

						// 接続の設定クラスを取得
						Setting::CConnect connectSetting = GetConnectSetting();

						// 接続の設定を更新
						polingSetting.Connect = connectSetting;

						// ポーリングする機器の識別子
						postscript << "Poling::Setting::Identifier(" << polingSetting.Connect.Identifier << ")";
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// ポーリングで読み込む範囲を確認
						for (const auto& addressRange : polingSetting.Connect.AddressRanges)
						{
							// 読み込む範囲をログ出力
							postscript << "Poling::Setting::AddressRange(";
							postscript << Specify::Format(addressRange.Start);
							postscript << "～ x";
							postscript << addressRange.Size;
							// ログ出力
							postscript << ")";
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
						}

						// Ioをポーリングする為の設定を更新
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
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void DisconnectDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoDisconnect;
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
				/// @brief			トリガーの発砲
				/// @param[in]		object	アドレス指定クラス
				////////////////////////////////////////////////////////////////////////////////
				void WakeupTrigger(const Status::CEntity& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WakeupTrigger()" << Logging::ConstSeparator;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncTrigger);

					do
					{
						// ビットを確認
						if (m_triggers.count(object) <= 0)
						{
							// 登録なし
							break;
						}

						// ビットの方向
						Status::EnumDirection direction = Status::DirectionRise;

						// ビットを監視する状態を確認
						if (m_triggers.at(object).count(Status::DirectionRise))
						{
							// ビットの立ち上がりを監視 ⇒ ビットが立ち上がっているか確認
							if (Variant::Convert<bool>(object.Value) == false)
							{
								// ビットがOFF
								break;
							}

							// ビットの方向を確定
							direction = Status::DirectionRise;
						}
						else if (m_triggers.at(object).count(Status::DirectionFall))
						{
							// ビットの立下りを監視 ⇒ ビットが立ち下がっているか確認
							if (Variant::Convert<bool>(object.Value))
							{
								// ビットがON
								break;
							}

							// ビットの方向を確定
							direction = Status::DirectionFall;
						}
						else
						{
							// ビットの情報なし
							break;
						}

						Trigger::CInput result;

						// アドレスを更新
						result.Setting.Address = object;
						// ビットの方向を更新
						result.Setting.Direction = direction;
						// 中断フラグを解除
						result.Suspension = false;

						// 登録を走査
						postscript << "アドレス:" << Specify::Format(object) << Logging::ConstSeparator << "ビット:" << Variant::FormatValue(object.Value) << Logging::ConstSeparator << "トリガを通知";
						for (auto iterator = m_triggers.at(object).at(direction).begin(); iterator != m_triggers.at(object).at(direction).end(); iterator++)
						{
							Signal::Subject::CTemplate<Trigger::CInput> subject;

							// 関数を登録
							subject.RegistrationFunction(*iterator);

							try
							{
								// 関数を通知
								subject.CallingFunction(result);
							}
							catch (const std::exception& e)
							{
								// エラーメッセージを統合
								postscript << Logging::ConstSeparator << e.what();
							}
						}

						// ビットの方向を削除
						m_triggers.at(object).clear();

						// アドレスごとに登録してある関数の数を確認
						if (m_triggers.at(object).size() <= 0)
						{
							// 登録なし ⇒ アドレスごとの登録を削除
							m_triggers.erase(object);
						}

						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					} while (false);
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
					std::vector<Status::CEntity> triggers;
					bool notify = false;

					// アドレスを走査
					for (const auto& value : object.Values)
					{
						postscript << "アドレス:" << Specify::Format(value.first);
						postscript << Logging::ConstSeparator << Variant::FormatType(value.second) << ":";

						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncData);

						// アドレスが登録済みか確認
						if (0 < m_input.Values.count(value.first))
						{
							// データあり ⇒ 格納済みの値と比較
							if (Variant::Equal(m_input.Values.at(value.first), value.second))
							{
								// 値の変化なし ⇒ 次のアドレスへ
								postscript.str("");
								continue;
							}

							// 変化あり
							postscript << Variant::FormatValue(m_input.Values.at(value.first)) << " → ";
						}

						// 新規or変化あり
						postscript << Variant::FormatValue(value.second);
						m_input.Values[value.first] = value.second;
						changedInput.Values[value.first] = value.second;
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// 型を確認
						if (value.second.type() == typeid(bool))
						{
							// ビット型 ⇒ トリガーを確定
							triggers.emplace_back(Status::CEntity(value.first, value.second));
						}

						// 値あり
						notify = true;
					}

					// トリガーを発砲
					for (const auto& trigger : triggers)
					{
						// トリガーの発砲
						WakeupTrigger(trigger);
					}

					// 通知するデータを確認
					if (notify)
					{
						// 通知するデータがある ⇒ 入力値の変化を通知
						m_noticeChange.RequestNotice(changedInput);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			出力を更新
				/// @param[in]		object	IOの状態(出力)クラス
				////////////////////////////////////////////////////////////////////////////////
				void SetOutput(const Status::COutput& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "SetOutput()" << Logging::ConstSeparator;

					// データを走査
					for (const auto& entity : object.Entitys)
					{
						postscript << "アドレス:" << Specify::Format(entity);
						postscript << Logging::ConstSeparator << Variant::FormatType(entity.Value) << ":";

						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncData);

						// アドレスのデータを取得
						auto element = std::find_if(m_output.Entitys.begin(), m_output.Entitys.end(), [&](const auto& element) { return Variant::Equal(element, entity); });

						// アドレスが登録済みか確認
						if (element != m_output.Entitys.end())
						{
							// データあり
							postscript << Variant::FormatValue(element->Value) << " → ";

							// データを更新
							element->Value = entity.Value;
						}
						else
						{
							// データなし ⇒ データを追加
							m_output.Entitys.emplace_back(entity);

							// ソート
							std::sort(m_output.Entitys.begin(), m_output.Entitys.end(), [](auto const& first, auto const& second)
							{
								return first < second;
							});
						}

						// 変化あり
						postscript << Variant::FormatValue(entity.Value);
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			入力のトリガを通知する関数を登録
				/// @param[in]		object	監視の設定クラス
				////////////////////////////////////////////////////////////////////////////////
				void RegistrationTrigger(const Trigger::CSetting& object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncTrigger);

					// アドレスごとに登録してある関数の数を確認
					if (m_triggers.count(object.Address) <= 0)
					{
						// 登録なし ⇒ アドレスを登録
						m_triggers[object.Address];
					}

					// ビットの方向の登録を確認
					if (m_triggers.at(object.Address).count(object.Direction) <= 0)
					{
						// 登録なし ⇒ ビットの方向を登録
						m_triggers.at(object.Address)[object.Direction];
					}

					// トリガを通知する関数を登録
					m_triggers.at(object.Address).at(object.Direction).emplace_back(object.Wakeup);
				}

			private:
				//! 排他制御クラス(データ用)
				std::recursive_mutex m_asyncData;

				//! IOの状態(入力)のクラス
				Status::CInput m_input;

				//! IOの状態(出力)のクラス
				Status::COutput m_output;

				//! IOの監視、IOの制御、状態を通知するクラス
				CPoling m_poling;

				//! 入力の変化を通知するクラス
				Notice::Worker::CTemplate<OnChange, Status::CInput> m_noticeChange;

				//! 排他制御クラス(入力のトリガを通知する関数用)
				std::recursive_mutex m_asyncTrigger;

				//! 入力のトリガを通知する関数
				std::map<Specify::CAddress, std::map<Status::EnumDirection, std::vector<std::weak_ptr<std::function<Trigger::OnWakeup>>>>> m_triggers;
			};
		}
	}
}

