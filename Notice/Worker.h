#pragma once

#include <deque>

#include "Thread/Worker.h"

namespace Standard
{
	namespace Notice
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      通知するテンプレートのクラス
			///				⇒ スレッドの制御を行うテンプレートのクラスから派生
			/// @param[in]	FCalled	通知を呼び出す関数ポインタ(テンプレートで指定)
			/// @param[in]	CNotice	通知する情報のクラス(テンプレートで指定)
			////////////////////////////////////////////////////////////////////////////////
			template <typename FCalled, typename CNotice>
			class CTemplate
				: virtual public Thread::Worker::CTemplate
			{
			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @enum			処理の順序
				////////////////////////////////////////////////////////////////////////////////
				enum EnumOrder
				{
					OrderWait,			// 待機
					OrderInitialize,	// 初期化
					OrderDestroy,		// 破棄
					OrderNotice			// 通知
				};

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Thread::Worker::CTemplate()
				{
					m_order = OrderWait;
					m_requestInitialize = false;
					m_requestDestroy = false;
					_FunctionCalled = nullptr;

					// 名称を更新
					SetName("Notice::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					// 通知する関数の解除
					DetachFunctionCalled();
				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知する関数の登録
				/// @param[in]		object	通知する関数
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionCalled(std::function<void(const CNotice&)> object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFcalled);

					_FunctionCalled = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知する関数の解除
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionCalled()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFcalled);

					_FunctionCalled = nullptr;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化を要求
				////////////////////////////////////////////////////////////////////////////////
				void RequestInitialize()
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "RequestInitialize()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestInitialize);

						// 初期化の要求を設定
						m_requestInitialize = true;

						// 動作要求を起床
						WakeupRequestAction();
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄を要求
				////////////////////////////////////////////////////////////////////////////////
				void RequestDestroy()
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "RequestDestroy()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestDestroy);

						// 破棄の要求を設定
						m_requestDestroy = true;

						// 動作要求を起床
						WakeupRequestAction();
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知を要求
				/// @param[in]		object	通知する情報のクラス
				////////////////////////////////////////////////////////////////////////////////
				void RequestNotice(const CNotice object)
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "RequestNotice()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncNotice);

						// 通知する情報のクラスを追加
						m_notices.push_back(object);
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					// 動作要求を起床
					WakeupRequestAction();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数の処理
				/// @return			タイムアウト時間[ms]
				////////////////////////////////////////////////////////////////////////////////
				int MainAction() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "MainAction()" << Logging::ConstSeparator;

					int ret = 0;

					// 既定の関数
					ret = Thread::Worker::CTemplate::MainAction();

					try
					{
						switch (GetOrder())
						{
						case OrderWait:
							// 待機 ⇒ 初期化の要求を確認
							if (IsRequestInitialize())
							{
								// 運転中を更新
								SetRunning(true);

								// 初期化へ移行
								SetOrder(OrderInitialize);

								// タイムアウトを初期化
								ret = 0;
								break;
							}

							// 運転中を解除
							SetRunning(false);
							break;

						case OrderInitialize:
							// 初期化 ⇒ 通知へ移行
							SetOrder(OrderNotice);

							// タイムアウトを初期化
							ret = 0;
							break;

						case OrderDestroy:
							// 破棄 ⇒ 待機へ移行
							SetOrder(OrderWait);

							// タイムアウトを初期化
							ret = 0;
							break;

						case OrderNotice:
							// 通知 ⇒ 通知するデータの有無を確認
							if (IsNotice())
							{
								// 通知するデータあり ⇒ 通知
								Notice();

								// タイムアウトを初期化
								ret = 0;
								break;
							}

							// 停止の要求 or 破棄の要求を確認
							if (GetRequestStop() || IsRequestDestroy())
							{
								// 停止要求 ⇒ 破棄へ移行
								SetOrder(OrderDestroy);

								// タイムアウトを初期化
								ret = 0;
								break;
							}

							// 通知するデータなし ⇒ メイン関数の周期でタイムアウトを設定
							ret = GetCycle();
							break;
						}
					}
					catch (const Exception::CObject& e)
					{
						// 例外の処理 ⇒ ログのクラスに変換して出力
						Transfer::Output(Exception::Convert::Logging(logging, postscript.str(), e));
						postscript.str("");

						switch (GetOrder())
						{
						case OrderDestroy:
							// 破棄 ⇒ 待機へ移行
							SetOrder(OrderWait);
							break;

						default:
							// その他 ⇒ 破棄へ移行
							SetOrder(OrderDestroy);
							break;
						}

						// タイムアウトを初期化
						ret = 0;
					}

					return ret;
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			処理の順序を取得
				/// @return			処理の順序
				////////////////////////////////////////////////////////////////////////////////
				EnumOrder GetOrder()
				{
					EnumOrder ret = OrderWait;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "GetOrder()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncOrder);

						ret = m_order;
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			処理の順序を更新
				/// @param[in]		value	処理の順序
				////////////////////////////////////////////////////////////////////////////////
				void SetOrder(EnumOrder value)
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "SetOrder()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncOrder);

						m_order = value;
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化の要求を確認
				/// @detail			確認後に初期化の要求は解除する
				/// @return			true:初期化を要求 / false:初期化は未要求
				////////////////////////////////////////////////////////////////////////////////
				bool IsRequestInitialize()
				{
					bool ret = false;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "IsRequestInitialize()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestInitialize);

						// 初期化の要求を取得
						ret = m_requestInitialize;

						// 初期化の要求を解除
						m_requestInitialize = false;
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄の要求を確認
				/// @detail			確認後に破棄の要求は解除する
				/// @return			true:破棄を要求 / false:破棄は未要求
				////////////////////////////////////////////////////////////////////////////////
				bool IsRequestDestroy()
				{
					bool ret = false;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "IsRequestDestroy()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestDestroy);

						// 破棄の要求を取得
						ret = m_requestDestroy;

						// 破棄の要求を解除
						m_requestDestroy = false;
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知するデータの有無を確認
				/// @return			true:通知するデータあり / false:通知するデータなし
				////////////////////////////////////////////////////////////////////////////////
				bool IsNotice()
				{
					bool ret = false;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "IsNotice()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncNotice);

						// 通知する情報のクラス数を確認
						if (0 < m_notices.size())
						{
							// 通知する情報あり
							ret = true;
						}
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知する情報のクラスで先頭のデータを取得
				/// @return			通知する情報のクラス
				////////////////////////////////////////////////////////////////////////////////
				std::shared_ptr<CNotice> GetsNoticeFront()
				{
					std::shared_ptr<CNotice> ret = nullptr;

					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "GetsNoticeFront()";

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncNotice);

						// 通知する情報のクラス数を確認
						if (0 < m_notices.size())
						{
							// 通知する情報のクラスあり ⇒ 先頭の通知する情報のクラスを取得
							ret = std::make_shared<CNotice>(m_notices.front());
						}
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知を呼び出し
				/// @param[in]		object	通知する情報のクラス
				////////////////////////////////////////////////////////////////////////////////
				void Call(CNotice object)
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "Call()";

					try
					{
						do
						{
							// 排他制御
							std::lock_guard<std::recursive_mutex> lock(m_asyncFcalled);

							// 通知する関数を確認
							if (_FunctionCalled == nullptr)
							{
								// 通知する関数なし
								break;
							}

							// 通知を呼び出し
							_FunctionCalled(object);
						} while (false);
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知を実行
				////////////////////////////////////////////////////////////////////////////////
				void Notice()
				{
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					postscript << GetName() << "Notice()";

					try
					{
						do
						{
							// 通知する情報のクラスで先頭のデータを取得
							std::shared_ptr<CNotice> notice = GetsNoticeFront();

							// 通知する情報のクラスを確認
							if (notice == nullptr)
							{
								// 通知する情報のクラスなし
								break;
							}

							// 通知を呼び出し
							Call(*notice);

							// 排他制御
							std::lock_guard<std::recursive_mutex> lock(m_asyncNotice);

							// 通知する情報のクラス数を確認
							if (0 < m_notices.size())
							{
								// 通知する情報のクラスあり ⇒ 先頭の通知する情報のクラスを削除
								m_notices.erase(m_notices.begin());
							}
						} while (false);
					}
					catch (const std::exception& e)
					{
						// 例外の処理
						postscript << Logging::ConstSeparator << e.what();

						// 例外を発砲
						throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					}
				}

			private:
				//! 排他制御クラス(処理の順序用)
				std::recursive_mutex m_asyncOrder;

				//! 処理の順序
				EnumOrder m_order;

				//! 排他制御クラス(初期化の要求用)
				std::recursive_mutex m_asyncRequestInitialize;

				//! 初期化の要求
				bool m_requestInitialize;

				//! 排他制御クラス(破棄の要求用)
				std::recursive_mutex m_asyncRequestDestroy;

				//! 破棄の要求
				bool m_requestDestroy;

				//! 排他制御クラス(通知する関数用)
				std::recursive_mutex m_asyncFcalled;

				//! 通知する関数
				std::function<void(const CNotice&)> _FunctionCalled;

				//! 排他制御クラス(通知する情報のクラス用)
				std::recursive_mutex m_asyncNotice;

				//! 通知する情報のクラス
				std::deque<CNotice> m_notices;
			};
		}
	}
}