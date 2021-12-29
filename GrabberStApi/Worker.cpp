#include "Worker.h"
#include "Common/Utility/StopWatch.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			登録するコールバック関数
		/// @param[in]		node	ノード
		/// @param[in]		me	自身のクラス
		////////////////////////////////////////////////////////////////////////////////
		void __stdcall OnNodeCallbackFunction(GenApi::INode* node, CWorker* me)
		{
			if (me)
			{
				// コールバックから呼び出し
				me->ReceiveNode(node);
			}
		};

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
		/// @brief			デバイスロストを確認
		/// @return			デバイスロストの状態
		////////////////////////////////////////////////////////////////////////////////
		bool CWorker::IsDeviceLost()
		{
			bool ret = false;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "IsDeviceLost()" << Logging::ConstSeparator;

			try
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				do
				{
					// インターフェースを確認
					if (m_stDevice.IsValid() == false)
					{
						// インターフェース未確定 ⇒ デバイスロストなし
						break;
					}

					// データストリームを確認
					if (m_stDataStream.IsValid() == false)
					{
						// データストリーム未確定 ⇒ デバイスロストなし
						break;
					}

					// デバイスロストの状態を取得
					ret = m_stDevice->IsDeviceLost();
				} while (false);
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知＆発砲
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			コールバックから呼び出し関数
		/// @detail			コールバック発生時に呼び出してもらう
		/// @param[in]		node	ノード
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::ReceiveNode(GenApi::INode* node)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDeviceLost;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReceiveNode()" << Logging::ConstSeparator;

			try
			{
				// デバイスロストの状態を確認
				if (IsDeviceLost())
				{
					// デバイスロスト発生
					postscript << "DeviceLost";

					// 例外の出力
					Exception::CObject exception = OutputException(errorCode, logging, postscript.str());

					// 例外を通知
					NotifyException(exception);
				}
			}
			catch (const std::exception& e)
			{
				// 例外の処理 ⇒ 例外を通知
				OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			デバイスを確認
		/// @param[in]		setting	接続の設定クラス
		/// @param[in]		nodeMap	インターフェースのノード
		/// @param[in]		index	デバイスの番号
		////////////////////////////////////////////////////////////////////////////////
		bool CWorker::IsDevice(const Setting::CConnect& setting, const GenApi::INodeMap& nodeMap, const uint32_t index)
		{
			bool ret = false;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << GetName() << "IsDevice(" << index << ")" << Logging::ConstSeparator;

			do
			{
				// カメラの識別子を確認
				if (setting.Identifier.empty() == false)
				{
					// 識別子あり ⇒ IPアドレスで選択
					GenApi::CIntegerPtr deviceIpAddress(nodeMap.GetNode("GevDeviceIPAddress"));

					// IPアドレスが有効か確認
					if (GenApi::IsAvailable(deviceIpAddress))
					{
						std::string ipAddress = deviceIpAddress->ToString().c_str();

						// IPアドレスが一致しているか確認
						if (setting.Identifier.compare(ipAddress) != 0)
						{
							// 不一致
							break;
						}
					}
				}
				else
				{
					// その他 ⇒ 先頭に見つかったデバイスを選択
				}

				// デバイスが有効
				ret = true;
			} while (false);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			パラメータをデフォルトに戻す
		/// @param[in]		nodeMap	インターフェースのリモートノード
		/// @param[in]		object	デフォルトな設定クラス
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DefaultParameter(const GenApi::INodeMap& nodeMap, const Setting::CDefault& setting)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << GetName() << "DefaultParameter()" << Logging::ConstSeparator;

			try
			{
				CParameter parameter;

				// ユーザー設定をデフォルトに変更
				parameter.WriteText(nodeMap, "UserSetSelector", "Default");

				// ユーザ設定をロード
				parameter.Execute(nodeMap, "UserSetLoad");

				// 取り込み回数を1回
				parameter.WriteText(nodeMap, "AcquisitionMode", "SingleFrame");

				// フレーム開始トリガの選択
				parameter.Selector(nodeMap, "TriggerSelector", "FrameStart");

				// トリガを無効
				parameter.WriteText(nodeMap, "TriggerMode", "Off");

				//// トリガをソフトウェアトリガ
				//parameter.WriteText(nodeMap, "TriggerSource", "Software");

				// 取り込みの条件クラスを取得
				Condition::CCapture captureCondition = GetCaptureCondition();

				// センサの最大幅を取得
				captureCondition.WidthMaximum = (int)parameter.ReadInteger(nodeMap, "WidthMax");

				int64_t imageWidth = captureCondition.WidthMaximum;

				// 画像の幅が設定されている and センサの最大幅より小さいか確認
				if (0 < setting.ImageWidth && setting.ImageWidth < captureCondition.WidthMaximum)
				{
					// 画像の幅を変更
					imageWidth = setting.ImageWidth;
				}

				// 画像の幅を変更
				imageWidth = parameter.WriteInteger(nodeMap, "Width", imageWidth);

				// オフセットXを計算
				int64_t offsetX = (int64_t)std::round((double)(captureCondition.WidthMaximum - imageWidth) / 2);

				// オフセットXを変更
				parameter.WriteInteger(nodeMap, "OffsetX", offsetX);

				// センサの最大高さを取得
				captureCondition.HeightMaximum = (int)parameter.ReadInteger(nodeMap, "HeightMax");

				int64_t imageHeight = captureCondition.HeightMaximum;

				// 画像の高さが設定されている and センサの最大高さより小さいか確認
				if (0 < setting.ImageHeight && setting.ImageHeight < captureCondition.HeightMaximum)
				{
					// 画像の高さを変更
					imageHeight = setting.ImageHeight;
				}

				// 画像の高さを変更
				imageHeight = parameter.WriteInteger(nodeMap, "Height", imageHeight);

				// オフセットYを計算
				int64_t offsetY = (int64_t)std::round((double)(captureCondition.HeightMaximum - imageHeight) / 2);

				// オフセットYを変更
				parameter.WriteInteger(nodeMap, "OffsetY", offsetY);

				// アナログゲインの選択
				parameter.Selector(nodeMap, "GainSelector", "AnalogAll");

				// アナログゲインの範囲を読み込み
				captureCondition.AnalogGain = parameter.ReadRange(nodeMap, "Gain");

				// デジタルゲインの選択
				parameter.Selector(nodeMap, "GainSelector", "DigitalAll");

				// デジタルゲインの範囲を読み込み
				captureCondition.DigitalGain = parameter.ReadRange(nodeMap, "Gain");

				// 露光時間の範囲を読み込み
				captureCondition.ExposureTime = parameter.ReadRange(nodeMap, "ExposureTime");

				// 取り込みの条件を更新
				SetCaptureCondition(captureCondition);
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			Heartbeatを更新
		/// @param[in]		nodeMap	インターフェースのリモートノード
		/// @param[in]		timeout	タイムアウト[us]
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::UpdateHeartbeat(const GenApi::INodeMap& nodeMap, double timeout)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << GetName() << "UpdateHeartbeat()" << Logging::ConstSeparator;

			try
			{
				// Heartbeatタイムアウト(標準)を取得
				GenApi::CValuePtr deviceHeartbeatTimeout(nodeMap.GetNode("DeviceLinkHeartbeatTimeout"));

				// Heartbeatタイムアウト(標準)の有無を確認
				if (deviceHeartbeatTimeout == nullptr)
				{
					// Heartbeatタイムアウト(標準)なし ⇒ Heartbeatタイムアウト(非推奨)の有無を確認
					deviceHeartbeatTimeout = nodeMap.GetNode("GevHeartbeatTimeout");

					// Heartbeatタイムアウト(非推奨)の有無を確認
					if (deviceHeartbeatTimeout.IsValid() == false)
					{
						// Heartbeatタイムアウト(非推奨)なし ⇒ 例外を発砲
						throw std::exception("Heartbeatなし");
					}

					// タイムアウトの単位を変換 [us] → [ms]
					timeout *= 1000;
				}

				std::stringstream buffer;

				// タイムアウトを文字列に変換
				buffer << timeout;

				// Heartbeatタイムアウトを更新
				postscript << "GenApi::HeartbeatTimeout(" << buffer.str() << ")";
				deviceHeartbeatTimeout->FromString(buffer.str().c_str());
				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			DeviceLostのコールバックを登録
		/// @param[in]		nodeMap	インターフェースのローカルノード
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::RegisterDeviceLost(const GenApi::INodeMap& nodeMap)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << GetName() << "RegisterDeviceLost()" << Logging::ConstSeparator;

			try
			{
				// DeviceLostを取得
				postscript << "GenApi::GetNode(EventDeviceLost)";
				GenApi::CNodePtr nodeCallback(nodeMap.GetNode("EventDeviceLost"));
				postscript.str("");

				// DeviceLostを確認
				if (nodeCallback)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

					// コールバック関数を登録
					postscript << "GenApi::RegisterCallback()";
					StApi::RegisterCallback(nodeCallback, &OnNodeCallbackFunction, this);
					postscript.str("");
				}
				else
				{
					// DeviceLostなし ⇒ 例外を発砲
					throw std::exception("DeviceLostなし");
				}
				
				// EventSelectorを取得
				postscript << "GenApi::GetNode(EventSelector)";
				GenApi::CEnumerationPtr enumerationEventSelector(nodeMap.GetNode("EventSelector"));
				postscript.str("");

				// DeviceLostを取得
				postscript << "GenApi::GetEntryByName(DeviceLost)";
				GenApi::CEnumEntryPtr enumerationEntryEventSelectorEntry(enumerationEventSelector->GetEntryByName("DeviceLost"));
				postscript.str("");

				// DeviceLostを選択
				postscript << "GenApi::SetIntValue(DeviceLost)";
				enumerationEventSelector->SetIntValue(enumerationEntryEventSelectorEntry->GetValue());
				postscript.str("");

				// EventNotificationを取得
				postscript << "GenApi::GetNode(EventNotification)";
				GenApi::CEnumerationPtr enumerationEventNotification(nodeMap.GetNode("EventNotification"));
				postscript.str("");

				// Onを取得
				postscript << "GenApi::GetEntryByName(On)";
				GenApi::CEnumEntryPtr enumerationEntryEventNotificationEntry(enumerationEventNotification->GetEntryByName("On"));
				postscript.str("");

				// Onを選択
				postscript << "GenApi::SetIntValue(On)";
				enumerationEventNotification->SetIntValue(enumerationEntryEventNotificationEntry->GetValue());
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// 例外の処理
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// 例外を発砲
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			初期化
		/// @detail			初期化のタイミングで呼び出される
		///					※ライブラリの初期化などを行う
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
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ライブラリを初期化
				postscript << "StApi::StApiInitialize()";
				StApi::StApiInitialize();
				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				StApi::EStSystemVendor_t systemVendor = StApi::EStSystemVendor_t::StSystemVendor_Default;
				StApi::EStInterfaceType_t interfaceType = StApi::EStInterfaceType_t::StInterfaceType_All;

				// システムモジュールを開く
				postscript << "StApi::CreateIStSystem()";
				m_stSystem.Reset(StApi::CreateIStSystem(systemVendor, interfaceType));
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
		///					※ライブラリの破棄などを行う
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
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// システムを開放
				postscript << "StApi::System::Reset()";
				m_stSystem.Reset();
				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// ライブラリを破棄
				postscript << "StApi::StApiTerminate()";
				StApi::StApiTerminate();
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
		///					※データストリームの開始などを行う
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
				// 接続設定クラスを取得
				Setting::CConnect connectSetting = GetConnectSetting();

				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// インターフェース数を取得
				uint32_t interfaceCount = m_stSystem->GetInterfaceCount();
				postscript << "Interface:" << interfaceCount;

				// インターフェース数を確認
				if (interfaceCount <= 0)
				{
					errorCode = Exception::CodeGrabberInterface;

					// 例外を発砲
					throw std::exception("インターフェースなし");
				}
				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// インターフェースの確認
				bool checkInterface = true;

				// インターフェースと接続されているカメラ数分を走査 ※インターフェースの確認が有効の場合
				for (uint32_t interfaceIndex = 0; interfaceIndex < interfaceCount && checkInterface; interfaceIndex++)
				{
					// インターフェースを取得
					postscript << "StApi::GetIStInterface(" << interfaceIndex << ")";
					StApi::IStInterface *stInterface = m_stSystem->GetIStInterface(interfaceIndex);
					postscript.str("");

					// 接続されているデバイス(カメラ)を検出し、デバイス(カメラ)リストを更新
					postscript << "StApi::GetDeviceCount()";
					stInterface->UpdateDeviceList();
					postscript.str("");

					// デバイス数を取得
					postscript << "StApi::GetDeviceCount(" << interfaceIndex << ")";
					const uint32_t deviceCount = stInterface->GetDeviceCount();
					postscript << Logging::ConstSeparator << "Device:" << deviceCount;

					// デバイス数を確認
					if (deviceCount <= 0)
					{
						// デバイスなし ⇒ 次のインターフェースへ
						continue;
					}
					// ログ出力
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// インターフェースポインターのノードを取得
					postscript << "GenApi::GetINodeMap()";
					GenApi::CNodeMapPtr nodeMap(stInterface->GetIStPort()->GetINodeMap());
					postscript.str("");

					// デバイスを選択
					postscript << "GenApi::DeviceSelector()";
					GenApi::CIntegerPtr deviceSelector(nodeMap->GetNode("DeviceSelector"));
					postscript.str("");

					// デバイス数を取得
					postscript << "StApi::GetMax()";
					const int64_t deviceMax = deviceSelector->GetMax();
					postscript << Logging::ConstSeparator << "Device:" << deviceMax;
					// ログ出力
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// デバイス数分を走査
					for (uint32_t indexDevice = 0; indexDevice <= deviceMax; indexDevice++)
					{
						// デバイスの選択
						deviceSelector->SetValue(indexDevice);

						// デバイスを確認
						if (IsDevice(connectSetting, *nodeMap, indexDevice) == false)
						{
							// 不一致 ⇒ 次のデバイスへ
							continue;
						}

						// インターフェースを更新
						postscript << "StApi::Interface::CreateIStDevice(" << indexDevice << ")";
						m_stDevice.Reset(stInterface->CreateIStDevice((size_t)indexDevice));
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// データストリームを更新
						postscript << "StApi::Interface::CreateIStDataStream()";
						m_stDataStream.Reset(m_stDevice->CreateIStDataStream());
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// デフォルトの設定クラスを取得
						Setting::CDefault defaultSetting = GetDefaultSetting();

						// リモートのノードを取得
						GenApi::CNodeMapPtr remoteNode(m_stDevice->GetRemoteIStPort()->GetINodeMap());

						// パラメータをデフォルトに戻す
						postscript << "DefaultParameter()";
						DefaultParameter(*remoteNode, defaultSetting);
						postscript.str("");

						// Heartbeatを更新
						postscript << "StApi::Interface::UpdateHeartbeat()";
						UpdateHeartbeat(*remoteNode, defaultSetting.HeartbeatTimeout);
						postscript.str("");

						// ローカルのノードを取得
						GenApi::CNodeMapPtr localNode(m_stDevice->GetLocalIStPort()->GetINodeMap());

						// DeviceLostのコールバックを登録
						postscript << "StApi::Interface::RegisterHeartbeat()";
						RegisterDeviceLost(*localNode);
						postscript.str("");

						// イベントのスレッドを開始
						postscript << "StApi::Interface::StartEventAcquisitionThread()";
						m_stDevice->StartEventAcquisitionThread();
						postscript.str("");

						// データストリームの開始
						postscript << "StApi::DataStream::StartAcquisition()";
						m_stDataStream->StartAcquisition();
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// インターフェースの確認を解除
						checkInterface = false;
						break;
					}
				}

				// インターフェース and データストリームを確認
				if (m_stDevice.IsValid() == false || m_stDataStream.IsValid() == false)
				{
					errorCode = Exception::CodeGrabberDevice;

					// 例外を発砲
					throw std::exception("該当するカメラなし");
				}
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
		///					※データストリームの開放などを行う
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
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// デバイスロスト
				bool deviceLost = false;

				// インターフェースを確認
				if (m_stDevice.IsValid())
				{
					// デバイスロストを取得
					deviceLost = m_stDevice->IsDeviceLost();

					// デバイスロストを確認
					if (deviceLost == false)
					{
						// デバイスロストなし ⇒ インターフェースの取り込みを停止
						postscript << "StApi::Interface::AcquisitionStop()";
						//@@@m_stDevice->AcquisitionStop();
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// イベントのスレッドを停止
					postscript << "StApi::Interface::StopEventAcquisitionThread()";
					m_stDevice->StopEventAcquisitionThread();
					postscript.str("");
				}

				// データストリームを確認
				if (m_stDataStream.IsValid())
				{
					// デバイスロスト and 撮像中を確認
					if (deviceLost == false && m_stDataStream->IsGrabbing())
					{
						// デバイスロストなし ⇒ データストリームの取り込みを停止
						postscript << "StApi::DataStream::StopAcquisition()";
						//@@@m_stDataStream->StopAcquisition();
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}
				}

				// データストリームを解放
				postscript << "StApi::DataStream::Reset()";
				m_stDataStream.Reset();
				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// インターフェースを開放
				postscript << "StApi::Interface::Reset()";
				m_stDevice.Reset();
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

			do
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// データストリームを確認
				if (m_stDataStream.IsValid() == false)
				{
					// データストリーム未確定
					break;
				}

				// インターフェースを確認
				if (m_stDevice.IsValid() == false)
				{
					// インターフェース未確定
					break;
				}

				ret = true;
			} while (false);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			接続済みを確認
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::IsConnected()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDevice;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "IsConnected()" << Logging::ConstSeparator;

			try
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// データストリームを確認
				if (m_stDataStream.IsValid() == false)
				{
					errorCode = Exception::CodeGrabberDataStream;

					// 例外を発砲
					throw std::exception("データストリーム未確定");
				}
				postscript.str("");

				// インターフェースを確認
				if (m_stDevice.IsValid() == false)
				{
					errorCode = Exception::CodeGrabberInterface;

					// 例外を発砲
					throw std::exception("インターフェース未確定");
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
		/// @brief			アナログゲインを読み込み
		/// @detail			アナログゲイン値を読み込むタイミングで呼び出される
		///					★継承した関数
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
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// 接続済みを確認
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// リモートのノードを取得
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// アナログゲインの選択
				postscript << "Parameter::Selector(GainSelector, AnalogAll)";
				parameter.Selector(*nodeMap, "GainSelector", "AnalogAll");
				postscript.str("");

				// アナログゲインを読み込み
				postscript << "Parameter::ReadDouble(Gain)";
				ret = parameter.ReadDouble(*nodeMap, "Gain");
				postscript << Logging::ConstSeparator << "値:" << ret;
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
		/// @detail			アナログゲイン値を書き込むタイミングで呼び出される
		///					★継承した関数
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
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// 接続済みを確認
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// リモートのノードを取得
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// アナログゲインの選択
				postscript << "Parameter::Selector(GainSelector, AnalogAll)";
				parameter.Selector(*nodeMap, "GainSelector", "AnalogAll");
				postscript.str("");

				// アナログゲインを書き込み
				postscript << "Parameter::WriteDouble()";
				parameter.WriteDouble(*nodeMap, "Gain", value);
				postscript << Logging::ConstSeparator << "値:" << value;
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
		/// @detail			デジタルゲイン値を読み込むタイミングで呼び出される
		///					★継承した関数
		/// @return			デジタルゲイン値
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadDigitalGainDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadDigitalGainDerived()" << Logging::ConstSeparator;

			try
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// 接続済みを確認
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// リモートのノードを取得
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// デジタルゲインの選択
				postscript << "Parameter::Selector(GainSelector, DigitalAll)";
				parameter.Selector(*nodeMap, "GainSelector", "DigitalAll");
				postscript.str("");

				// デジタルゲインを読み込み
				postscript << "Parameter::ReadDouble(Gain)";
				ret = parameter.ReadDouble(*nodeMap, "Gain");
				postscript << Logging::ConstSeparator << "値:" << ret;
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
		/// @detail			デジタルゲイン値を書き込むタイミングで呼び出される
		///					★継承した関数
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
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// 接続済みを確認
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// リモートのノードを取得
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// デジタルゲインの選択
				postscript << "Parameter::Selector(GainSelector, DigitalAll)";
				parameter.Selector(*nodeMap, "GainSelector", "DigitalAll");
				postscript.str("");

				// デジタルゲインを書き込み
				postscript << "Parameter::WriteDouble()";
				parameter.WriteDouble(*nodeMap, "Gain", value);
				postscript << Logging::ConstSeparator << "値:" << value;
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
		/// @detail			露光時間を読み込むタイミングで呼び出される
		///					★継承した関数
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
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// 接続済みを確認
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// リモートのノードを取得
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// 露光時間を読み込み
				postscript << "Parameter::ReadDouble(ExposureTime)";
				ret = parameter.ReadDouble(*nodeMap, "ExposureTime");
				postscript << Logging::ConstSeparator << "値:" << ret;
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
		/// @detail			露光時間を書き込むタイミングで呼び出される
		///					★継承した関数
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
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// 接続済みを確認
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// リモートのノードを取得
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// デジタルゲインの選択
				postscript << "Parameter::Selector(ExposureTimeSelector, Common)";
				parameter.Selector(*nodeMap, "ExposureTimeSelector", "Common");
				postscript.str("");

				// 露光時間を書き込み
				postscript << "Parameter::WriteDouble()";
				parameter.WriteDouble(*nodeMap, "ExposureTime", value);
				postscript << Logging::ConstSeparator << "値:" << value;
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
		/// @detail			画像を取り込むタイミングで呼び出される
		///					★継承した関数
		/// @return			画像のクラス
		////////////////////////////////////////////////////////////////////////////////
		Image::CObject CWorker::CaptureDerived()
		{
			Image::CObject ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberCapture;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "CaptureDerived()" << Logging::ConstSeparator;

			try
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// 接続済みを確認
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				// 取り込み中を確認
				if (m_stDataStream->IsGrabbing() == false)
				{
					errorCode = Exception::CodeGrabberCaptureNotStart;

					// 例外を発砲
					throw std::exception("データストリームの取り込み未実施");
				}

				// デフォルトな設定クラスを取得
				Setting::CDefault defaultSetting = GetDefaultSetting();

				// 取り込みを要求
				postscript << "StApi::Interface::AcquisitionStart()";
				m_stDevice->AcquisitionStart();
				postscript.str("");

				// 取り込み待ち
				postscript << "StApi::DataStream::RetrieveBuffer(" << defaultSetting.CaptureTimeout << ")";
				StApi::CIStStreamBufferPtr stStreamBuffer(m_stDataStream->RetrieveBuffer((uint32_t)defaultSetting.CaptureTimeout));

				// 画像の確認
				if (stStreamBuffer.IsValid() == false)
				{
					errorCode = Exception::CodeGrabberCaptureTimeout;

					// 例外を発砲
					throw std::exception("タイムアウト");
				}
				postscript.str("");

				// 画像の情報を取得
				postscript << "StApi::DataStreamBuffer::IsImagePresent()";
				if (stStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent() == false)
				{
					errorCode = Exception::CodeGrabberCaptureNotImage;

					// 例外を発砲
					throw std::exception("画像データなし");
				}
				postscript.str("");

				// 画像データを取得
				postscript << "StApi::DataStreamBuffer::GetIStImage()";
				StApi::IStImage *stImage = stStreamBuffer->GetIStImage();

				StApi::EStPixelFormatNamingConvention_t format = stImage->GetImagePixelFormat();
				int depth = 0;
				int width = (int)stImage->GetImageWidth();
				int height = (int)stImage->GetImageHeight();

				// 画像フォーマット
				postscript << Logging::ConstSeparator << format;

				switch (format)
				{
				case StApi::EStPixelFormatNamingConvention_t::StPFNC_Mono8:
					// モノクロ8ビット
					depth = 8;
					break;

				default:
					// その他 ⇒ 未対応
					errorCode = Exception::CodeGrabberCaptureNotFormat;

					// 例外を発砲
					throw std::exception("未対応");
				}
				postscript.str("");

				// 画像データを生成
				postscript << "Image::Update()";
				ret.Update(depth, height, width, (const unsigned char*)stImage->GetImageBuffer());
				postscript.str("");

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

			return ret;
		}
	}
}