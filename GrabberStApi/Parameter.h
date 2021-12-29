#pragma once

#include "Common/Grabber/Condition.h"

#include "StApi_TL.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CParameter
		/// @brief      カメラのパラメータを制御するクラス
		////////////////////////////////////////////////////////////////////////////////
		class CParameter
		{
		public:
			// コンストラクタ
			CParameter();

			// デストラクタ
			~CParameter();

		public:
			// 利用可能を確認
			void IsAvailable(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

			// 読み込み可能を確認
			void IsRead(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

			// 書き込み可能を確認
			void IsWrite(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

		public:
			// 設定を選択
			void Selector(const GenApi::INodeMap& nodeMap, const std::string& item, const std::string& value) throw(...);

			// 読み込み(整数値)
			int64_t ReadInteger(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

			// 読み込み(実数値)
			double ReadDouble(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

			// 書き込み(文字列)
			void WriteText(const GenApi::INodeMap& nodeMap, const std::string& item, const std::string& value) throw(...);

			// 書き込み(整数値)
			int64_t WriteInteger(const GenApi::INodeMap& nodeMap, const std::string& item, const int64_t value) throw(...);

			// 書き込み(実数値)
			double WriteDouble(const GenApi::INodeMap& nodeMap, const std::string& item, const double value) throw(...);

		public:
			// 範囲を読み込み(実数)
			Condition::CRangeDouble ReadRange(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);

		public:
			// 実行
			void Execute(const GenApi::INodeMap& nodeMap, const std::string& item) throw(...);
		};
	}
}
