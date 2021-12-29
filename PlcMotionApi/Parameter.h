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
			ParameterServoTimeout,			// �T�[�{����̃^�C���A�E�g[ms]
			ParameterStartJog,				// ���̃W���O�����s����ݒ�N���X
			ParameterStopJog				// ���̃W���O���~����ݒ�N���X
		};
	}
}
