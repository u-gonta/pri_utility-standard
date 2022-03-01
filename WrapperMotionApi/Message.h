#pragma once

#include "Common/Logging/Basis.h"
#include "SettingDevice.h"

namespace Standard
{
	namespace MotionApi
	{
		namespace Message
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			モーションの設定を文字列に変換
			/// @param[in]		object モーションの設定を保持するテンプレートのクラス
			/// @return			モーションの設定を文字列に変換したデータ
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(const MotionApi::Device::Setting::Motion::CTemplate& object)
			{
				std::stringstream ret;

				ret << "座標系:";
				switch (object.Coordinate)
				{
				case MotionApi::Device::CoodinateWork:
					ret << "ワーク";
					break;

				case MotionApi::Device::CoodinateMachine:
					ret << "機械";
					break;
				}

				ret << Logging::ConstSeparator << "動作:";
				switch (object.MoveType)
				{
				case MotionApi::Device::MoveRelative:
					ret << "相対";
					break;

				case MotionApi::Device::MoveAbsolute:
					ret << "絶対";
					break;

				case MotionApi::Device::MoveRotaryShort:
					ret << "回転(近回り)";
					break;

				case MotionApi::Device::MoveRotaryPositive:
					ret << "回転(正転)";
					break;

				case MotionApi::Device::MoveRotaryNegative:
					ret << "回転(逆転)";
					break;
				}

				ret << Logging::ConstSeparator << "速度:";
				switch (object.VelocityType)
				{
				case MotionApi::Device::VelocityUnit:
					ret << "指令単位/s";
					break;

				case MotionApi::Device::VelocityPercent:
					ret << "%";
					break;
				}

				ret << Logging::ConstSeparator << "加減速:";
				switch (object.AccDecType)
				{
				case MotionApi::Device::AccDecUnit:
					ret << "指令単位/s^2";
					break;

				case MotionApi::Device::AccDecTime:
					ret << "時定数[ms]";
					break;

				case MotionApi::Device::AccDecKeep:
					ret << "現在の設定";
					break;
				}

				ret << Logging::ConstSeparator << "加減速フィルタ:";
				switch (object.FilterType)
				{
				case MotionApi::Device::FilterCurve:
					ret << "移動平均";
					break;

				case MotionApi::Device::FilterExp:
					ret << "指数";
					break;

				case MotionApi::Device::FilterNothing:
					ret << "なし";
					break;

				case MotionApi::Device::FilterKeep:
					ret << "現在の設定";
					break;
				}

				ret << Logging::ConstSeparator << "速度の上限値";
				if (object.MaxVelocity.Indirect)
				{
					ret << "(間接):" << "OW" << Text::FormatX(4, object.MaxVelocity.Value, "");
				}
				else
				{
					ret << "(直接):" << object.MaxVelocity.Value;
				}

				ret << Logging::ConstSeparator << "加速度";
				if (object.Acceleration.Indirect)
				{
					ret << "(間接):" << "OW" << Text::FormatX(4, object.Acceleration.Value, "");
				}
				else
				{
					ret << "(直接):" << object.Acceleration.Value;
				}

				ret << Logging::ConstSeparator << "減速度";
				if (object.Deceleration.Indirect)
				{
					ret << "(間接):" << "OW" << Text::FormatX(4, object.Deceleration.Value, "");
				}
				else
				{
					ret << "(直接):" << object.Deceleration.Value;
				}

				ret << Logging::ConstSeparator << "フィルタ時間";
				if (object.FilterTime.Indirect)
				{
					ret << "(間接):" << "OW" << Text::FormatX(4, object.FilterTime.Value, "");
				}
				else
				{
					ret << "(直接):" << object.FilterTime.Value;
				}

				ret << Logging::ConstSeparator << "速度";
				if (object.Velocity.Indirect)
				{
					ret << "(間接):" << "OW" << Text::FormatX(4, object.Velocity.Value, "");
				}
				else
				{
					ret << "(直接):" << object.Velocity.Value;
				}

				ret << Logging::ConstSeparator << "原点復帰のアプローチ速度";
				if (object.ApproachVelocity.Indirect)
				{
					ret << "(間接):" << "OW" << Text::FormatX(4, object.ApproachVelocity.Value, "");
				}
				else
				{
					ret << "(直接):" << object.ApproachVelocity.Value;
				}

				ret << Logging::ConstSeparator << "原点復帰のクリープ速度";
				if (object.CreepVelocity.Indirect)
				{
					ret << "(間接):" << "OW" << Text::FormatX(4, object.CreepVelocity.Value, "");
				}
				else
				{
					ret << "(直接):" << object.CreepVelocity.Value;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ポジションの設定を文字列に変換
			/// @param[in]		object ポジションの設定を保持するテンプレートのクラス
			/// @return			ポジションの設定を文字列に変換したデータ
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(const MotionApi::Device::Setting::Position::CTemplate& object)
			{
				std::stringstream ret;

				ret << "位置";
				switch (object.PositionType)
				{
				case MotionApi::Device::PositionImmediate:
					ret << "(直接):" << object.PositionData;
					break;

				case MotionApi::Device::PositionIndirect:
					ret << "(間接):" << "OW" << Text::FormatX(4, object.PositionData, "");
					break;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			原点復帰の方式を文字列に変換
			/// @param[in]		object 原点復帰の方式
			/// @return			原点復帰の方式を文字列に変換したデータ
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(MotionApi::Device::EnumOrigin object)
			{
				std::stringstream ret;

				ret << Logging::ConstSeparator << "原点復帰:";
				switch (object)
				{
				case MotionApi::Device::OriginDec1_C:
					ret << "DEC1 + C相パルス方式";
					break;

				case MotionApi::Device::OriginZero:
					ret << "ZERO信号方式";
					break;

				case MotionApi::Device::OriginDec1_Zero:
					ret << "DEC1 + ZERO信号方式";
					break;

				case MotionApi::Device::OriginC:
					ret << "C相パルス方式";
					break;

				case MotionApi::Device::OriginDec2_Zero:
					ret << "DEC2 + ZERO信号方式";
					break;

				case MotionApi::Device::OriginDec1_L_Zero:
					ret << "DEC1 + LMT + ZERO信号方式";
					break;

				case MotionApi::Device::OriginDec2_C:
					ret << "DEC2 + C相信号方式";
					break;

				case MotionApi::Device::OriginDec1_L_C:
					ret << "DEC1 + LMT + C相信号方式";
					break;

				case MotionApi::Device::OriginC_Only:
					ret << "新C相パルス方式";
					break;

				case MotionApi::Device::OriginPot_C:
					ret << "POT & C相パルス方式";
					break;

				case MotionApi::Device::OriginPot_Only:
					ret << "POT方式";
					break;

				case MotionApi::Device::OriginHomeLs_C:
					ret << "HOME LS & C相パルス方式";
					break;

				case MotionApi::Device::OriginHomeLs_Only:
					ret << "HOME LS方式";
					break;

				case MotionApi::Device::OriginNot_C:
					ret << "NOT & C相パルス方式";
					break;

				case MotionApi::Device::OriginNot_Only:
					ret << "NOT方式";
					break;

				case MotionApi::Device::OriginInput_C:
					ret << "INPUT & C相パルス方式";
					break;

				case MotionApi::Device::OriginInput_Only:
					ret << "INPUT方式";
					break;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			移動の方向を文字列に変換
			/// @param[in]		object 移動の方向
			/// @return			移動の方向を文字列に変換したデータ
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(MotionApi::Device::EnumDirection object)
			{
				std::stringstream ret;

				ret << Logging::ConstSeparator << "移動:";
				switch (object)
				{
				case MotionApi::Device::DirectionPositive:
					ret << "正方向";
					break;

				case MotionApi::Device::DirectionNegative:
					ret << "逆方向";
					break;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			完了の属性を文字列に変換
			/// @param[in]		object 完了の属性
			/// @return			完了の属性を文字列に変換したデータ
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(MotionApi::Device::EnumComplete object)
			{
				std::stringstream ret;

				ret << Logging::ConstSeparator << "完了の属性:";
				switch (object)
				{
				case MotionApi::Device::CompleteDistribution:
					ret << "払い出し完了";
					break;

				case MotionApi::Device::CompletePositioning:
					ret << "位置決め完了";
					break;

				case MotionApi::Device::CompleteStart:
					ret << "指令開始";
					break;
				}

				return ret.str();
			}
		}
	}
}