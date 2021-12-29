#pragma once

#include <mutex>

#include "Common/Tool/Logging.h"
#include "Exception/Convert.h"
#include "Transfer/Transfer.h"

#pragma comment(lib, "Transfer.lib")

namespace Standard
{
	namespace Thread
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      スレッドの制御を行うテンプレートのクラス
			////////////////////////////////////////////////////////////////////////////////
			class CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate()
				{
					m_requestStop = false;
					m_running = false;
					m_cycle = 10;
					m_name = "Thread::";
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @detail			※無効にする
				////////////////////////////////////////////////////////////////////////////////
				CTemplate& operator = (const CTemplate& object) = delete;

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "~CTemplate()" << Logging::ConstSeparator;

					// 停止
					Stop();

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数の周期を更新
				/// @param[in]		value	メイン関数の周期[ms]
				////////////////////////////////////////////////////////////////////////////////
				void SetCycle(int value)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncCycle);

					// メイン関数の周期を更新
					m_cycle = value;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			名称を更新
				/// @param[in]		name	名称
				////////////////////////////////////////////////////////////////////////////////
				void SetName(const std::string& name)
				{
					// 名称を更新
					m_name = name;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			開始
				////////////////////////////////////////////////////////////////////////////////
				void Start() throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Start()" << Logging::ConstSeparator;

					// 停止
					Stop();

					// メイン関数の開始
					StartMain();

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			停止
				////////////////////////////////////////////////////////////////////////////////
				void Stop() throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Stop()" << Logging::ConstSeparator;

					do
					{
						// メイン関数の停止を確認
						if (IsStopMain())
						{
							// 停止中
							break;
						}

						// 停止要求をON
						SetRequestStop(true);

						// 動作要求を起床
						WakeupRequestAction();

						// メイン関数の停止待ち
						StopMain();

						// 停止要求をOFF
						SetRequestStop(false);

						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					} while (false);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			動作要求を起床
				////////////////////////////////////////////////////////////////////////////////
				void WakeupRequestAction()
				{
					// 動作要求を起床
					m_requestAction.notify_one();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			名称を取得
				/// @return			名称
				////////////////////////////////////////////////////////////////////////////////
				std::string GetName()
				{
					std::string ret = "";

					// 名称を取得
					ret = m_name;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			停止要求を取得
				/// @return			停止要求フラグ
				////////////////////////////////////////////////////////////////////////////////
				bool GetRequestStop()
				{
					bool ret = false;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncRequestStop);

					// 停止要求フラグを取得
					ret = m_requestStop;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数の周期を取得
				/// @return			メイン関数の周期[ms]
				////////////////////////////////////////////////////////////////////////////////
				int GetCycle()
				{
					int ret = 0;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncCycle);

					// メイン関数の周期を取得
					ret = m_cycle;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			終了の処理
				///	@detail			★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void Finish() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数の処理
				///	@detail			★派生先で記述する
				/// @return			タイムアウト時間[ms]
				///					※初期値は10000ms
				////////////////////////////////////////////////////////////////////////////////
				virtual int MainAction() throw(...)
				{
					return 10000;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			運転中を更新
				/// @param[in]		value	true:運転中 / false:停止中
				////////////////////////////////////////////////////////////////////////////////
				void SetRunning(bool value)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncRunning);

					// 運転中フラグを更新
					m_running = value;
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			停止要求を更新
				/// @param[in]		value	true:停止要求を設定 / false:停止要求を解除
				////////////////////////////////////////////////////////////////////////////////
				void SetRequestStop(bool value)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncRequestStop);

					// 停止要求フラグを更新
					m_requestStop = value;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			運転中を取得
				/// @return			運転中フラグ
				////////////////////////////////////////////////////////////////////////////////
				bool GetRunning()
				{
					bool ret = false;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncRunning);

					// 運転中フラグを取得
					ret = m_running;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数スレッドを開始
				////////////////////////////////////////////////////////////////////////////////
				void StartMain()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncMainThread);

					// メイン関数スタート
					m_mainThread = std::thread([this] { Main(); });
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数スレッドの停止を取得
				/// @return			true:停止中 / false:動作中
				////////////////////////////////////////////////////////////////////////////////
				bool IsStopMain()
				{
					bool ret = true;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncMainThread);

					// メイン関数の起床を確認
					if (m_mainThread.joinable())
					{
						ret = false;
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数スレッドを停止
				////////////////////////////////////////////////////////////////////////////////
				void StopMain()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncMainThread);

					// メイン関数の起床を確認
					if (m_mainThread.joinable())
					{
						// メイン関数の停止待ち
						m_mainThread.join();
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			動作要求の監視待ち
				/// @param[in]		timeout	タイムアウト[ms] ※0より小さい場合は∞
				////////////////////////////////////////////////////////////////////////////////
				std::cv_status WaitRequestAction(int timeout)
				{
					std::cv_status ret = std::cv_status::timeout;

					do
					{
						// タイムアウトを確認
						if (timeout == 0)
						{
							// タイムアウトなし
							break;
						}

						// 排他制御
						std::unique_lock<std::mutex> lock(m_asyncRequestAction);

						// タイムアウトの∞を確認
						if (timeout < 0)
						{
							// 動作要求待ち
							m_requestAction.wait(lock);

							// 正常
							ret = std::cv_status::no_timeout;
							break;
						}

						// 動作要求待ち
						ret = m_requestAction.wait_for(lock, std::chrono::milliseconds(timeout));
					} while (false);

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数
				////////////////////////////////////////////////////////////////////////////////
				void Main()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "Main()" << Logging::ConstSeparator;

					int timeout;

					// タイムアウトを初期化
					timeout = 0;

					while (true)
					{
						try
						{
							// 動作要求の監視待ち
							std::cv_status result = WaitRequestAction(timeout);

							// メイン関数の処理
							timeout = MainAction();

							// 停止要求を確認
							if (GetRequestStop() == false)
							{
								continue;
							}

							// 運転中を確認
							if (GetRunning())
							{
								// タイムアウトを初期化
								timeout = 0;
								continue;
							}

							// 終了
							Finish();
							break;
						}
						catch (const std::exception& e)
						{
							// 例外の処理 ⇒ ログのクラスに変換して出力
							Transfer::Output(Exception::Convert::Logging(logging, postscript.str(), e));
							postscript.str("");
							break;
						}
					}
				}

			protected:
				//! 排他制御クラス(停止要求フラグ用)
				std::recursive_mutex m_asyncRequestStop;

				//! 停止要求フラグ
				bool m_requestStop;

				//! 排他制御クラス(運転中フラグ用)
				std::recursive_mutex m_asyncRunning;

				//! 運転中フラグ
				bool m_running;

				//! 排他制御クラス(メイン関数の周期用)
				std::recursive_mutex m_asyncCycle;

				//! メイン関数の周期[ms]
				int m_cycle;

			private:
				//! 排他制御クラス(動作要求用)
				std::mutex m_asyncRequestAction;

				//! 動作要求
				std::condition_variable m_requestAction;

				//! 排他制御クラス(メイン関数スレッド用)
				std::recursive_mutex m_asyncMainThread;

				//! メイン関数スレッド
				std::thread m_mainThread;

				//! 名称
				std::string m_name;
			};
		}
	}
}