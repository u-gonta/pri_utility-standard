#pragma once

#include <map>

#include "Common/Io/Setting.h"

namespace Standard
{
	namespace Plc
	{
		namespace Register
		{
			//! Sレジスタ
			const char ConstCategoryS = 'S';

			//! Mレジスタ
			const char ConstCategoryM = 'M';

			//! Iレジスタ
			const char ConstCategoryI = 'I';

			//! Oレジスタ
			const char ConstCategoryO = 'O';

			//! Cレジスタ
			const char ConstCategoryC = 'C';

			//! ビット型
			const char ConstTypeBit = 'B';

			//! ワード型
			const char ConstTypeWord = 'W';

			//! ロング型
			const char ConstTypeLong = 'L';

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CSpecify
			/// @brief      レジスタの指定データを保持するクラス
			////////////////////////////////////////////////////////////////////////////////
			class CSpecify
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CSpecify()
				{
					_category = "";
					Index = 0;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				/// @param[in]		種別
				////////////////////////////////////////////////////////////////////////////////
				CSpecify(std::string category) : CSpecify()
				{
					std::stringstream postscript;

					postscript << "種別:" << category;

					// 種別を確認
					if (category.size() < 1)
					{
						// 種別が未指定
						postscript << "が未指定";
						throw std::exception(postscript.str().c_str());
					}

					// 種別を更新
					_category = category;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @return			自クラス
				////////////////////////////////////////////////////////////////////////////////
				CSpecify& operator = (const CSpecify& object) noexcept
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CSpecify()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			データのタイプを変更
				/// @param[in]		type データのタイプ
				////////////////////////////////////////////////////////////////////////////////
				void ChangeType(const char& type)
				{
					std::stringstream postscript;
					std::stringstream category;

					postscript << "種別:" << _category;

					// 種別を取得
					category << GetCategory();
					// データのタイプを確認
					if (ConstTypeBit == GetType())
					{
						// ビット型 ⇒ データのタイプを確認
						if (ConstTypeBit != type)
						{
							// ワード型 or ロング型 ⇒ ビットの桁を削除
							Index >>= 4;
						}
					}
					else
					{
						// ワード型 or ロング型 ⇒ データのタイプを確認
						if (ConstTypeBit == type)
						{
							// ビット型 ⇒ ビットの桁を追加
							Index <<= 4;
						}
					}
					// データのタイプを更新
					category << type;

					// 種別を更新
					_category = category.str();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			種別を取得
				/// @return			種別
				////////////////////////////////////////////////////////////////////////////////
				char GetCategory()
				{
					char ret = ConstCategoryM;

					std::stringstream postscript;

					postscript << "種別:" << _category;

					// 種別を確認
					if (_category.size() < 1)
					{
						// 種別が未指定
						postscript << "が未指定";
						throw std::exception(postscript.str().c_str());
					}

					// 種別を取得
					ret = _category.at(0);

					switch (ret)
					{
					case ConstCategoryS:
					case ConstCategoryM:
					case ConstCategoryI:
					case ConstCategoryO:
					case ConstCategoryC:
						// S、M、I、O、Cレジスタ
						break;

					default:
						// その他
						postscript << "が不適切";
						throw std::exception(postscript.str().c_str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			データのタイプを取得
				/// @return			タイプ
				////////////////////////////////////////////////////////////////////////////////
				char GetType()
				{
					char ret = ConstTypeBit;

					std::stringstream postscript;

					postscript << "種別:" << _category;

					// 種別を確認
					if (_category.size() < 2)
					{
						// データタイプが未指定
						postscript << "でデータのタイプが未指定";
						throw std::exception(postscript.str().c_str());
					}

					// データのタイプを取得
					ret = _category.at(1);

					switch (ret)
					{
					case ConstTypeBit:
					case ConstTypeWord:
					case ConstTypeLong:
						// ビット、ワード、ロング型
						break;

					default:
						// その他
						postscript << "でデータのタイプが不適切";
						throw std::exception(postscript.str().c_str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			データの桁数を取得
				///					※ビットの桁は省く
				/// @return			データの桁数
				////////////////////////////////////////////////////////////////////////////////
				int GetMargin()
				{
					int ret = 5;

					// レジスタの種別を確認
					if (ConstCategoryI == GetCategory() || ConstCategoryO == GetCategory())
					{
						// I、Oレジスタ ⇒ データタイプ以降の4文字をレジスタ番号として取得
						ret = 4;
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			アドレスを取得
				/// @return			アドレス
				////////////////////////////////////////////////////////////////////////////////
				std::string GetAddress()
				{
					std::stringstream ret;
					int margin;

					// 種別を取得
					ret << GetCategory();
					// データのタイプを取得
					ret << GetType();
					// データの桁数を取得
					margin = GetMargin();
					// データのタイプを確認
					if (ConstTypeBit == GetType())
					{
						// ビット型 ⇒ ビットの桁を追加
						margin += 1;
					}
					// 番号を確定
					ret << Text::FormatX(margin, Index, "");

					return ret.str();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CSpecify& object)
				{
					this->_category = object._category;
					this->Index = object.Index;
				}

			private:
				//! 種別
				std::string _category;

			public:
				//! 番号
				unsigned long Index;
			};
		}
	}
}