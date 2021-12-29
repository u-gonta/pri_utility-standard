#pragma once

#include "Signal/Worker.h"

namespace Standard
{
	namespace Signal
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplateExpand
			/// @brief      状態を監視、書き込み、通知するテンプレートを拡張したクラス
			///				⇒ 状態を監視、書き込み、通知するテンプレートのクラスから派生
			///				※初期化、破棄の関数を追加
			/// @param[in]	CSetting	状態を監視、通知する設定クラス(テンプレートで指定)
			/// @param[in]	CArgument	通知する情報のクラス(テンプレートで指定)
			/// @param[in]	CWrite	書き込む情報のクラス(テンプレートで指定)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CSetting, typename CArgument, typename CWrite>
			class CTemplateExpand
				: virtual public CTemplate<CSetting, CArgument, CWrite>
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplateExpand() : CTemplate<CSetting, CArgument, CWrite>()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplateExpand()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化
				////////////////////////////////////////////////////////////////////////////////
				void Initialize()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					std::stringstream message;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "Initialize()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// トリガーを待機するクラス
					Observer::CTemplate<Trigger::CInitialize> observer;

					// トリガーを通知をする関数を登録
					postscript << "Subject::RegistrationFunction()";
					std::shared_ptr<std::function<Trigger::OnWakeupInitialize>> onWakeup = std::make_shared<std::function<Trigger::OnWakeupInitialize>>(std::bind(&Observer::CTemplate<Trigger::CInitialize>::OnCalled, &observer, std::placeholders::_1));
					postscript.str("");

					// トリガーを待機するタイムアウトを更新
					observer.Setting.Timeout = GetInitializeTimeout();

					// 初期化を監視してトリガの通知をする関数を登録
					postscript << "AdditionWakeupInitialize()";
					AdditionWakeupInitialize(onWakeup);
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 初期化を要求
					postscript << "RequestInitialize()";
					this->RequestInitialize();
					postscript.str("");

					// トリガーを待つ
					postscript << "Observer::Wait()";
					observer.Wait();
					// ログ出力
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄
				////////////////////////////////////////////////////////////////////////////////
				void Destroy()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					std::stringstream message;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "Destroy()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// トリガーを待機するクラス
					Observer::CTemplate<Trigger::CDestroy> observer;

					// トリガーを通知をする関数を登録
					postscript << "Subject::RegistrationFunction()";
					std::shared_ptr<std::function<Trigger::OnWakeupDestroy>> onWakeup = std::make_shared<std::function<Trigger::OnWakeupDestroy>>(std::bind(&Observer::CTemplate<Trigger::CDestroy>::OnCalled, &observer, std::placeholders::_1));
					postscript.str("");

					// トリガーを待機するタイムアウトを更新
					observer.Setting.Timeout = GetDestroyTimeout();

					// 破棄を監視してトリガの通知をする関数を登録
					postscript << "AdditionWakeupDestroy()";
					AdditionWakeupDestroy(onWakeup);
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 破棄を要求
					postscript << "RequestDestroy()";
					this->RequestDestroy();
					postscript.str("");

					// トリガーを待つ
					postscript << "Observer::Wait()";
					observer.Wait();
					// ログ出力
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 結果を取得
					auto result = observer.GetArgument();

					// 結果を確認
					if (result.Suspension)
					{
						// 中断が発生 ⇒ 例外を発砲

					}
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			名称を取得
				/// 				※スレッドのクラスで保持している名称を取得するように追加
				/// @return			名称
				////////////////////////////////////////////////////////////////////////////////
				std::string GetName()
				{
					std::string ret = "";

					// 名称を取得
					ret = Thread::Worker::CTemplate::GetName();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化のタイムアウト時間を取得
				///	@detail			※初期値は1000ms
				///					★派生先で記述する
				/// @return			タイムアウト時間[ms]
				////////////////////////////////////////////////////////////////////////////////
				virtual int GetInitializeTimeout()
				{
					int ret = 1000;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄のタイムアウト時間を取得
				///	@detail			※初期値は1000ms
				///					★派生先で記述する
				/// @return			タイムアウト時間[ms]
				////////////////////////////////////////////////////////////////////////////////
				virtual int GetDestroyTimeout()
				{
					int ret = 1000;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化の完了を起床
				/// @param[in]		object	初期化の結果クラス
				////////////////////////////////////////////////////////////////////////////////
				void WakeupInitializeComplete(const Trigger::CInitialize& object) noexcept
				{
					Logging::CObject logging;

					logging.Message << GetName() << "WakeupInitializeComplete()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncInitialize);

						Subject::CTemplate<Trigger::CInitialize> subject;

						// 関数を登録
						subject.RegistrationFunction(m_initialize);

						// 関数を通知
						subject.CallingFunction(object);
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ ログへ追記
						logging.Message << e.what();
						// ログを保存
						Transfer::Output(logging);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄の完了を起床
				/// @param[in]		object	破棄の結果クラス
				////////////////////////////////////////////////////////////////////////////////
				void WakeupDestroyComplete(const Trigger::CDestroy& object) noexcept
				{
					Logging::CObject logging;

					logging.Message << GetName() << "WakeupDestroyComplete()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncDestroy);

						Subject::CTemplate<Trigger::CDestroy> subject;

						// 関数を登録
						subject.RegistrationFunction(m_destroy);

						// 関数を通知
						subject.CallingFunction(object);
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ ログへ追記
						logging.Message << e.what();
						// ログを保存
						Transfer::Output(logging);
					}
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化の通知を登録
				/// param[in]		object	通知する関数
				////////////////////////////////////////////////////////////////////////////////
				void AdditionWakeupInitialize(std::weak_ptr<std::function<Trigger::OnWakeupInitialize>> object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncInitialize);

					// 関数を更新
					m_initialize = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄の通知を登録
				/// param[in]		object	通知する関数
				////////////////////////////////////////////////////////////////////////////////
				void AdditionWakeupDestroy(std::weak_ptr<std::function<Trigger::OnWakeupDestroy>> object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncDestroy);

					// 関数を更新
					m_destroy = object;
				}

			private:
				//! 排他制御クラス(初期化の完了を通知する関数用)
				std::recursive_mutex m_asyncInitialize;

				//! 初期化の完了を通知する関数
				std::weak_ptr<std::function<Trigger::OnWakeupInitialize>> m_initialize;

				//! 排他制御クラス(破棄の完了を通知する関数用)
				std::recursive_mutex m_asyncDestroy;

				//! 破棄の完了を通知する関数
				std::weak_ptr<std::function<Trigger::OnWakeupDestroy>> m_destroy;
			};
		}
	}
}