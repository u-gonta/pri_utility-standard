#pragma once

#include "Common/Tool/Text.h"
#include "Common/Tool/Logging.h"
#include "Common/Utility/StopWatch.h"
#include "Exception/Convert.h"
#include "Transfer/Transfer.h"
#include "WrapperMotionApi/include/ErrorCode.h"
#include "WrapperMotionApi/Exception.h"
#include "WrapperMotionApi/WrapperMotionApi.h"
#include "WrapperMotionApi/Result.h"
#include "WrapperMotionApi/Message.h"

#pragma comment(lib, "WrapperMotionApi.lib")

namespace Standard
{
	namespace Plc
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			ハンドルを文字列へ変換
		/// @param[in]		value ハンドルの値
		/// @return			文字列に変換したハンドル
		////////////////////////////////////////////////////////////////////////////////
		inline std::string FormatHandle(unsigned long value)
		{
			std::string ret = "";

			// ハンドルを文字列へ変換
			ret = Text::FormatX(8, value);

			return ret;
		}
	}
}