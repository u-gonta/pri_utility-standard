#pragma once

#include <string>
#include "cereal\archives\json.hpp"

namespace Standard
{
	namespace Archive
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			文字列をクラスに変換
		/// @param[in]		title	クラスのタイトル
		/// @param[in]		object	変換する文字列
		////////////////////////////////////////////////////////////////////////////////
		template<class T>
		inline const T& Input(const std::string& title, const std::string& object)
		{
			T ret;

			std::stringstream buffer;

			buffer << object;

			cereal::JSONInputArchive archive(buffer);

			// デシリアライズ
			archive(cereal::make_nvp(title, ret));

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			クラスを文字列に変換
		/// @param[in]		title	クラスのタイトル
		/// @param[in]		object	変換するクラス
		////////////////////////////////////////////////////////////////////////////////
		template<class T>
		inline std::string Output(const std::string& title, const T& object)
		{
			std::stringstream ret;

			{
				cereal::JSONOutputArchive archive(ret);

				// シリアライズ
				archive(cereal::make_nvp(title, object));
			}

			return ret.str();
		}
	}
}