#pragma once

#include "boost/asio.hpp"
#include "boost/thread.hpp"

namespace Standard
{
	namespace Utility
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CSequentialAsync
		/// @brief      �����񓯊������̃N���X
		///	@detail		IO�T�[�r�X�Ƀ|�X�g���ꂽ�����ʂ�ɔ񓯊��ŏ��������s����
		////////////////////////////////////////////////////////////////////////////////
		class CSequentialAsync
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			/// @param[in]		io_service	IO�T�[�r�X
			/// @param[in]		count	�X���b�h��
			////////////////////////////////////////////////////////////////////////////////
			CSequentialAsync(boost::asio::io_service& io_service, std::size_t count)
				: m_io_service(io_service)
			{
				m_work.reset(new boost::asio::io_service::work(m_io_service));

				// �X���b�h����������
				for (std::size_t index = 0; index < count; index++)
				{
					m_group.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�f�X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			~CSequentialAsync()
			{
				// IO�T�[�r�X���~
				Stop();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�T�[�r�X���~
			////////////////////////////////////////////////////////////////////////////////
			void Stop()
			{
				m_work.reset();
				m_group.join_all();
			}

		private:
			//! IO�T�[�r�X
			boost::asio::io_service& m_io_service;

			//! IO�T�[�r�X�̃��[�J�[
			boost::shared_ptr<boost::asio::io_service::work> m_work;

			//! IO�T�[�r�X�̃O���[�v
			boost::thread_group m_group;
		};
	}
}