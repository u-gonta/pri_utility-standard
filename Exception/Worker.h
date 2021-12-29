#pragma once

#include <mutex>

#include "Common/Exception/Basis.h"
#include "Common/Logging/Basis.h"
#include "Common/Tool/Text.h"
#include "Common/Tool/Logging.h"
#include "Transfer/Transfer.h"

#pragma comment(lib, "Transfer.lib")

#include "Notice/Worker.h"

namespace Standard
{
	namespace Exception
	{
		namespace Worker
		{
			//! 例外を受信する関数の定義
			typedef void(OnException)(const Exception::CObject&);

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      例外の制御を行うテンプレートのクラス
			////////////////////////////////////////////////////////////////////////////////
			class CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate()
				{
					// 名称を更新
					m_noticeException.SetName("Exception::Notice::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{

				}

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外の名称を更新
				/// @param[in]		name	例外の名称
				////////////////////////////////////////////////////////////////////////////////
				void SetNameException(const std::string& name)
				{
					// 名称を更新
					m_noticeException.SetName(name);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外を通知する関数の登録
				/// @param[in]		object	例外を通知する関数
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionException(std::function<OnException> object)
				{
					// 例外を通知する関数の登録
					m_noticeException.AttachFunctionCalled(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外を通知する関数の解除
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionException()
				{
					// 例外を通知する関数の解除
					m_noticeException.DetachFunctionCalled();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外を通知
				/// @param[in]		object	例外のクラス
				////////////////////////////////////////////////////////////////////////////////
				void NotifyException(const Exception::CObject& object)
				{
					// 例外を通知
					m_noticeException.RequestNotice(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外の出力
				/// @detail			ログの出力と例外のクラスを生成する
				/// @param[in]		errorCode	例外のコード
				/// @param[in]		deviceErrorCode	機器のエラーコード
				/// @param[in]		logging	ログのクラス
				/// @param[in]		postscript	追加の文字列
				/// @param[in]		e	例外のクラス
				/// @return			例外のクラス
				////////////////////////////////////////////////////////////////////////////////
				Exception::CObject OutputException(Exception::EnumCode errorCode, int deviceErrorCode, const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
				{
					Exception::CObject ret;

					Logging::CObject buffer;

					// ログのクラスに変換
					buffer = Exception::Convert::Logging(errorCode, deviceErrorCode, logging, postscript, e);

					// 例外のクラスを更新
					ret.ErrorCode = errorCode;
					ret.DeviceErrorCode = deviceErrorCode;
					ret.Message = buffer.Message.str();

					// ログ出力
					Transfer::Output(buffer);

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外の出力
				/// @detail			ログの出力と例外のクラスを生成する
				/// @param[in]		errorCode	例外のコード
				/// @param[in]		logging	ログのクラス
				/// @param[in]		postscript	追加の文字列
				/// @return			例外のクラス
				////////////////////////////////////////////////////////////////////////////////
				Exception::CObject OutputException(Exception::EnumCode errorCode, const Logging::CObject& logging, const std::string& postscript)
				{
					// 例外の出力
					return OutputException(errorCode, Exception::CodeSuccess, logging, postscript, std::exception(""));
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外の出力
				/// @detail			ログの出力と例外のクラスを生成する
				/// @param[in]		errorCode	例外のコード
				/// @param[in]		deviceErrorCode	機器のエラーコード
				/// @param[in]		postscript	追加の文字列
				/// @return			例外のクラス
				////////////////////////////////////////////////////////////////////////////////
				Exception::CObject OutputException(Exception::EnumCode errorCode, int deviceErrorCode, const std::string& postscript)
				{
					Logging::CObject logging;

					logging.Message << postscript;

					// 例外の出力
					return OutputException(errorCode, deviceErrorCode, logging, "", std::exception(""));
				}

			private:
				//! 例外を通知するクラス
				Notice::Worker::CTemplate<OnException, Exception::CObject> m_noticeException;
			};
		}
	}
}