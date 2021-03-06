#pragma once

#include <vector>

#include "boost/any.hpp"

namespace Standard
{
	namespace Variant
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CObject
		/// @brief      任意の要素とそれに紐づく値を保持するクラス
		////////////////////////////////////////////////////////////////////////////////
		class CObject
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CObject()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			/// @param[in]		TElement	要素の型(テンプレートで指定)
			/// @param[in]		TValue	値の型(テンプレートで指定)
			/// @param[in]		element	要素
			/// @param[in]		value	値
			////////////////////////////////////////////////////////////////////////////////
			template<typename TElement, typename TValue>
			CObject(const TElement& element, const TValue& value)
			{
				this->Element = element;
				this->Value = value;
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
				this->Element = object.Element;
				this->Value = object.Value;
			}

		public:
			//! 要素
			boost::any Element;

			//! 値
			boost::any Value;
		};

		////////////////////////////////////////////////////////////////////////////////
		/// @class      CManager
		/// @brief      任意の要素とそれに紐づく値を保持するクラスを管理するクラス
		/// @detail		任意の型へ変換なども行う
		////////////////////////////////////////////////////////////////////////////////
		class CManager
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CManager()
			{
				Objects.clear();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コピー代入演算子
			/// @return			自クラス
			////////////////////////////////////////////////////////////////////////////////
			CManager& operator = (const CManager& object) noexcept
			{
				// 更新
				Update(object);

				return *this;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デストラクタ
			////////////////////////////////////////////////////////////////////////////////
			~CManager()
			{

			}

		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			要素が一致した値を返す
			/// @detail			テンプレートで要素の型を指定
			/// @param[in]		TElement	要素の型(テンプレートで指定)
			/// @param[in]		element	要素
			/// @return			要素が一致した値 ※不一致の場合はempty状態になっている
			////////////////////////////////////////////////////////////////////////////////
			template <typename TElement>
			boost::any GetValue(const boost::any& element)
			{
				boost::any ret;

				// 情報を走査
				for (const auto& object : Objects)
				{
					// 要素の一致を確認
					if (IsMatch<TElement>(element, object.Element) == false)
					{
						// 不一致
						continue;
					}

					// 一致
					ret = object.Value;
					break;
				}

				return ret;
			}

		protected:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			自クラスを更新
			/// @param[in]		objects	自クラス
			////////////////////////////////////////////////////////////////////////////////
			void Update(const CManager& object)
			{
				this->Objects.clear();
				std::copy(object.Objects.begin(), object.Objects.end(), std::back_inserter(this->Objects));
			}

		private:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			要素の一致を確認
			/// @detail			テンプレートで要素の型を指定
			/// @param[in]		TElement	要素の型(テンプレートで指定)
			/// @param[in]		first	比較元の要素
			/// @param[in]		second	比較先の要素
			/// @return			true:一致 / false:不一致
			////////////////////////////////////////////////////////////////////////////////
			template <typename TElement>
			bool IsMatch(const boost::any& first, const boost::any& second)
			{
				bool ret = false;

				do
				{
					// 要素の空を確認
					if (first.empty() | second.empty())
					{
						// 空
						break;
					}

					// 要素の型を確認
					if (first.type() != second.type())
					{
						// 不一致
						break;
					}

					// 要素の値を比較
					if (boost::any_cast<TElement>(first) != boost::any_cast<TElement>(second))
					{
						// 不一致
						break;
					}

					// 一致
					ret = true;
				} while (false);

				return ret;
			}

		public:
			//! 任意の要素とそれに紐づく値を保持するクラス
			std::vector<Variant::CObject> Objects;
		};
	}
}