#pragma once

#include "boost/asio.hpp"
#include "boost/thread.hpp"

namespace Standard
{
	namespace Utility
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CSequentialAsync
		/// @brief      順次非同期処理のクラス
		///	@detail		IOサービスにポストされた順序通りに非同期で処理を実行する
		////////////////////////////////////////////////////////////////////////////////
		class CSequentialAsync
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			/// @param[in]		io_service	IOサービス
			/// @param[in]		count	スレッド数
			////////////////////////////////////////////////////////////////////////////////
			CSequentialAsync(boost::asio::io_service& io_service, std::size_t count)
				: m_io_service(io_service)
			{
				m_work.reset(new boost::asio::io_service::work(m_io_service));

				// スレッド数だけ走査
				for (std::size_t index = 0; index < count; index++)
				{
					m_group.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デストラクタ
			////////////////////////////////////////////////////////////////////////////////
			~CSequentialAsync()
			{
				// IOサービスを停止
				Stop();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			サービスを停止
			////////////////////////////////////////////////////////////////////////////////
			void Stop()
			{
				m_work.reset();
				m_group.join_all();
			}

		private:
			//! IOサービス
			boost::asio::io_service& m_io_service;

			//! IOサービスのワーカー
			boost::shared_ptr<boost::asio::io_service::work> m_work;

			//! IOサービスのグループ
			boost::thread_group m_group;
		};
	}
}