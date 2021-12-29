#pragma once

#include "Io/Poling.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      IO�̊Ď��AIO�̐���A��Ԃ�ʒm����N���X
			///				�� IO�̐���AIO�̊Ď��A��Ԃ�ʒm����e���v���[�g�̃N���X����h��
			/// @detail		IO�̊Ď��AIO�̐���A��Ԃ̒ʒm���s��
			////////////////////////////////////////////////////////////////////////////////
			class CWorker
				: virtual public Worker::CTemplate
			{
			public:
				// �R���X�g���N�^
				CWorker();

				// �f�X�g���N�^
				~CWorker();

			protected:
				// ������
				void InitializeDerived() override;

				// �j��
				void DestroyDerived() override;

				// �ǂݍ���
				void ReadDerived() override;

				// ��������
				void WriteDerived() override;
			};
		}
	}
}