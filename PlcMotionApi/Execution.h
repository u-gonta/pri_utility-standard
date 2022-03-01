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
		/// @brief			�n���h���𕶎���֕ϊ�
		/// @param[in]		value �n���h���̒l
		/// @return			������ɕϊ������n���h��
		////////////////////////////////////////////////////////////////////////////////
		inline std::string FormatHandle(unsigned long value)
		{
			std::string ret = "";

			// �n���h���𕶎���֕ϊ�
			ret = Text::FormatX(8, value);

			return ret;
		}
	}
}