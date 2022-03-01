#pragma once

#include "boost/any.hpp"

#include "Thread/Worker.h"
#include "Exception/Worker.h"
#include "Signal/Observer.h"
#include "Signal/Subject.h"
#include "Signal/Result.h"
#include "Common/Utility/StopWatch.h"

namespace Standard
{
	namespace Signal
	{
		namespace Trigger
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CInitialize
			/// @brief      初期化の結果クラス
			///				⇒ 状態を監視した結果を保持するテンプレートのクラスから継承
			////////////////////////////////////////////////////////////////////////////////
			class CInitialize
				: virtual public Result::CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CInitialize() : Result::CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @return			自クラス
				////////////////////////////////////////////////////////////////////////////////
				CInitialize& operator = (const CInitialize& object) noexcept
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CInitialize()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CInitialize& object)
				{

				}
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CDestroy
			/// @brief      破棄の結果クラス
			///				⇒ 状態を監視した結果を保持するテンプレートのクラスから継承
			////////////////////////////////////////////////////////////////////////////////
			class CDestroy
				: virtual public Result::CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CDestroy() : Result::CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @return			自クラス
				////////////////////////////////////////////////////////////////////////////////
				CDestroy& operator = (const CDestroy& object) noexcept
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CDestroy()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CDestroy& object)
				{

				}
			};

			//! 初期化を通知する関数の定義
			typedef void(OnWakeupInitialize)(const CInitialize&);

			//! 破棄を通知する関数の定義
			typedef void(OnWakeupDestroy)(const CDestroy&);
		}

		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      状態を監視、書き込み、通知するテンプレートのクラス
			///				⇒ スレッドの制御を行うテンプレートのクラスから派生
			///				⇒ 例外の制御を行うテンプレートのクラスから派生
			/// @param[in]	CSetting	状態を監視、通知する設定クラス(テンプレートで指定)
			/// @param[in]	CArgument	通知する情報のクラス(テンプレートで指定)
			/// @param[in]	CWrite	書き込む情報のクラス(テンプレートで指定)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CSetting, typename CArgument, typename CWrite>
			class CTemplate
				: virtual public Thread::Worker::CTemplate, virtual public Exception::Worker::CTemplate
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
					OrderRetry,			// リトライ
					OrderRead,			// 読み込み
					OrderWrite			// 書き込み
				};

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Thread::Worker::CTemplate(), Exception::Worker::CTemplate()
				{
					m_order = OrderWait;
					m_requestInitialize = false;
					m_requestDestroy = false;
					m_retry = 0;

					std::string name;

					// 名称を更新
					name = "Signal::";
					SetNameException(name + "Exception::");
					SetNameArgument(name + "Notice::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					// 通知する関数を解除
					DetachFunctionCalled();
				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知の名称を更新
				/// @param[in]		name	通知の名称
				////////////////////////////////////////////////////////////////////////////////
				void SetNameArgument(const std::string& name)
				{
					// 名称を更新
					m_noticeArgument.SetName(name);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			状態の通知を開始
				/// @detail			通知する関数を登録する前に呼び出す
				/// @param[in]		cycle	状態の通知を監視する間隔[ms]
				////////////////////////////////////////////////////////////////////////////////
				void StartNotice(int cycle = 60000)
				{
					// 状態の通知を行う周期を設定
					m_noticeArgument.SetCycle(cycle);

					// 状態の通知を開始
					m_noticeArgument.Start();

					// 状態の通知で初期化を要求
					m_noticeArgument.RequestInitialize();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			状態の通知を停止
				/// @detail			通知する関数を解除後に呼び出す
				////////////////////////////////////////////////////////////////////////////////
				void StopNotice()
				{
					// 状態の通知で破棄を要求
					m_noticeArgument.RequestDestroy();

					// 状態の通知を停止
					m_noticeArgument.Stop();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知する関数の登録
				/// @detail			通知時に呼び出されたい関数を指定する
				/// @param[in]		object	呼び出されたい関数
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionCalled(std::function<void(const CArgument&)> object)
				{
					// 状態の通知を開始
					StartNotice();

					// 通知する関数を登録
					m_noticeArgument.AttachFunctionCalled(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知する関数の解除
				/// @detail			通知時に呼び出される関数を解除する
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionCalled()
				{
					// 通知する関数を解除
					m_noticeArgument.DetachFunctionCalled();

					// 状態の通知を停止
					StopNotice();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			設定のクラスを取得
				/// @return			設定のクラス
				////////////////////////////////////////////////////////////////////////////////
				CSetting GetSetting()
				{
					CSetting ret;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "GetSetting()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncSetting);

						// 設定のクラスを取得
						ret = m_setting;
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			設定のクラスを更新
				/// @param[in]		object	設定のクラス
				////////////////////////////////////////////////////////////////////////////////
				void SetSetting(const CSetting& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "SetSetting()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncSetting);

						// 設定のクラスを更新
						m_setting = object;
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化を要求
				////////////////////////////////////////////////////////////////////////////////
				void RequestInitialize()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "RequestInitialize()" << Logging::ConstSeparator;

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
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄を要求
				////////////////////////////////////////////////////////////////////////////////
				void RequestDestroy()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "RequestDestroy()" << Logging::ConstSeparator;

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
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			書き込みを要求
				/// @param[in]		object	書き込む情報のクラス
				////////////////////////////////////////////////////////////////////////////////
				void RequestWrite(const CWrite& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "RequestWrite()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						// 書き込む情報のクラスを追加
						m_writes.push_back(object);

						// 動作要求を起床
						WakeupRequestAction();
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			状態を通知
				/// @param[in]		object	例外のクラス
				////////////////////////////////////////////////////////////////////////////////
				void NotifyArgument(const CArgument& object)
				{
					// 状態を通知
					m_noticeArgument.RequestNotice(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			リトライ回数を初期化
				////////////////////////////////////////////////////////////////////////////////
				void InitializeRetry()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncRetry);

					// リトライ回数を初期化
					m_retry = 0;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			リトライ回数を更新
				////////////////////////////////////////////////////////////////////////////////
				void CountupRetry()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncRetry);

					// リトライ回数を更新
					m_retry += 1;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			リトライ回数を取得
				/// @return			リトライ回数
				////////////////////////////////////////////////////////////////////////////////
				int GetRetry()
				{
					int ret = 0;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncRetry);

					// リトライ回数を取得
					ret = m_retry;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			リトライを確認
				///	@detail			※初期値は無効
				///					★派生先で記述する
				/// @return			true:リトライが有効 / false:リトライが無効
				////////////////////////////////////////////////////////////////////////////////
				virtual bool IsRetry()
				{
					bool ret = false;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			リトライの遅延時間を取得
				///	@detail			※初期値は1000ms
				///					★派生先で記述する
				/// @return			リトライの遅延時間[ms]
				////////////////////////////////////////////////////////////////////////////////
				virtual int GetDelayRetry()
				{
					int ret = 1000;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			書き込む情報のクラス数を取得
				/// @return			書き込む情報のクラス数
				////////////////////////////////////////////////////////////////////////////////
				size_t GetWriteSize()
				{
					size_t ret = 0;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

					// 書き込むデータのクラス数を取得
					ret = m_writes.size();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			書き込みを確認
				///	@detail			書き込むデータのクラスが存在するか以外の判定は派生先で記述する
				///					★派生先で記述する
				/// @return			true:書き込むデータあり / false:書き込むデータなし
				////////////////////////////////////////////////////////////////////////////////
				virtual bool IsWrite()
				{
					bool ret = false;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "IsWrite()" << Logging::ConstSeparator;

					try
					{
						// 書き込む情報のクラス数を確認
						if (0 < GetWriteSize())
						{
							// 書き込むデータあり
							ret = true;
						}
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			書き込む情報のクラスを取得
				/// @param[in]		書き込む情報のインデックス番号
				/// @return			書き込む情報のクラス
				////////////////////////////////////////////////////////////////////////////////
				std::shared_ptr<CWrite> GetWrite(size_t index)
				{
					std::shared_ptr<CWrite> ret = nullptr;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "GetWrite()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						// 書き込むデータのクラス数を確認
						if (m_writes.size() <= index)
						{
							// 書き込むデータなし
							std::stringstream message;
							message << "登録数:" << m_writes.size() << Logging::ConstSeparator << "指定:" << index;
							throw message.str();
						}

						// 書き込むデータあり
						ret = std::make_shared<CWrite>(*(m_writes.begin() + index));
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			書き込む情報のクラスを削除
				/// @param[in]		indexs	書き込む情報のインデックス番号
				////////////////////////////////////////////////////////////////////////////////
				void PopWrite(const std::vector<size_t>& indexs)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "PopWrite()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						std::vector<size_t> targets;

						// インデックス番号をコピー
						std::copy(indexs.begin(), indexs.end(), std::back_inserter(targets));

						// インデックス番号を降順でソート
						std::sort(targets.begin(), targets.end(), std::greater<size_t>());

						// インデックス番号を走査
						for (const auto& index : targets)
						{
							// 書き込むデータのクラス数を確認
							if (m_writes.size() <= index)
							{
								// 書き込むデータの範囲外
								continue;
							}

							// データを削除
							m_writes.erase(m_writes.begin() + index);
						}
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			書き込む情報のクラスを取得
				///	@detail			先頭に登録してあるデータを取得する
				/// @return			書き込む情報のクラス
				////////////////////////////////////////////////////////////////////////////////
				std::shared_ptr<CWrite> GetWriteFirst()
				{
					std::shared_ptr<CWrite> ret = nullptr;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "GetWriteFirst()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						// 書き込むデータのクラス数を確認
						if (0 < m_writes.size())
						{
							// 書き込むデータあり
							ret = std::make_shared<CWrite>(m_writes.front());
						}
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			書き込む情報のクラスを削除
				///	@detail			先頭に登録してあるデータを削除する
				////////////////////////////////////////////////////////////////////////////////
				void PopWriteFirst()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "PopWriteFirst()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

						// 書き込むデータのクラス数を確認
						if (0 < m_writes.size())
						{
							// 書き込むデータあり ⇒ 先頭のデータを削除
							m_writes.pop_front();
						}
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化
				///	@detail			初期化を行うタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void InitializeDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄
				///	@detail			破棄を行うタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void DestroyDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			読み込み
				///	@detail			読み込みを行うタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void ReadDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			書き込み
				///	@detail			書き込みを行うタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void WriteDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			メイン関数の処理
				/// @return			タイムアウト時間[ms]
				////////////////////////////////////////////////////////////////////////////////
				int MainAction() override
				{
					int ret = 0;

					// 既定の関数
					ret = Thread::Worker::CTemplate::MainAction();

					try
					{
						// 書き込みフラグ
						bool write = false;

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

							// 破棄の要求を確認
							if (IsRequestDestroy())
							{
								// 運転中を更新
								SetRunning(true);

								// 停止要求 ⇒ 破棄へ移行
								SetOrder(OrderDestroy);

								// タイムアウトを初期化
								ret = 0;
								break;
							}

							// 運転中を解除
							SetRunning(false);
							break;

						case OrderInitialize:
							// 初期化
							InitializeDerived();

							// 破棄の要求を解除
							ReleaseDestroy();

							// 読み込みへ移行
							SetOrder(OrderRead);

							// タイムアウトを初期化
							ret = 0;
							break;

						case OrderDestroy:
							// 破棄
							DestroyDerived();

							// 待機へ移行
							SetOrder(OrderWait);

							// タイムアウトを初期化
							ret = 0;
							break;

						case OrderRetry:
							// リトライ ⇒ 初期化へ移行
							SetOrder(OrderInitialize);

							// タイムアウトを初期化
							ret = 0;
							break;

						case OrderRead:
							// 読み込み ⇒ 書き込むデータのクラスを確認
							write = IsWrite();

							// 書き込むデータがない and 停止の要求 or 破棄の要求を確認
							if (write == false && (GetRequestStop() || IsRequestDestroy()))
							{
								// 停止要求 ⇒ 破棄へ移行
								SetOrder(OrderDestroy);

								// タイムアウトを初期化
								ret = 0;
								break;
							}

							// 読み込み
							ReadDerived();

							// リトライを初期化
							InitializeRetry();

							if (write)
							{
								// 書き込みデータあり ⇒ 書き込みへ移行
								SetOrder(OrderWrite);

								// タイムアウトを初期化
								ret = 0;
								break;
							}

							// メイン関数の周期でタイムアウトを設定
							ret = GetCycle();
							break;

						case OrderWrite:
							// 書き込み
							WriteDerived();

							// リトライを初期化
							InitializeRetry();

							// 読み込みへ移行
							SetOrder(OrderRead);

							// タイムアウトを初期化
							ret = 0;
							break;
						}
					}
					catch (const Exception::CObject& e)
					{
						// 例外の処理 ⇒ 例外を通知
						OutputException((Exception::EnumCode)e.ErrorCode, e.DeviceErrorCode, e.Message.c_str());

						// タイムアウトを初期化
						ret = 0;

						switch (GetOrder())
						{
						case OrderDestroy:
							// 破棄 ⇒ 待機へ移行
							SetOrder(OrderWait);
							break;

						default:
							// その他 ⇒ リトライ回数を更新
							CountupRetry();

							// リトライを確認
							if (IsRetry())
							{
								// リトライの遅延時間を取得
								ret = GetDelayRetry();

								// リトライへ移行
								SetOrder(OrderRetry);
							}
							else
							{
								// 破棄へ移行
								SetOrder(OrderDestroy);
							}
							break;
						}
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

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "GetOrder()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncOrder);

						ret = m_order;
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			処理の順序を更新
				/// @param[in]		value	処理の順序
				////////////////////////////////////////////////////////////////////////////////
				void SetOrder(EnumOrder value)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "SetOrder()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncOrder);

						m_order = value;
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
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

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "IsRequestInitialize()" << Logging::ConstSeparator;

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
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
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

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "IsRequestDestroy()" << Logging::ConstSeparator;

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
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄の要求を解除
				////////////////////////////////////////////////////////////////////////////////
				void ReleaseDestroy()
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeUnknown;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "ReleaseDestroy()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncRequestDestroy);

						// 破棄の要求を解除
						m_requestDestroy = false;
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を発砲
						throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
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

				//! 排他制御クラス(設定用)
				std::recursive_mutex m_asyncSetting;

				//! 設定のクラス
				CSetting m_setting;

				//! 排他制御クラス(リトライ用)
				std::recursive_mutex m_asyncRetry;

				//! リトライの回数
				int m_retry;

				//! 排他制御クラス(書き込むデータのクラス用)
				std::recursive_mutex m_asyncWrite;

				//! 書き込むデータのクラス
				std::deque<CWrite> m_writes;

				//! 状態を通知するクラス
				Notice::Worker::CTemplate<void(const CArgument&), CArgument> m_noticeArgument;
			};
		}
	}
}