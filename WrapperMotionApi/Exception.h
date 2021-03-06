#pragma once

#include <string>
#include <exception>

#include "Common/Logging/Basis.h"
#include "Common/Tool/Text.h"

#include "include/ymcPCAPI.h"

namespace Standard
{
	namespace MotionApi
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CException
		/// @brief      例外の情報を保持するクラス
		///				⇒ std::exceptionから派生
		////////////////////////////////////////////////////////////////////////////////
		class CException
			: virtual public std::exception
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CException() : std::exception("")
			{
				ErrorCode = MP_SUCCESS;
				Message = "";
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			/// @param[in]		errorCode	エラーコード
			////////////////////////////////////////////////////////////////////////////////
			CException(int errorCode) : std::exception("")
			{
				// 更新
				Update(errorCode, "");
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			/// @param[in]		errorCode	エラーコード
			/// @param[in]		message	メッセージ
			////////////////////////////////////////////////////////////////////////////////
			CException(int errorCode, const std::string& message) : std::exception(message.c_str())
			{
				std::stringstream buffer;

				// エラーコードとメッセージを結合
				buffer << message;
				//@@@buffer << ":" << Text::FormatX(8, errorCode);

				// 更新
				Update(errorCode, buffer.str());
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コピー代入演算子
			/// @return			自クラス
			////////////////////////////////////////////////////////////////////////////////
			CException& operator = (const CException& object) noexcept
			{
				// 更新
				Update(object.ErrorCode, object.Message);

				return *this;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デストラクタ
			////////////////////////////////////////////////////////////////////////////////
			~CException()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			メッセージを取得
			///					★継承した関数
			/// @return			メッセージ
			////////////////////////////////////////////////////////////////////////////////
			virtual const char* what() const throw()
			{
				return Message.c_str();
			}

		private:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			自クラスを更新
			/// @param[in]		errorCode	エラーコード
			/// @param[in]		message	メッセージ
			////////////////////////////////////////////////////////////////////////////////
			void Update(int errorCode, const std::string& message)
			{
				this->ErrorCode = errorCode;
				this->Message = message;
			}

		public:
			//! エラーコード
			int ErrorCode;

			//! メッセージ
			std::string Message;
		};
	}
}