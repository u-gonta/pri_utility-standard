#pragma once

#include <string>
#include "cereal\archives\json.hpp"

namespace Standard
{
	namespace Archive
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��������N���X�ɕϊ�
		/// @param[in]		title	�N���X�̃^�C�g��
		/// @param[in]		object	�ϊ����镶����
		////////////////////////////////////////////////////////////////////////////////
		template<class T>
		inline const T& Input(const std::string& title, const std::string& object)
		{
			T ret;

			std::stringstream buffer;

			buffer << object;

			cereal::JSONInputArchive archive(buffer);

			// �f�V���A���C�Y
			archive(cereal::make_nvp(title, ret));

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�N���X�𕶎���ɕϊ�
		/// @param[in]		title	�N���X�̃^�C�g��
		/// @param[in]		object	�ϊ�����N���X
		////////////////////////////////////////////////////////////////////////////////
		template<class T>
		inline std::string Output(const std::string& title, const T& object)
		{
			std::stringstream ret;

			{
				cereal::JSONOutputArchive archive(ret);

				// �V���A���C�Y
				archive(cereal::make_nvp(title, object));
			}

			return ret.str();
		}
	}
}