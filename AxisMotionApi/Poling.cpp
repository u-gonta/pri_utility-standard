#include "Poling.h"
#include "Common/Tool/Variant.h"

#include "WrapperMotionApi/Status.h"
#include "PlcMotionApi/Parameter.h"
#include "PlcMotionApi/ExecutionController.h"
#include "PlcMotionApi/ExecutionRegister.h"
#include "PlcMotionApi/ExecutionAxis.h"
#include "PlcMotionApi/ExecutionDevice.h"

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
					auto buffer = polingSetting.Connect.Other.GetValue<Plc::EnumParameter>(Plc::ParameterOpenController);

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
					postscript << "Plc::Controller::Initialize()";
					auto resultInitialize = Plc::Controller::Initialize(settingController);
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
						postscript << "Plc::Axis::Initialize()";
						auto result = Plc::Axis::Initialize(settingAxis);
						postscript.str("");

						// 軸の名称を更新
						status.Name = driver.second.Name;

						// 軸のハンドルを更新
						status.Handle = result.Axis.Handle;

						// 先頭アドレスを更新
						status.Address = settingAxis.Declare.Address;

						// 分解能を更新
						status.Resolution = settingAxis.Declare.Resolution;

						// 軸の状態を追加
						postscript << "Addition()";
						AdditionStatus(driver.first, status);
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
						postscript << "Plc::Controller::Destroy()";
						Plc::Controller::Destroy(settingController);
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
				//@@@Transfer::Output(Logging::Join(logging, postscript.str()));
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

					// 全軸の状態を取得
					postscript << "GetStatus()";
					auto statuses = GetStatus();
					postscript.str("");

					// 監視する番号を取得
					auto size = GetSurveillanceSize();
					std::map<size_t, bool> completes;

					// 監視を走査
					for (size_t index = 0; index < size; index++)
					{
						// 監視を完了に設定
						completes[index] = true;
					}

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

						// 入力レジスタの読み込み
						postscript << "Plc::Register::Get()";
						auto result = Plc::Register::Get(content);
						postscript.str("");

						// 入力レジスタの値を機器の状態を保持するクラスへ変換
						auto driver = ConvertInput(result);

						// レジスタの種別を更新
						content.Category = Plc::Register::ConstCategoryO;
						content.Category += Plc::Register::ConstTypeWord;

						// 出力レジスタの読み込み
						postscript << "Plc::Register::Get()";
						result = Plc::Register::Get(content);
						postscript.str("");

						// 出力レジスタの値を機器の状態を保持するクラスへ変換
						driver += ConvertOutput(result);

						// 名称を更新
						driver.Name = status.second.Name;

						// 位置の単位を変換
						driver.Coordinate[Status::Input::CoordinateNegativeLimit] *= status.second.Resolution;
						driver.Coordinate[Status::Input::CoordinatePositiveLimit] *= status.second.Resolution;
						driver.Coordinate[Status::Input::CoordinateCurrent] *= status.second.Resolution;

						// 機器の状態を保持するクラスを更新
						input.Drivers[status.first] = driver;

						// 監視を走査
						for (int index = 0; index < size; index++)
						{
							// 監視を取得
							auto surveillance = GetSurveillance(index);

							// 監視の軸を走査
							for (const auto& id : surveillance.Ids)
							{
								// 監視の状態を確認
								switch (surveillance.Status)
								{
								case CSurveillance::StatusFinish:
									// 終了
									if (driver.Motion[Status::Input::MotionCompleted] == false && driver.Motion[Status::Input::MotionAbnormal] == false)
									{
										// 異常終了と実行完了が共にOFF
										completes[index] = false;
									}
									break;
								}
							}
						}
					}

					std::vector<size_t> indexs;

					// 監視の状態を走査
					for (const auto& complete : completes)
					{
						// 監視の完了を確認
						if (complete.second)
						{
							// 監視を完了 ⇒ 監視を取得
							auto surveillance = GetSurveillance(complete.first);

							Execution::CFinish finish;

							// 監視の完了を通知
							WakeupFinish(finish, surveillance.Wakeup);

							// 監視の削除に登録
							indexs.emplace_back(complete.first);
						}
					}

					// 監視を削除
					EraseSurveillance(indexs);

					// 読み込みを通知
					NotifyArgument(input);

					// ログ出力
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					//@@@Transfer::Output(Logging::Join(logging, postscript.str()));
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
						std::vector<size_t> erases;

						// 書き込むクラス数を取得
						auto size = GetWriteSize();

						for (int index = 0; index < size; index++)
						{
							// 書き込むクラスを取得
							auto write = GetWrite(index);

							// 書き込みクラスを確認
							if (write == nullptr)
							{
								// 書き込むクラスがない ⇒ 次のクラスへ
								continue;
							}

							bool valid = true;

							// 軸の指令を判定
							if (write->Output.Command.type() == typeid(Status::Output::CServo))
							{
								// サーボを制御
								valid = Servo(*write);
							}
							else if (write->Output.Command.type() == typeid(Status::Output::CMoveOrigin))
							{
								// 原点復帰
								valid = MoveOrigin(*write);
							}
							else if (write->Output.Command.type() == typeid(Status::Output::CStartJog))
							{
								// ジョグを実行
								valid = StartJog(*write);
							}
							else if (write->Output.Command.type() == typeid(Status::Output::CStopJog))
							{
								// ジョグを停止
								valid = StopJog(*write);
							}

							// 実行の開始を確認
							if (valid)
							{
								// 削除するデータを追加
								erases.emplace_back(index);
							}
						}

						// 登録してあるデータを削除
						PopWrite(erases);

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
				postscript << "Plc::Axis::Destroy()";
				Plc::Axis::Destroy(setting);
				postscript.str("");

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// 軸の状態をクリア
				m_axisStatuses.clear();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸の状態を追加
			/// @param[in]		id		軸の識別子
			/// @param[in]		object	軸の状態クラス
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::AdditionStatus(int id, const CStatus& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "AdditionStatus()" << Logging::ConstSeparator;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// 軸の状態を追加
				postscript << "ID:" << id;
				postscript << Logging::ConstSeparator << "名称:" << object.Name;
				postscript << Logging::ConstSeparator << "ハンドル:" << Plc::FormatHandle(object.Handle);
				postscript << Logging::ConstSeparator << "アドレス:" << Text::FormatX(4, object.Address);
				postscript << Logging::ConstSeparator << "分解能:" << object.Resolution;
				m_axisStatuses[id] = object;
				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸の状態を取得
			/// @param[in]		id	軸の識別子
			/// @return			軸の状態クラス
			////////////////////////////////////////////////////////////////////////////////
			CStatus CWorker::GetStatus(int id)
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
			/// @brief			全軸の状態を取得
			/// @return			全軸の状態クラス
			////////////////////////////////////////////////////////////////////////////////
			std::map<int, CStatus> CWorker::GetStatus()
			{
				std::map<int, CStatus> ret;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// 軸の状態を取得
				ret = m_axisStatuses;

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸を監視する数を取得
			/// @return			軸を監視する数
			////////////////////////////////////////////////////////////////////////////////
			size_t CWorker::GetSurveillanceSize()
			{
				size_t ret = 0;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillance);

				// 軸を監視する数を取得
				ret = m_surveillances.size();

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸の監視を取得
			/// @param[in]		index	軸を監視する番号
			/// @return			軸を監視する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			CSurveillance CWorker::GetSurveillance(size_t index)
			{
				CSurveillance ret;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillance);

				// 軸を監視する数を確認
				if (index < 0 || m_surveillances.size() <= index)
				{
					// 軸の監視する番号が範囲外
					std::stringstream message;
					message << "軸を監視する番号が範囲外:" << index;
					throw std::exception(message.str().c_str());
				}

				// 軸を監視する設定クラスを取得
				ret = m_surveillances.at(index);

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸を監視を追加
			/// @param[in]		object		軸を監視する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::AdditionSurveillance(const CSurveillance& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "AdditionSurveillance()" << Logging::ConstSeparator;

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillance);

				// 軸の監視を追加
				for (const auto& id : object.Ids)
				{
					// 軸の状態を取得
					auto status = GetStatus(id);

					postscript << status.Name << Logging::ConstSeparator << "監視を開始";
					switch (object.Status)
					{
					case CSurveillance::StatusFinish:
						postscript << "(完了)";
						break;
					}
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				m_surveillances.emplace_back(object);
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸を監視を削除
			/// @param[in]		indexs		軸を監視する番号
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::EraseSurveillance(const std::vector<size_t>& indexs)
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "EraseSurveillance()" << Logging::ConstSeparator;

				std::vector<size_t> targets;

				// インデックス番号をコピー
				std::copy(indexs.begin(), indexs.end(), std::back_inserter(targets));

				// インデックス番号を降順でソート
				std::sort(targets.begin(), targets.end(), std::greater<size_t>());

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillance);

				// インデックス番号を走査
				for (const auto& index : targets)
				{
					// 書き込むデータのクラス数を確認
					if (m_surveillances.size() <= index)
					{
						// 書き込むデータの範囲外
						continue;
					}

					for (const auto& id : m_surveillances.at(index).Ids)
					{
						// 軸の状態を取得
						auto status = GetStatus(id);

						postscript << status.Name << Logging::ConstSeparator << "監視を終了";
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// データを削除
					m_surveillances.erase(m_surveillances.begin() + index);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			入力レジスタの値を機器の状態を保持するクラスへ変換
			/// @param[in]		object	レジスタの値クラス
			/// @return			機器の状態を保持するクラス
			////////////////////////////////////////////////////////////////////////////////
			Axis::Status::Input::CDriver CWorker::ConvertInput(const Plc::Register::Result::CContent& object)
			{
				Axis::Status::Input::CDriver ret;

				unsigned long warning = 0;
				unsigned long alarm = 0;
				unsigned long coordinate = 0;

				// レジスタの値を格納
				for (const auto& numeric : object.Words)
				{
					switch (numeric.first)
					{
					case 0x00:
						// 運転ステータス ⇒ 運転ステータスの信号を走査
						for (const auto& iterator : MotionApi::Status::RunningIterator())
						{
							// 信号を分解
							bool buffer = ((unsigned short)numeric.second >> iterator & 0x01 ? true : false);

							switch (iterator)
							{
							case MotionApi::Status::RunningServo:
								// 運転中(サーボON) ⇒ サーボON
								ret.Motion[Status::Input::MotionServo] = buffer;
								break;
							}
						}
						break;

					case 0x02:
						// ワーニング ⇒ 下位を更新
						warning = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x03:
						// ワーニング ⇒ 上位を更新
						warning |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// ワーニングの信号を走査
						for (const auto& iterator : MotionApi::Status::WarningIterator())
						{
							// ワーニングを更新
							ret.Warning[iterator] = (warning >> iterator & 0x01 ? true : false);
						}
						break;

					case 0x04:
						// アラーム ⇒ 下位を更新
						alarm = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x05:
						// アラーム ⇒ 上位を更新
						alarm |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// アラームの信号を走査
						for (const auto& iterator : MotionApi::Status::AlarmIterator())
						{
							// アラームを更新
							ret.Alarm[iterator] = (alarm >> iterator & 0x01 ? true : false);
						}
						break;

					case 0x09:
						// モーションコマンドステータス	⇒ コマンドステータスの信号を走査
						for (const auto& iterator : MotionApi::Status::CommandIterator())
						{
							// 信号を分解
							bool buffer = ((unsigned short)numeric.second >> iterator & 0x01 ? true : false);

							switch (iterator)
							{
							case MotionApi::Status::CommandRunning:
								// 実行中 ⇒ 実行中
								ret.Motion[Status::Input::MotionRunning] = buffer;
								break;

							case MotionApi::Status::CommandAbnormalFinish:
								// 異常終了状態 ⇒ 異常
								ret.Motion[Status::Input::MotionAbnormal] = buffer;
								break;

							case MotionApi::Status::CommandCompleted:
								// 実行完了 ⇒ 実行完了
								ret.Motion[Status::Input::MotionCompleted] = buffer;
								break;
							}
						}
						break;

					case 0x0C:
						// 位置ステータス ⇒ 位置ステータスの信号を走査
						for (const auto& iterator : MotionApi::Status::PositionIterator())
						{
							// 信号を分解
							bool buffer = ((unsigned short)numeric.second >> iterator & 0x01 ? true : false);

							switch (iterator)
							{
							case MotionApi::Status::PositionOriginCompleted:
								// 原点復帰完了 ⇒ 原点復帰完了
								ret.Motion[Status::Input::MotionOrigin] = buffer;
								break;
							}
						}
						break;

					case 0x16:
						// 機械座標系フィードバック位置(APOS) ⇒ 下位を更新
						coordinate = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x17:
						// 機械座標系フィードバック位置(APOS) ⇒ 上位を更新
						coordinate |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// 現在の座標を更新
						ret.Coordinate[Status::Input::CoordinateCurrent] = (double)(signed long)coordinate;
						break;
					}
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			出力レジスタの値を機器の状態を保持するクラスへ変換
			/// @param[in]		object	レジスタの値クラス
			/// @return			機器の状態を保持するクラス
			////////////////////////////////////////////////////////////////////////////////
			Axis::Status::Input::CDriver CWorker::ConvertOutput(const Plc::Register::Result::CContent& object)
			{
				Axis::Status::Input::CDriver ret;

				unsigned long negativeLimit = 0;
				unsigned long positiveLimit = 0;

				// レジスタの値を格納
				for (const auto& numeric : object.Words)
				{
					switch (numeric.first)
					{
					case 0x66:
						// 正方向ソフトリミット ⇒ 下位を更新
						positiveLimit = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x67:
						// 正方向ソフトリミット ⇒ 上位を更新
						positiveLimit |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// 正方向ソフトリミットを更新
						ret.Coordinate[Status::Input::CoordinatePositiveLimit] = (double)(signed long)positiveLimit;
						break;

					case 0x68:
						// 負方向ソフトリミット ⇒ 下位を更新
						negativeLimit = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x69:
						// 負方向ソフトリミット ⇒ 上位を更新
						negativeLimit |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// 負方向ソフトリミットを更新
						ret.Coordinate[Status::Input::CoordinateNegativeLimit] = (double)(signed long)negativeLimit;
						break;
					}
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			制御を停止
			/// @param[in]		object	制御を実行する設定のクラス
			/// @return			true:実行を開始
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::Suspend(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

				logging.Message << GetName() << "Suspend()" << Logging::ConstSeparator;

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

					// 制御の停止か確認
					if (object.Output.Command.type() != typeid(Status::Output::CSuspend))
					{
						// サーボの制御ではない
						postscript << "軸の制御クラスと不一致：" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// 状態の停止を指定するクラスに変換
					auto output = boost::any_cast<Status::Output::CSuspend>(object.Output.Command);

					Plc::Device::Setting::CSuspend setting;

					// 名称を更新
					setting.Name = GetName();

					// 軸を走査
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Suspend::CAxis axis;

						// 軸の状態を取得
						auto status = GetStatus(driver.first);

						// 軸のハンドルを更新
						axis.Handle = status.Handle;

						// 軸のレジスタ先頭アドレスを更新
						axis.Address = status.Address;

						// 軸を追加
						setting.Axises.emplace_back(axis);
					}

					// タイムアウト[ms]を取得
					auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterSuspendTimeout);

					// タイムアウト[ms]が有効か確認
					if (Variant::IsIntValue(buffer))
					{
						// タイムアウト[ms]を更新
						setting.Timeout = Variant::ConvertInt(buffer);
					}

					// 制御を停止
					postscript << "Plc::Device::Suspend()";
					Plc::Device::Suspend(setting);
					postscript.str("");

					// 処理の完了を起床
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					ret = true;
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

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			サーボを制御
			/// @param[in]		object	制御を実行する設定のクラス
			/// @return			true:実行を開始
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::Servo(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

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

					// サーボの状態を指定するクラスに変換
					auto output = boost::any_cast<Status::Output::CServo>(object.Output.Command);

					Plc::Device::Setting::CServo setting;

					// 名称を更新
					setting.Name = GetName();

					// 制御を更新
					setting.Mode = output.Mode;

					// 軸を走査
					for (const auto& driver : output.Drivers)
					{
						// 軸の状態を取得
						auto status = GetStatus(driver.first);

						// 軸のハンドルを追加
						setting.Handles.emplace_back(status.Handle);

						postscript << status.Name << Logging::ConstSeparator << "サーボ:";
						postscript << (setting.Mode == Signal::EnumStatus::StatusOn ? "ON" : "OFF");
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// タイムアウト[ms]を取得
					auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterServoTimeout);

					// タイムアウト[ms]が有効か確認
					if (Variant::IsIntValue(buffer))
					{
						// タイムアウト[ms]を更新
						setting.Timeout = Variant::ConvertInt(buffer);
					}

					// サーボを制御
					postscript << "Plc::Device::Servo()";
					Plc::Device::Servo(setting);
					postscript.str("");

					// 処理の完了を起床
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ログ出力
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					ret = true;
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

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			原点復帰を実行
			/// @param[in]		object	制御を実行する設定のクラス
			/// @return			true:実行を開始
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::MoveOrigin(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

				logging.Message << GetName() << "MoveOrigin()" << Logging::ConstSeparator;

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

					// 原点復帰の実行か確認
					if (object.Output.Command.type() != typeid(Status::Output::CMoveOrigin))
					{
						// 原点復帰の実行ではない
						postscript << "軸の制御クラスと不一致：" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// 原点復帰を指定するクラスに変換
					auto output = boost::any_cast<Status::Output::CMoveOrigin>(object.Output.Command);

					Plc::Device::Setting::CMoveOrigin setting;
					CSurveillance surveillance;

					// 名称を更新
					setting.Name = GetName();

					// 軸を走査
					for (auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Origin::CAxis axis;

						// 軸の原点復帰を行う設定クラスを取得
						auto buffer = driver.second.Other.GetValue<Plc::EnumParameter>(Plc::ParameterOriginAxis);

						// 軸の原点復帰を行う設定クラスが有効か確認
						if (Variant::IsValue<MotionApi::Device::Setting::Origin::CAxis>(buffer))
						{
							// 軸の原点復帰を行う設定クラスを更新
							axis = Variant::Convert<MotionApi::Device::Setting::Origin::CAxis>(buffer);
						}

						// 軸の状態を取得
						auto status = GetStatus(driver.first);

						// 軸のハンドルを更新
						axis.Handle = status.Handle;

						// 関節指定のデータをレジスタの先頭アドレスからの値に更新
						axis.Reflection(status.Address);

						// ポジションのタイプを確認
						if (axis.PositionType == MotionApi::Device::PositionIndirect)
						{
							// ポジションが関節指定 ⇒ 原点復帰最終走行距離のアドレスへ更新
							axis.PositionData = status.Address + 0x42;
						}

						// 軸の設定を追加
						setting.Axises.emplace_back(axis);

						// 監視する軸の識別子を追加
						surveillance.Ids.emplace_back(driver.first);

						postscript << status.Name << Logging::ConstSeparator << "原点復帰";
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// タイムアウト[ms]を取得
					auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterMoveOriginTimeout);

					// タイムアウト[ms]が有効か確認
					if (Variant::IsIntValue(buffer))
					{
						// タイムアウト[ms]を更新
						setting.Timeout = Variant::ConvertInt(buffer);
					}

					// 原点復帰を実行
					postscript << "Plc::Device::MoveOrigin()";
					Plc::Device::MoveOrigin(setting);
					postscript.str("");

					// 監視の種類を更新
					surveillance.Status = CSurveillance::StatusFinish;

					// 制御した結果の通知を呼び出す関数を更新
					surveillance.Wakeup = object.Wakeup;

					// 軸の監視を登録
					AdditionSurveillance(surveillance);

					ret = true;
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

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ジョグを開始
			/// @param[in]		object	制御を実行する設定のクラス
			/// @return			true:実行を開始
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::StartJog(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

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

					// ジョグの開始か確認
					if (object.Output.Command.type() != typeid(Status::Output::CStartJog))
					{
						// ジョグの開始ではない
						postscript << "軸の制御クラスと不一致：" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// ジョグの開始を指定するクラスに変換
					auto output = boost::any_cast<Status::Output::CStartJog>(object.Output.Command);

					Plc::Device::Setting::CStartJog setting;

					// 名称を更新
					setting.Name = GetName();

					// 軸を走査
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Jog::CStart axis;

						// 軸のジョグ実行を行う設定クラスを取得
						auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterStartJog);

						// 軸のジョグ実行を行う設定クラスが有効か確認
						if (Variant::IsValue<MotionApi::Device::Setting::Jog::CStart>(buffer))
						{
							// 軸のジョグ実行を行う設定クラスを更新
							axis = Variant::Convert<MotionApi::Device::Setting::Jog::CStart>(buffer);
						}

						// 軸のハンドルを更新
						axis.Handle = GetStatus(driver.first).Handle;

						// 軸の設定を追加
						setting.Axises.emplace_back(axis);
					}

					// ジョグを実行
					postscript << "Plc::Device::StartJog()";
					Plc::Device::StartJog(setting);
					postscript.str("");

					// 処理の完了を起床
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					ret = true;
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

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ジョグを停止
			/// @param[in]		object	制御を実行する設定のクラス
			/// @return			true:実行を開始
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::StopJog(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

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

					// ジョグの停止を指定するクラスに変換
					auto output = boost::any_cast<Status::Output::CStopJog>(object.Output.Command);

					Plc::Device::Setting::CStopJog setting;

					// 名称を更新
					setting.Name = GetName();

					// 軸を走査
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Jog::CStop axis;

						// 軸のジョグ停止を行う設定クラスを取得
						auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterStopJog);

						// 軸のジョグ停止を行う設定クラスが有効か確認
						if (Variant::IsValue<std::string>(buffer))
						{
							// 軸のジョグ停止を行う設定クラスを更新
							axis = Variant::Convert<MotionApi::Device::Setting::Jog::CStop>(buffer);
						}

						// 軸のハンドルを更新
						axis.Handle = GetStatus(driver.first).Handle;

						// 軸の設定を追加
						setting.Axises.emplace_back(axis);
					}

					// ジョグを停止
					postscript << "Plc::Device::StopJog()";
					Plc::Device::StopJog(setting);
					postscript.str("");

					// 処理の完了を起床
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					ret = true;
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

				return ret;
			}
		}
	}
}