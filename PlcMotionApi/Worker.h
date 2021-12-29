#pragma once

#include "Plc/Worker.h"

namespace Standard
{
	namespace Plc
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      PLC�̐�����s���N���X
		///				�� PLC�̐�����s���e���v���[�g�̃N���X����p��
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Worker::CTemplate
		{
		public:
			// �R���X�g���N�^
			CWorker();

			// �f�X�g���N�^
			~CWorker() override;

		protected:
			// ������
			void InitializeDerived() override;

			// �j��
			void DestroyDerived() override;

			// �ڑ�
			void ConnectDerived() override;

			// �ؒf
			void DisconnectDerived() override;

			// �ڑ��̏�Ԃ��擾
			bool GetConnected() override;

		private:
			//! �r������N���X(���C�u�����p)
			std::recursive_mutex m_asyncLibrary;
		};
	}
}
