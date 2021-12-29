#pragma once

#include "Logging.h"
#include "Text.h"

namespace Standard
{
	namespace Exception
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��O���f�o�b�O�o�� ���C�����C���֐�
		/// @detail			���O�̃N���X�ɒǉ��̕�����Ɨ�O�̃��b�Z�[�W��t�����ăf�o�b�O�o�͂���
		/// @param[in]		logging	���O�̃N���X
		/// @param[in]		postscript	�ǉ��̕�����
		/// @param[in]		e	��O
		////////////////////////////////////////////////////////////////////////////////
		inline void OutputDebug(const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
		{
			Logging::CObject buffer = Logging::Exception(logging, postscript, e);

			// �f�o�b�O�o��
			::OutputDebugString(Text::Convert(buffer.Message.str()).c_str());
		}
	}
}
