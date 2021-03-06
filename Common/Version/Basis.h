#pragma once

#include <string>

namespace Standard
{
	namespace Version
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CObject
		/// @brief      バージョンの情報を保持するクラス
		////////////////////////////////////////////////////////////////////////////////
		class CObject
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CObject()
			{
				FileName = "";
				FileDescription = "";
				Major = 0;
				Minor = 0;
				Build = 0;
				Revision = 0;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デストラクタ
			////////////////////////////////////////////////////////////////////////////////
			~CObject()
			{

			}

		public:
			//! ファイル名
			std::string FileName;

			//! ファイルのバージョン情報
			std::string FileDescription;

			//! バージョン番号(メジャー)
			int Major;

			//! バージョン番号(マイナー)
			int Minor;

			//! バージョン番号(ビルド)
			int Build;

			//! バージョン番号(リビジョン)
			int Revision;
		};
	}
}
