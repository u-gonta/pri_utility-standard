#pragma once

#include "Execution.h"
#include "SettingAxis.h"
#include "Result.h"

namespace Standard
{
	namespace Plc
	{
		namespace Axis
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸を初期化
			/// @param[in]		object 軸を初期化する設定クラス
			/// @return			軸を初期化した結果クラス
			////////////////////////////////////////////////////////////////////////////////
			inline Result::CInitialize Initialize(const Setting::CInitialize& object)
			{
				Result::CInitialize ret;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Initialize()" << Logging::ConstSeparator;

				try
				{
					// 軸のハンドルを作成する設定クラス
					MotionApi::Axis::Setting::CDeclare setting = object.Declare;

					// 軸のハンドルを作成
					postscript << "MotionApi::DeclareAxis()";
					postscript << Logging::ConstSeparator << "スロットNo:" << object.Declare.SlotNo;
					postscript << Logging::ConstSeparator << "サブスロットNo:" << object.Declare.SubSlotNo;
					postscript << Logging::ConstSeparator << "ステーションNo:" << object.Declare.StationNo;
					postscript << Logging::ConstSeparator << "論理軸No:" << object.Declare.LogicalNo;
					postscript << Logging::ConstSeparator << "軸タイプ:" << object.Declare.Type;
					ret.Axis = MotionApi::DeclareAxis(setting);
					// ログ出力
					postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(ret.Axis.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			軸を破棄
			/// @param[in]		object 軸を破棄する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			inline void Destroy(const Setting::CDestroy& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Destroy()" << Logging::ConstSeparator;

				try
				{
					// 軸のハンドルを破棄
					postscript << "MotionApi::ClearAllAxis()";
					MotionApi::ClearAllAxis();
					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}
		}
	}
}