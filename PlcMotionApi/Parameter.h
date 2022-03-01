#pragma once

#include "Common/Iterator/Basis.h"

namespace Standard
{
	namespace Plc
	{
		//! �p�����[�^�̒�`
		enum EnumParameter
		{
			ParameterOpenController,		// �R���g���[�����J���ݒ�N���X
			ParameterApiTimeout,			// Api�^�C���A�E�g�̐ݒ�N���X
			ParameterDeclareAxis,			// ���̃n���h�����쐬����ݒ�N���X
			ParameterSuspendTimeout,		// ����̒�~�^�C���A�E�g[ms]
			ParameterServoTimeout,			// �T�[�{����̃^�C���A�E�g[ms]
			ParameterOriginAxis,			// ���̌��_���A��ݒ肷��N���X
			ParameterMoveOriginTimeout,		// ���_���A�̃^�C���A�E�g[ms]
			ParameterStartJog,				// �W���O�̊J�n��ݒ肷��N���X
			ParameterStopJog				// �W���O�̒�~��ݒ肷��N���X
		};
	}
}
