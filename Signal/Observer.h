#pragma once

namespace Standard
{
	namespace Signal
	{
		namespace Observer
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CSetting
			/// @brief      信号の受信を待機する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			class CSetting
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CSetting()
				{
					Timeout = 1000;
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
				virtual ~CSetting()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CSetting& object)
				{
					this->Timeout = object.Timeout;
				}

			public:
				//! タイムアウト[ms]
				int Timeout;
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CObserver
			/// @brief      信号の受信を待機するテンプレートのクラス
			/// @param[in]	CArgument	受信する情報のクラス(テンプレートで指定)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CArgument>
			class CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				////////////////////////////////////////////////////////////////////////////////
				CTemplate& operator = (const CTemplate& object)
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{

				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知を呼び出される
				/// @param[in]		object	受信データのクラス
				////////////////////////////////////////////////////////////////////////////////
				void OnCalled(const CArgument& object) noexcept
				{
					try
					{
						// 受信データのクラスを更新
						SetArgument(object);

						// 受信データの処理
						ReceiveDerived(object);
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外のクラスを更新
						SetException(Exception::CodeUnknown, e.what());
					}

					// イベント発生を起床
					m_occurrence.notify_one();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			受信データのクラスを取得
				/// @return			受信データのクラス
				////////////////////////////////////////////////////////////////////////////////
				CArgument GetArgument()
				{
					CArgument ret;

					// 排他制御
					std::unique_lock<std::mutex> lock(m_asyncArgument);

					ret = m_argument;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			呼び出され待ち
				////////////////////////////////////////////////////////////////////////////////
				void Wait() throw(...)
				{
					CSetting setting = Setting;

					// 排他制御
					std::unique_lock<std::mutex> lock(m_asyncOccurrence);

					// タイムアウトの∞を確認
					if (setting.Timeout < 0)
					{
						// タイムアウトなし ⇒ イベント発生待ち
						m_occurrence.wait(lock);
					}
					else
					{
						// タイムアウトあり ⇒ イベント発生待ち
						if (m_occurrence.wait_for(lock, std::chrono::milliseconds(setting.Timeout)) == std::cv_status::timeout)
						{
							std::stringstream postscript;

							postscript << "タイムアウト:" << setting.Timeout << "[ms]";

							// 例外を発砲
							throw std::exception(postscript.str().c_str());
						}

						// 例外のクラス
						Exception::CObject exception;

						// 例外のクラスを取得
						exception = GetException();

						// 例外が発生したか確認
						if (exception.ErrorCode != Exception::CodeSuccess)
						{
							// 例外を発砲
							throw std::exception(exception.Message.c_str());
						}
					}
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CTemplate& object)
				{
					this->m_argument = object.m_argument;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			受信データの処理
				/// @detail			★派生先で記述
				/// @param[in]		object	受信データのクラス
				////////////////////////////////////////////////////////////////////////////////
				virtual void ReceiveDerived(const CArgument& object)
				{

				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外のクラスを更新
				/// @param[in]		code	例外コード
				/// @param[in]		message	メッセージ
				////////////////////////////////////////////////////////////////////////////////
				void SetException(Exception::EnumCode code, const std::string& message)
				{
					// 排他制御
					std::unique_lock<std::mutex> lock(m_asyncException);

					m_exception.ErrorCode = code;
					m_exception.Message = message;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外のクラスを取得
				/// @return			例外のクラス
				////////////////////////////////////////////////////////////////////////////////
				Exception::CObject GetException()
				{
					Exception::CObject ret;

					// 排他制御
					std::unique_lock<std::mutex> lock(m_asyncException);

					ret = m_exception;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			受信データのクラスを更新
				/// @param[in]		object	受信データのクラス
				////////////////////////////////////////////////////////////////////////////////
				void SetArgument(const CArgument& object)
				{
					// 排他制御
					std::unique_lock<std::mutex> lock(m_asyncArgument);

					m_argument = object;
				}

			public:
				//! 信号の受信を待機する設定クラス
				CSetting Setting;

			private:
				//! 排他制御クラス(例外のクラス用)
				std::mutex m_asyncException;

				//! 例外のクラス
				Exception::CObject m_exception;

				//! 排他制御クラス(シグナル発生用)
				std::mutex m_asyncOccurrence;

				//! イベント発生
				std::condition_variable m_occurrence;

				//! 排他制御クラス(受信データのクラス用)
				std::mutex m_asyncArgument;

				//! 受信データのクラス
				CArgument m_argument;
			};
		}
	}
}