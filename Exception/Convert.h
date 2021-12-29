#pragma once

#include "Common/Exception/Basis.h"
#include "Common/Logging/Basis.h"
#include "Common/Tool/Text.h"

namespace Standard
{
	namespace Exception
	{
		namespace Convert
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���b�Z�[�W�ɕϊ�
			/// @detail			��O�̏���t���������b�Z�[�W�𐶐�����
			/// @param[in]		logging	���O�̃N���X
			/// @param[in]		postscript	�ǉ��̕�����
			/// @param[in]		e	��O�̃N���X
			/// @return			���b�Z�[�W
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Message(const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
			{
				std::stringstream ret;

				// ���b�Z�[�W��ǉ�
				ret << logging.Message.str();

				// �ǉ��̕�������m�F
				if (postscript.empty() == false)
				{
					// �ǉ��̕������ǉ�
					ret << Logging::ConstSeparator << postscript;
				}

				std::string buffer = e.what();

				// �G���[�̃��b�Z�[�W���m�F
				if (buffer.empty() == false)
				{
					// �G���[�̃��b�Z�[�W��ǉ�
					ret << Logging::ConstSeparator << buffer;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���b�Z�[�W�ɕϊ�
			/// @detail			��O�̏���t���������b�Z�[�W�𐶐�����
			/// @param[in]		errorCode	��O�̃R�[�h
			/// @param[in]		deviceErrorCode	�@��̃G���[�R�[�h
			/// @param[in]		logging	���O�̃N���X
			/// @param[in]		postscript	�ǉ��̕�����
			/// @param[in]		e	��O�̃N���X
			/// @return			���b�Z�[�W
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Message(Exception::EnumCode errorCode, int deviceErrorCode, const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
			{
				std::stringstream ret;

				// �G���[�R�[�h��t��
				ret << "�G���[:" << Text::FormatX(4, errorCode);

				// ���O�̃N���X�ɕϊ�
				ret << Logging::ConstSeparator << Message(logging, postscript, e);

				// �@��̃G���[�R�[�h���m�F
				if (deviceErrorCode != Exception::DeviceCodeSuccess)
				{
					// �@��̃G���[�R�[�h��ǉ�
					ret << Logging::ConstSeparator << "�R�[�h:" << Text::FormatX(8, deviceErrorCode);
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���O�̃N���X�ɕϊ�
			/// @detail			��O�̏���t���������O�̃N���X�𐶐�����
			/// @param[in]		logging	���O�̃N���X
			/// @param[in]		postscript	�ǉ��̕�����
			/// @param[in]		e	��O�̃N���X
			/// @return			���O�̃N���X
			////////////////////////////////////////////////////////////////////////////////
			inline Logging::CObject Logging(const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
			{
				Logging::CObject ret;

				// ���O�̃��x�����X�V
				ret.Level = Logging::LevelError;

				// ���b�Z�[�W��ǉ�
				ret.Message << Message(logging, postscript, e);

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���O�̃N���X�ɕϊ�
			/// @detail			��O�̏���t���������O�̃N���X�𐶐�����
			/// @param[in]		errorCode	��O�̃R�[�h
			/// @param[in]		deviceErrorCode	�@��̃G���[�R�[�h
			/// @param[in]		logging	���O�̃N���X
			/// @param[in]		postscript	�ǉ��̕�����
			/// @param[in]		e	��O�̃N���X
			/// @return			���O�̃N���X
			////////////////////////////////////////////////////////////////////////////////
			inline Logging::CObject Logging(Exception::EnumCode errorCode, int deviceErrorCode, const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
			{
				Logging::CObject ret;

				// �G���[�R�[�h��t��
				ret.Message << "�G���[:" << Text::FormatX(4, errorCode);

				// ���O�̃N���X�ɕϊ�
				ret = Logging(logging, postscript, e);

				// �@��̃G���[�R�[�h���m�F
				if (deviceErrorCode != Exception::DeviceCodeSuccess)
				{
					// �@��̃G���[�R�[�h��ǉ�
					ret.Message << Logging::ConstSeparator << "�R�[�h:" << Text::FormatX(8, deviceErrorCode);
				}

				return ret;
			}
		}
	}
}