#pragma once

#include "Common/Logging/Basis.h"

namespace Standard
{
	namespace Transfer
	{
		namespace Setting
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CObject
			/// @brief      転送を行う設定クラス
			////////////////////////////////////////////////////////////////////////////////
			class CObject
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CObject()
				{
					IpAddress = "127.0.0.1";
					Port = 50001;
					Lower = Logging::LevelInfomation;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @return			自クラス
				////////////////////////////////////////////////////////////////////////////////
				CObject& operator = (const CObject& object) noexcept
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CObject()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CObject& object)
				{
					this->IpAddress = object.IpAddress;
					this->Port = object.Port;
					this->Lower = object.Lower;
				}

			public:
				//! 転送先のIPアドレス
				std::string IpAddress;

				//! 転送先のポート
				unsigned short Port;

				//! 出力する下限レベル
				Logging::EnumLevel Lower;
			};
		}
	}
}