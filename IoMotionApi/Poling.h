#pragma once

#include "Io/Poling.h"

#include "WrapperMotionApi/Result.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      IO�̊Ď��AIO�̐���A��Ԃ�ʒm����N���X
			///				�� IO�̐���AIO�̊Ď��A��Ԃ�ʒm����e���v���[�g�̃N���X����p��
			/// @detail		IO�̊Ď��AIO�̐���A��Ԃ̒ʒm���s��
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

				// ���g���C���m�F
				bool IsRetry() override;

				// �ǂݍ���
				void ReadDerived() override;

				// ��������
				void WriteDerived() override;

			public:
				// �ڑ��̏�Ԃ��擾
				bool GetConnected();

			private:
				// �ڑ��ς݂��m�F
				void IsConnected();

			private:
				//! �r������N���X(�R���g���[�����J�������ʃN���X�p)
				std::recursive_mutex m_asyncController;

				//! �R���g���[�����J�������ʃN���X
				MotionApi::Controller::Result::COpen m_controller;
			};
		}
	}
}
