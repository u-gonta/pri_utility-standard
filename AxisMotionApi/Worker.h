#pragma once

#include "Axis/Worker.h"
#include "Poling.h"

namespace Standard
{
	namespace Axis
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      ���̐�����s���N���X
		///				�� ���̊Ď��A���̐���A��Ԃ�ʒm����e���v���[�g�̃N���X����h��
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Axis::Worker::CTemplate<Poling::CWorker>
		{
		public:
			// �R���X�g���N�^
			CWorker();

			// �f�X�g���N�^
			~CWorker() override;
		};
	}
}

