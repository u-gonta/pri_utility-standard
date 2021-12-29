#pragma once

#include "Axis/Poling.h"

namespace Standard
{
	namespace Axis
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      ���̐���A���̊Ď��A��Ԃ�ʒm����N���X
			///				�� ���̐���A���̊Ď��A��Ԃ�ʒm����e���v���[�g�̃N���X����p��
			/// @detail		���̐���A���̊Ď��A��Ԃ̒ʒm���s��
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

				// �ǂݍ���
				void ReadDerived() override;

				// ��������
				void WriteDerived() override;

				// ������ʒm
				template <typename CWrite, typename CArgument>
				void NotifyComplete(std::shared_ptr<CArgument> object, bool remove = true);
			};
		}
	}
}