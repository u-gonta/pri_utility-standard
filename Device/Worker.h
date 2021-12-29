#pragma once

#include "Exception/Worker.h"
#include "Common/Utility/StopWatch.h"

namespace Standard
{
	namespace Device
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      デバイスの制御を行うテンプレートのクラス
			///				⇒ 例外の制御を行うテンプレートのクラスから派生
			/// @param[in]	CDefault	デフォルトの設定クラス(テンプレートで指定)
			/// @param[in]	CConnect	接続の設定クラス(テンプレートで指定)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CDefault, typename CConnect>
			class CTemplate
				: virtual public Exception::Worker::CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Exception::Worker::CTemplate()
				{
					m_initialize = false;
					m_destroy = false;
					m_name = "Device::";
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					// 破棄
					Destroy();
				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			名称を更新
				/// @param[in]		name	名称
				////////////////////////////////////////////////////////////////////////////////
				void SetName(const std::string& name)
				{
					// 名称を更新
					m_name = name;
					SetNameException(name + "Exception::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化
				/// @detail			ライブラリの初期化までを行う
				/// @param[in]		object	デフォルトの設定クラス
				////////////////////////////////////////////////////////////////////////////////
				void Initialize(const CDefault& object) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Initialize()" << Logging::ConstSeparator;

					postscript << "デフォルトの設定クラスを更新";
					SetDefaultSetting(object);
					postscript.str("");

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 破棄済みを確認
					IsDiscarded();

					// 初期化済みを確認
					IsInitialize();

					// 初期化
					InitializeDerived();

					// 初期フラグを設定
					m_initialize = true;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄
				////////////////////////////////////////////////////////////////////////////////
				void Destroy() throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Destroy()" << Logging::ConstSeparator;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					do
					{
						// 初期フラグを確認
						if (m_initialize == false)
						{
							postscript << "未初期化";

							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
							break;
						}

						// 破棄フラグを確認
						if (m_destroy)
						{
							postscript << "破棄済み";

							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
							break;
						}

						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// 切断
						Disconnect();

						// 破棄
						DestroyDerived();

						// 初期フラグを解除
						m_initialize = false;

						// 破棄フラグを設定
						m_destroy = true;
					} while (false);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			接続
				/// @param[in]		object	接続の設定クラス
				////////////////////////////////////////////////////////////////////////////////
				void Connect(const CConnect& object) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Connect()" << Logging::ConstSeparator;

					postscript << "接続の設定クラスを更新";
					SetConnectSetting(object);
					postscript.str("");

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 制御が可能か確認
					IsControllable();

					// 切断
					Disconnect();

					// 接続
					ConnectDerived();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			切断
				////////////////////////////////////////////////////////////////////////////////
				void Disconnect() throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Disconnect()" << Logging::ConstSeparator;

					do
					{
						// 接続の状態を確認
						if (GetConnected() == false)
						{
							// 未接続
							break;
						}

						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

						// 制御が可能か確認
						IsControllable();

						// 切断
						DisconnectDerived();
					} while (false);
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化済みの確認
				////////////////////////////////////////////////////////////////////////////////
				void IsInitialize()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 初期フラグを確認
					if (m_initialize)
					{
						// 初期化済みの例外を発砲
						ExceptionInitialized();
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			制御が可能か確認
				////////////////////////////////////////////////////////////////////////////////
				void IsControllable()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 破棄済みを確認
					IsDiscarded();

					// 未初期化を確認
					IsUninitialized();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デフォルトの設定クラスを取得
				/// @return			デフォルトの設定クラス
				////////////////////////////////////////////////////////////////////////////////
				CDefault GetDefaultSetting()
				{
					CDefault ret;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncDefaultSetting);

					// デフォルトの設定クラスを取得
					ret = m_defaultSetting;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デフォルトの設定クラスを更新
				/// @param[in]		object	デフォルトの設定クラス
				////////////////////////////////////////////////////////////////////////////////
				void SetDefaultSetting(const CDefault& object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncDefaultSetting);

					// デフォルトの設定クラスを更新
					m_defaultSetting = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			接続の設定クラスを取得
				/// @return			接続の設定クラス
				////////////////////////////////////////////////////////////////////////////////
				CConnect GetConnectSetting()
				{
					CConnect ret;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncConnectSetting);

					// 接続の設定クラスを取得
					ret = m_connectSetting;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			接続の設定クラスを更新
				/// @param[in]		object	接続の設定クラス
				////////////////////////////////////////////////////////////////////////////////
				void SetConnectSetting(const CConnect& object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncConnectSetting);

					// 接続の設定クラスを更新
					m_connectSetting = object;
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
				/// @brief			初期化済みの例外を発砲
				///	@detail			初期化の例外を発砲するタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void ExceptionInitialized() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄済みの例外を発砲
				///	@detail			破棄の例外を発砲するタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void ExceptionDiscarded() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			未初期化の例外を発砲
				///	@detail			未初期化の確認で例外を発砲するタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void ExceptionUninitialized() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			接続の状態を取得
				///	@detail			接続の状態を取得するタイミングで呼び出される
				///					★派生先で記述する
				/// @return			true:接続済み / false:未接続
				////////////////////////////////////////////////////////////////////////////////
				virtual bool GetConnected() throw(...)
				{
					bool ret = false;

					return ret;
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
				/// @brief			接続
				///	@detail			接続を行うタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void ConnectDerived() throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			切断
				///	@detail			切断を行うタイミングで呼び出される
				///					★派生先で記述する
				////////////////////////////////////////////////////////////////////////////////
				virtual void DisconnectDerived() throw(...)
				{

				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄済みの確認
				////////////////////////////////////////////////////////////////////////////////
				void IsDiscarded()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 破棄フラグを確認
					if (m_destroy)
					{
						// 破棄済みの例外を発砲
						ExceptionDiscarded();
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			未初期化の確認
				////////////////////////////////////////////////////////////////////////////////
				void IsUninitialized()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 初期化フラグを確認
					if (m_initialize == false)
					{
						// 未初期化の例外を発砲
						ExceptionUninitialized();
					}
				}

			protected:
				//! 排他制御クラス(フラグ用)
				std::recursive_mutex m_asyncFlag;

				//! 初期フラグ
				bool m_initialize;

				//! 破棄フラグ
				bool m_destroy;

			private:
				//! 排他制御クラス(デフォルトの設定クラス用)
				std::recursive_mutex m_asyncDefaultSetting;

				//! デフォルトの設定クラス
				CDefault m_defaultSetting;

				//! 排他制御クラス(接続の設定クラス用)
				std::recursive_mutex m_asyncConnectSetting;

				//! 接続の設定クラス
				CConnect m_connectSetting;

				//! 名称
				std::string m_name;
			};
		}
	}
}