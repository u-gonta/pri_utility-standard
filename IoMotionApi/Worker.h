#pragma once

#include "Io/Worker.h"
#include "Poling.h"

namespace Standard
{
	namespace Io
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      IO�̊Ď��AIO�̐���A��Ԃ�ʒm����N���X
		///				�� IO�̊Ď��AIO�̐���A��Ԃ�ʒm����e���v���[�g�̃N���X����h��
		/// @detail		IO�̊Ď��AIO�̐���A��Ԃ̒ʒm���s��
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Worker::CTemplate<Poling::CWorker>
		{
		public:
			// �R���X�g���N�^
			CWorker();

			// �f�X�g���N�^
			~CWorker() override;
		};
	}
}

