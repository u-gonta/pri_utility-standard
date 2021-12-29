#pragma once

#include "boost/any.hpp"

#include "Common/Logging/Basis.h"

namespace Standard
{
	namespace Variant
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			値の型が一致しているか確認
		/// @param[in]		TValue	値の型(テンプレートで指定)
		/// @param[in]		value	値
		/// @return			true:一致 / false:不一致
		////////////////////////////////////////////////////////////////////////////////
		template <typename TValue>
		inline bool IsValue(const boost::any& value)
		{
			bool ret = false;

			do
			{
				// 値の空を確認
				if (value.empty())
				{
					// 空
					break;
				}

				// 型を確認
				if (value.type() != typeid(TValue))
				{
					// 不一致
					break;
				}

				// 一致
				ret = true;
			} while (false);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			int型が一致しているか確認
		/// @param[in]		value	値
		/// @return			true:一致 / false:不一致
		////////////////////////////////////////////////////////////////////////////////
		inline bool IsIntValue(const boost::any& value)
		{
			return IsValue<int>(value);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			値を変換
		/// @param[in]		TValue	値の型(テンプレートで指定)
		/// @param[in]		value	値
		/// @return			変換した値
		////////////////////////////////////////////////////////////////////////////////
		template <typename TValue>
		inline TValue Convert(const boost::any& value)
		{
			TValue ret;

			// 変換
			ret = boost::any_cast<TValue>(value);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			値をint型へ変換
		/// @param[in]		value	値
		/// @return			int型に変換された値
		////////////////////////////////////////////////////////////////////////////////
		inline int ConvertInt(const boost::any& value)
		{
			return Convert<int>(value);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			値の型が一致しているか確認
		/// @param[in]		source	比較元
		/// @param[in]		destination	比較先
		/// @return			true:一致 / false:不一致
		////////////////////////////////////////////////////////////////////////////////
		inline bool Equal(const boost::any& source, const boost::any& destination)
		{
			bool ret = false;

			do
			{
				// 値の空を確認
				if (source.empty() || destination.empty())
				{
					// 空
					std::stringstream message;
					message << "データが空";
					message << Logging::ConstSeparator << "比較元:" << (source.empty() ? "" : source.type().name());
					message << Logging::ConstSeparator << "比較先:" << (destination.empty() ? "" : destination.type().name());
					throw std::exception(message.str().c_str());
				}

				// 値の型を確認
				if (source.type() != destination.type())
				{
					// 型が違う
					std::stringstream message;
					message << "データの型が不一致";
					message << Logging::ConstSeparator << "比較元:" << source.type().name();
					message << Logging::ConstSeparator << "比較先:" << destination.type().name();
					throw std::exception(message.str().c_str());
				}

				// 型を確認
				if (source.type() == typeid(bool))
				{
					// ビット型 ⇒ 値を比較
					if (Convert<bool>(source) != Convert<bool>(destination))
					{
						// 不一致
						break;
					}
				}
				else if (source.type() == typeid(__int16))
				{
					// 整数(2バイト)型 ⇒ 値を比較
					if (Convert<__int16>(source) != Convert<__int16>(destination))
					{
						// 不一致
						break;
					}
				}
				else if (source.type() == typeid(__int32))
				{
					// 整数(4バイト)型 ⇒ 値を比較
					if (Convert<__int32>(source) != Convert<__int32>(destination))
					{
						// 不一致
						break;
					}
				}
				else if (source.type() == typeid(__int64))
				{
					// 整数(8バイト)型 ⇒ 値を比較
					if (Convert<__int64>(source) != Convert<__int64>(destination))
					{
						// 不一致
						break;
					}
				}
				else if (source.type() == typeid(float))
				{
					// 単精度浮動小数点実数型 ⇒ 値を比較
					if (Convert<float>(source) != Convert<float>(destination))
					{
						// 不一致
						break;
					}
				}
				else if (source.type() == typeid(double))
				{
					// 倍精度浮動小数点実数型 ⇒ 値を比較
					if (Convert<double>(source) != Convert<double>(destination))
					{
						// 不一致
						break;
					}
				}

				// 一致
				ret = true;
			} while (false);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			値の型を文字列へ変換
		/// @param[in]		value	値
		/// @return			文字列に変換された値の型
		////////////////////////////////////////////////////////////////////////////////
		inline std::string FormatType(const boost::any& value)
		{
			std::stringstream ret;

			// 型を確認
			if (value.type() == typeid(bool))
			{
				ret << "ビット";
			}
			else if (value.type() == typeid(__int16))
			{
				ret << "整数(2バイト)";
			}
			else if (value.type() == typeid(__int32))
			{
				ret << "整数(4バイト)";
			}
			else if (value.type() == typeid(__int64))
			{
				ret << "整数(8バイト)";
			}
			else if (value.type() == typeid(float))
			{
				ret << "単精度浮動小数点実数";
			}
			else if (value.type() == typeid(double))
			{
				ret << "倍精度浮動小数点実数";
			}
			else
			{
				ret << "未対応(" << value.type().name() << ")";
			}

			return ret.str();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			値を文字列へ変換
		/// @param[in]		value	値
		/// @return			文字列に変換された値
		////////////////////////////////////////////////////////////////////////////////
		inline std::string FormatValue(const boost::any& value)
		{
			std::stringstream ret;

			// 型を確認
			if (value.type() == typeid(bool))
			{
				// ビット
				ret << Variant::Convert<bool>(value);
			}
			else if (value.type() == typeid(__int16))
			{
				// 整数(2バイト)
				ret << Variant::Convert<__int16>(value);
			}
			else if (value.type() == typeid(__int32))
			{
				// 整数(4バイト)
				ret << Variant::Convert<__int32>(value);
			}
			else if (value.type() == typeid(__int64))
			{
				// 整数(8バイト)
				ret << Variant::Convert<__int64>(value);
			}
			else if (value.type() == typeid(float))
			{
				// 単精度浮動小数点実数
				ret << Variant::Convert<float>(value);
			}
			else if (value.type() == typeid(double))
			{
				// 倍精度浮動小数点実数
				ret << Variant::Convert<double>(value);
			}
			else
			{
				ret << "未対応(" << value.type().name() << ")";
				throw std::exception(ret.str().c_str());
			}

			return ret.str();
		}
	}
}