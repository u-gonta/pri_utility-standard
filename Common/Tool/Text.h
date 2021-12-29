#pragma once

#include <iomanip>
#include <sstream>
#include <vector>
#include <tchar.h>
#include <codecvt>

namespace Standard
{
	namespace Text
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			wstring -> stringへ変換 ※インライン関数
		/// @param[in]		value	元データ
		/// @return         string型に変換された文字列
		////////////////////////////////////////////////////////////////////////////////
		inline std::string Convert(const std::wstring& value)
		{
			std::string ret = "";

			std::size_t converted{};
			std::vector<char> dest(value.size() * sizeof(wchar_t) + 1, '\0');
			_locale_t locale = ::_create_locale(LC_ALL, "jpn");
			if (::_wcstombs_s_l(&converted, dest.data(), dest.size(), value.data(), _TRUNCATE, locale) != 0)
			{
				::_free_locale(locale);
				throw std::system_error{ errno, std::system_category() };
			}
			::_free_locale(locale);
			dest.resize(std::char_traits<char>::length(dest.data()));
			dest.shrink_to_fit();
			ret = std::string(dest.begin(), dest.end());

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			string -> wstringへ変換 ※インライン関数
		/// @param[in]		value	元データ
		/// @return         wstring型に変換された文字列
		////////////////////////////////////////////////////////////////////////////////
		inline std::wstring Convert(const std::string& value)
		{
			std::wstring ret;//@@@ = _T("");

			std::size_t converted{};
			std::vector<wchar_t> dest(value.size() + 1, L'\0');
			_locale_t locale = ::_create_locale(LC_ALL, "jpn");
			if (::_mbstowcs_s_l(&converted, dest.data(), dest.size(), value.data(), _TRUNCATE, locale) != 0)
			{
				::_free_locale(locale);
				throw std::system_error{ errno, std::system_category() };
			}
			::_free_locale(locale);
			dest.resize(std::char_traits<wchar_t>::length(dest.data()));
			dest.shrink_to_fit();
			ret = std::wstring(dest.begin(), dest.end());

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			整数値を10進数文字列へ変換 ※インライン関数
		/// @detail			左側を桁数で0埋めした文字列を構築する
		/// @param[in]		margin	桁数
		/// @param[in]		value	整数値
		/// @return         フォーマットされた文字列
		////////////////////////////////////////////////////////////////////////////////
		inline std::string Format(int margin, int value)
		{
			std::stringstream ret;

			ret << std::setw(margin) << std::setfill('0') << value;

			return ret.str();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			整数値を10進数文字列へ変換 ※インライン関数
		/// @detail			左側を桁数で0埋めした文字列を構築する
		/// @param[in]		margin	桁数
		/// @param[in]		value	整数値
		/// @return         フォーマットされた文字列
		////////////////////////////////////////////////////////////////////////////////
		inline std::string Format(unsigned long margin, unsigned long value)
		{
			std::stringstream ret;

			ret << std::setw(margin) << std::setfill('0') << value;

			return ret.str();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			整数値を16進数文字列へ変換 ※インライン関数
		/// @detail			左側を桁数で0埋めした文字列を構築する
		/// @param[in]		margin	桁数
		/// @param[in]		value	整数値
		/// @return         フォーマットされた文字列
		////////////////////////////////////////////////////////////////////////////////
		inline std::string FormatX(int margin, int value, std::string header = "0x")
		{
			std::stringstream ret;

			ret << header << std::setw(margin) << std::setfill('0') << std::hex << value;

			return ret.str();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			実数値を文字列へ変換 ※インライン関数
		/// @detail			小数点以下の桁数だけ揃えた文字列を構築する
		/// @param[in]		decimal	桁数
		/// @param[in]		value	実数値
		/// @return         フォーマットされた文字列
		////////////////////////////////////////////////////////////////////////////////
		inline std::string Format(int decimal, double value)
		{
			std::stringstream ret;

			ret << std::fixed << std::setprecision(decimal) << value;

			return ret.str();
		}
	}
}