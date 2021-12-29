#pragma once

#include "Common/Exception/Basis.h"
#include "Common/Logging/Basis.h"
#include "Common/Tool/Text.h"

namespace Standard
{
	namespace Exception
	{
		namespace Convert
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			メッセージに変換
			/// @detail			例外の情報を付加したメッセージを生成する
			/// @param[in]		logging	ログのクラス
			/// @param[in]		postscript	追加の文字列
			/// @param[in]		e	例外のクラス
			/// @return			メッセージ
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Message(const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
			{
				std::stringstream ret;

				// メッセージを追加
				ret << logging.Message.str();

				// 追加の文字列を確認
				if (postscript.empty() == false)
				{
					// 追加の文字列を追加
					ret << Logging::ConstSeparator << postscript;
				}

				std::string buffer = e.what();

				// エラーのメッセージを確認
				if (buffer.empty() == false)
				{
					// エラーのメッセージを追加
					ret << Logging::ConstSeparator << buffer;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			メッセージに変換
			/// @detail			例外の情報を付加したメッセージを生成する
			/// @param[in]		errorCode	例外のコード
			/// @param[in]		deviceErrorCode	機器のエラーコード
			/// @param[in]		logging	ログのクラス
			/// @param[in]		postscript	追加の文字列
			/// @param[in]		e	例外のクラス
			/// @return			メッセージ
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Message(Exception::EnumCode errorCode, int deviceErrorCode, const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
			{
				std::stringstream ret;

				// エラーコードを付加
				ret << "エラー:" << Text::FormatX(4, errorCode);

				// ログのクラスに変換
				ret << Logging::ConstSeparator << Message(logging, postscript, e);

				// 機器のエラーコードを確認
				if (deviceErrorCode != Exception::DeviceCodeSuccess)
				{
					// 機器のエラーコードを追加
					ret << Logging::ConstSeparator << "コード:" << Text::FormatX(8, deviceErrorCode);
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ログのクラスに変換
			/// @detail			例外の情報を付加したログのクラスを生成する
			/// @param[in]		logging	ログのクラス
			/// @param[in]		postscript	追加の文字列
			/// @param[in]		e	例外のクラス
			/// @return			ログのクラス
			////////////////////////////////////////////////////////////////////////////////
			inline Logging::CObject Logging(const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
			{
				Logging::CObject ret;

				// ログのレベルを更新
				ret.Level = Logging::LevelError;

				// メッセージを追加
				ret.Message << Message(logging, postscript, e);

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ログのクラスに変換
			/// @detail			例外の情報を付加したログのクラスを生成する
			/// @param[in]		errorCode	例外のコード
			/// @param[in]		deviceErrorCode	機器のエラーコード
			/// @param[in]		logging	ログのクラス
			/// @param[in]		postscript	追加の文字列
			/// @param[in]		e	例外のクラス
			/// @return			ログのクラス
			////////////////////////////////////////////////////////////////////////////////
			inline Logging::CObject Logging(Exception::EnumCode errorCode, int deviceErrorCode, const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
			{
				Logging::CObject ret;

				// エラーコードを付加
				ret.Message << "エラー:" << Text::FormatX(4, errorCode);

				// ログのクラスに変換
				ret = Logging(logging, postscript, e);

				// 機器のエラーコードを確認
				if (deviceErrorCode != Exception::DeviceCodeSuccess)
				{
					// 機器のエラーコードを追加
					ret.Message << Logging::ConstSeparator << "コード:" << Text::FormatX(8, deviceErrorCode);
				}

				return ret;
			}
		}
	}
}