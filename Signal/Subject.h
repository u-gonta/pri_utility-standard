#pragma once

#include <mutex>

#include "boost/signals2.hpp"

namespace Standard
{
	namespace Signal
	{
		namespace Subject
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      信号を通知するクラス
			/// @param[in]	CArgument	通知する情報のクラス(テンプレートで指定)
			////////////////////////////////////////////////////////////////////////////////
			template <typename CArgument>
			class CTemplate
			{
			public:
				//! 通知する関数の定義
				typedef void(OnCall)(const CArgument&);

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
					// 通知をする関数の解除
					CancellationFunction();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知をする関数の登録
				/// @param[in]		object	通知をする関数
				////////////////////////////////////////////////////////////////////////////////
				void RegistrationFunction(std::weak_ptr<std::function<OnCall>> object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFunction);

					_Function = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知をする関数の解除
				////////////////////////////////////////////////////////////////////////////////
				void CancellationFunction()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFunction);

					_Function.reset();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			通知を呼び出し
				/// @param[in]		object	通知するデータのクラス
				////////////////////////////////////////////////////////////////////////////////
				void CallingFunction(const CArgument& object)
				{
					// 関数のシグナル
					boost::signals2::signal<OnCall> signal;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFunction);

					// 関数のポインタ
					std::shared_ptr<std::function<OnCall>> function = _Function.lock();

					// 関数を確認
					if (function == nullptr)
					{
						// 関数の実態なし
						throw std::exception("関数の実態なし");
					}

					// 関数へ接続
					boost::signals2::connection connection = signal.connect(*function);

					// 接続を確認
					if (connection.connected() == false)
					{
						// 未接続
						throw std::exception("関数が未接続");
					}

					// 呼び出し
					signal(object);

					// 切断
					connection.disconnect();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CTemplate& object)
				{
					this->_Function = object._Function;
				}

			private:
				//! 排他制御クラス(通知する関数用)
				std::recursive_mutex m_asyncFunction;

				//! 通知する関数
				std::weak_ptr<std::function<OnCall>> _Function;
			};
		}
	}
}