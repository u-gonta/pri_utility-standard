#pragma once

#include "Common/Logging/Basis.h"

namespace Standard
{
	namespace Logging
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			ログのクラスに統合 ※インライン関数
		/// @detail			ログのクラスに追加の文字列を付加する
		/// @param[in]		logging	ログのクラス
		/// @param[in]		postscript	追加の文字列
		////////////////////////////////////////////////////////////////////////////////
		inline Logging::CObject Join(const Logging::CObject& logging, const std::string& postscript)
		{
			Logging::CObject ret(logging);

			// 追加の文字列が空か確認
			if (postscript.empty() == false)
			{
				// 追加の文字列を結合
				ret.Message << Logging::ConstSeparator << postscript;
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			例外をログのクラスに統合 ※インライン関数
		/// @detail			ログのクラスに追加の文字列と例外のメッセージを付加する
		/// @param[in]		logging	ログのクラス
		/// @param[in]		postscript	追加の文字列
		////////////////////////////////////////////////////////////////////////////////
		inline Logging::CObject Exception(const Logging::CObject& logging, const std::string& postscript)
		{
			Logging::CObject ret = Join(logging, postscript);

			ret.Level = Logging::LevelError;

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			例外をログのクラスに統合 ※インライン関数
		/// @detail			ログのクラスに追加の文字列と例外のメッセージを付加する
		/// @param[in]		logging	ログのクラス
		/// @param[in]		postscript	追加の文字列
		/// @param[in]		e	例外の文字列
		////////////////////////////////////////////////////////////////////////////////
		inline Logging::CObject Exception(const Logging::CObject& logging, const std::string& postscript, const std::string& e)
		{
			Logging::CObject ret = Join(logging, postscript);

			ret.Level = Logging::LevelError;
			ret.Message << Logging::ConstSeparator << e;

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			例外をログのクラスに統合 ※インライン関数
		/// @detail			ログのクラスに追加の文字列と例外のメッセージを付加する
		/// @param[in]		logging	ログのクラス
		/// @param[in]		postscript	追加の文字列
		/// @param[in]		e	例外
		////////////////////////////////////////////////////////////////////////////////
		inline Logging::CObject Exception(const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
		{
			return Exception(logging, postscript, e.what());
		}
	}
}