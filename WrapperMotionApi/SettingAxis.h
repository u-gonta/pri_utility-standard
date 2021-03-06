#pragma once

#include "Identifier.h"

// 1バイトアラインメントに変更
#pragma pack(1)

namespace Standard
{
	namespace MotionApi
	{
		namespace Axis
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @enum			軸のタイプ
			////////////////////////////////////////////////////////////////////////////////
			enum EnumAxis
			{
				AxisReal = 1,		// 実サーボ軸
				AxisVirtual = 2,	// 仮想サーボ軸
				AxisExternal = 3	// 外部エンコーダ
			};

			namespace Setting
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CDeclare
				/// @brief      軸のハンドルを作成する設定クラス
				///				※MP3100のSVC(実物)：スロット=0,サブスロット=3
				///				※MP3100のSVR(仮想)：スロット=0,サブスロット=4
				////////////////////////////////////////////////////////////////////////////////
				class CDeclare
				{
				private:
					//! シリアライズのタイトル
					const std::string ConstHeader = "Declare";

				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コンストラクタ
					////////////////////////////////////////////////////////////////////////////////
					CDeclare()
					{
						SlotNo = 0;
						SubSlotNo = 3;
						StationNo = 0;
						LogicalNo = 0;
						Type = AxisReal;
						Address = 0;
						Resolution = 1;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コピー代入演算子
					/// @return			自クラス
					////////////////////////////////////////////////////////////////////////////////
					CDeclare& operator = (const CDeclare& object) noexcept
					{
						// 更新
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			デストラクタ
					////////////////////////////////////////////////////////////////////////////////
					~CDeclare()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			シリアライズ
					////////////////////////////////////////////////////////////////////////////////
					friend class cereal::access;
					template<class T>
					void serialize(T& archive)
					{
						archive(cereal::make_nvp("SlotNo", SlotNo));
						archive(cereal::make_nvp("SubSlotNo", SubSlotNo));
						archive(cereal::make_nvp("StationNo", StationNo));
						archive(cereal::make_nvp("LogicalNo", LogicalNo));
						archive(cereal::make_nvp("Axis", Type));
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			Json形式にシリアライズした文字列から更新
					/// @param[in]		object	Json形式にシリアライズした文字列
					////////////////////////////////////////////////////////////////////////////////
					void Input(std::string object)
					{
						// Json形式から変換
						Update(Archive::Input<CDeclare>(ConstHeader, object));
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			Json形式にシリアライズした文字列を取得
					/// @return			シリアライズした文字列
					////////////////////////////////////////////////////////////////////////////////
					std::string Output()
					{
						std::string ret;

						// Json形式に変換
						ret = Archive::Output<CDeclare>(ConstHeader, *this);

						return ret;
					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			自クラスを更新
					/// @param[in]		object	自クラス
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CDeclare& object)
					{
						this->SlotNo = object.SlotNo;
						this->SubSlotNo = object.SubSlotNo;
						this->StationNo = object.StationNo;
						this->LogicalNo = object.LogicalNo;
						this->Type = object.Type;
						this->Address = object.Address;
						this->Resolution = object.Resolution;
					}

				public:
					//! スロット番号
					unsigned short SlotNo;

					//! サブスロット番号
					unsigned short SubSlotNo;

					//! 軸が定義されているステーション番号
					unsigned short StationNo;

					//! 軸の論理番号
					unsigned short LogicalNo;

					//! 軸のタイプ
					EnumAxis Type;

					//! レジスタの先頭アドレス
					unsigned short Address;

					//! 位置の分解能
					double Resolution;
				};

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CClear
				/// @brief      軸のハンドルを破棄する設定クラス
				///				⇒ 識別子を保持するクラスから派生
				////////////////////////////////////////////////////////////////////////////////
				class CClear
					: virtual public CIdentifier
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コンストラクタ
					////////////////////////////////////////////////////////////////////////////////
					CClear()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コピー代入演算子
					/// @return			自クラス
					////////////////////////////////////////////////////////////////////////////////
					CClear& operator = (const CClear& object) noexcept
					{
						// 更新
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			デストラクタ
					////////////////////////////////////////////////////////////////////////////////
					~CClear()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			自クラスを更新
					/// @param[in]		object	自クラス
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CClear& object)
					{
						// 既定の関数
						CIdentifier::Update(object);
					}
				};
			}
		}
	}
}

// バイトアラインメントを戻す
#pragma pack()