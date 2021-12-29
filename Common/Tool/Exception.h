#pragma once

#include "Logging.h"
#include "Text.h"

namespace Standard
{
	namespace Exception
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			例外をデバッグ出力 ※インライン関数
		/// @detail			ログのクラスに追加の文字列と例外のメッセージを付加してデバッグ出力する
		/// @param[in]		logging	ログのクラス
		/// @param[in]		postscript	追加の文字列
		/// @param[in]		e	例外
		////////////////////////////////////////////////////////////////////////////////
		inline void OutputDebug(const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
		{
			Logging::CObject buffer = Logging::Exception(logging, postscript, e);

			// デバッグ出力
			::OutputDebugString(Text::Convert(buffer.Message.str()).c_str());
		}
	}
}
