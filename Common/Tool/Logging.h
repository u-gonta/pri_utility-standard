#pragma once

#include "Common/Logging/Basis.h"

namespace Standard
{
	namespace Logging
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���O�̃N���X�ɓ��� ���C�����C���֐�
		/// @detail			���O�̃N���X�ɒǉ��̕������t������
		/// @param[in]		logging	���O�̃N���X
		/// @param[in]		postscript	�ǉ��̕�����
		////////////////////////////////////////////////////////////////////////////////
		inline Logging::CObject Join(const Logging::CObject& logging, const std::string& postscript)
		{
			Logging::CObject ret(logging);

			// �ǉ��̕����񂪋󂩊m�F
			if (postscript.empty() == false)
			{
				// �ǉ��̕����������
				ret.Message << Logging::ConstSeparator << postscript;
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��O�����O�̃N���X�ɓ��� ���C�����C���֐�
		/// @detail			���O�̃N���X�ɒǉ��̕�����Ɨ�O�̃��b�Z�[�W��t������
		/// @param[in]		logging	���O�̃N���X
		/// @param[in]		postscript	�ǉ��̕�����
		////////////////////////////////////////////////////////////////////////////////
		inline Logging::CObject Exception(const Logging::CObject& logging, const std::string& postscript)
		{
			Logging::CObject ret = Join(logging, postscript);

			ret.Level = Logging::LevelError;

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��O�����O�̃N���X�ɓ��� ���C�����C���֐�
		/// @detail			���O�̃N���X�ɒǉ��̕�����Ɨ�O�̃��b�Z�[�W��t������
		/// @param[in]		logging	���O�̃N���X
		/// @param[in]		postscript	�ǉ��̕�����
		/// @param[in]		e	��O�̕�����
		////////////////////////////////////////////////////////////////////////////////
		inline Logging::CObject Exception(const Logging::CObject& logging, const std::string& postscript, const std::string& e)
		{
			Logging::CObject ret = Join(logging, postscript);

			ret.Level = Logging::LevelError;
			ret.Message << Logging::ConstSeparator << e;

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��O�����O�̃N���X�ɓ��� ���C�����C���֐�
		/// @detail			���O�̃N���X�ɒǉ��̕�����Ɨ�O�̃��b�Z�[�W��t������
		/// @param[in]		logging	���O�̃N���X
		/// @param[in]		postscript	�ǉ��̕�����
		/// @param[in]		e	��O
		////////////////////////////////////////////////////////////////////////////////
		inline Logging::CObject Exception(const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
		{
			return Exception(logging, postscript, e.what());
		}
	}
}