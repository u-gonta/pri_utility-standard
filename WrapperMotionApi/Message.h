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
			/// @brief			���[�V�����̐ݒ�𕶎���ɕϊ�
			/// @param[in]		object ���[�V�����̐ݒ��ێ�����e���v���[�g�̃N���X
			/// @return			���[�V�����̐ݒ�𕶎���ɕϊ������f�[�^
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(const MotionApi::Device::Setting::Motion::CTemplate& object)
			{
				std::stringstream ret;

				ret << "���W�n:";
				switch (object.Coordinate)
				{
				case MotionApi::Device::CoodinateWork:
					ret << "���[�N";
					break;

				case MotionApi::Device::CoodinateMachine:
					ret << "�@�B";
					break;
				}

				ret << Logging::ConstSeparator << "����:";
				switch (object.MoveType)
				{
				case MotionApi::Device::MoveRelative:
					ret << "����";
					break;

				case MotionApi::Device::MoveAbsolute:
					ret << "���";
					break;

				case MotionApi::Device::MoveRotaryShort:
					ret << "��](�߉��)";
					break;

				case MotionApi::Device::MoveRotaryPositive:
					ret << "��](���])";
					break;

				case MotionApi::Device::MoveRotaryNegative:
					ret << "��](�t�])";
					break;
				}

				ret << Logging::ConstSeparator << "���x:";
				switch (object.VelocityType)
				{
				case MotionApi::Device::VelocityUnit:
					ret << "�w�ߒP��/s";
					break;

				case MotionApi::Device::VelocityPercent:
					ret << "%";
					break;
				}

				ret << Logging::ConstSeparator << "������:";
				switch (object.AccDecType)
				{
				case MotionApi::Device::AccDecUnit:
					ret << "�w�ߒP��/s^2";
					break;

				case MotionApi::Device::AccDecTime:
					ret << "���萔[ms]";
					break;

				case MotionApi::Device::AccDecKeep:
					ret << "���݂̐ݒ�";
					break;
				}

				ret << Logging::ConstSeparator << "�������t�B���^:";
				switch (object.FilterType)
				{
				case MotionApi::Device::FilterCurve:
					ret << "�ړ�����";
					break;

				case MotionApi::Device::FilterExp:
					ret << "�w��";
					break;

				case MotionApi::Device::FilterNothing:
					ret << "�Ȃ�";
					break;

				case MotionApi::Device::FilterKeep:
					ret << "���݂̐ݒ�";
					break;
				}

				ret << Logging::ConstSeparator << "���x�̏���l";
				if (object.MaxVelocity.Indirect)
				{
					ret << "(�Ԑ�):" << "OW" << Text::FormatX(4, object.MaxVelocity.Value, "");
				}
				else
				{
					ret << "(����):" << object.MaxVelocity.Value;
				}

				ret << Logging::ConstSeparator << "�����x";
				if (object.Acceleration.Indirect)
				{
					ret << "(�Ԑ�):" << "OW" << Text::FormatX(4, object.Acceleration.Value, "");
				}
				else
				{
					ret << "(����):" << object.Acceleration.Value;
				}

				ret << Logging::ConstSeparator << "�����x";
				if (object.Deceleration.Indirect)
				{
					ret << "(�Ԑ�):" << "OW" << Text::FormatX(4, object.Deceleration.Value, "");
				}
				else
				{
					ret << "(����):" << object.Deceleration.Value;
				}

				ret << Logging::ConstSeparator << "�t�B���^����";
				if (object.FilterTime.Indirect)
				{
					ret << "(�Ԑ�):" << "OW" << Text::FormatX(4, object.FilterTime.Value, "");
				}
				else
				{
					ret << "(����):" << object.FilterTime.Value;
				}

				ret << Logging::ConstSeparator << "���x";
				if (object.Velocity.Indirect)
				{
					ret << "(�Ԑ�):" << "OW" << Text::FormatX(4, object.Velocity.Value, "");
				}
				else
				{
					ret << "(����):" << object.Velocity.Value;
				}

				ret << Logging::ConstSeparator << "���_���A�̃A�v���[�`���x";
				if (object.ApproachVelocity.Indirect)
				{
					ret << "(�Ԑ�):" << "OW" << Text::FormatX(4, object.ApproachVelocity.Value, "");
				}
				else
				{
					ret << "(����):" << object.ApproachVelocity.Value;
				}

				ret << Logging::ConstSeparator << "���_���A�̃N���[�v���x";
				if (object.CreepVelocity.Indirect)
				{
					ret << "(�Ԑ�):" << "OW" << Text::FormatX(4, object.CreepVelocity.Value, "");
				}
				else
				{
					ret << "(����):" << object.CreepVelocity.Value;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�|�W�V�����̐ݒ�𕶎���ɕϊ�
			/// @param[in]		object �|�W�V�����̐ݒ��ێ�����e���v���[�g�̃N���X
			/// @return			�|�W�V�����̐ݒ�𕶎���ɕϊ������f�[�^
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(const MotionApi::Device::Setting::Position::CTemplate& object)
			{
				std::stringstream ret;

				ret << "�ʒu";
				switch (object.PositionType)
				{
				case MotionApi::Device::PositionImmediate:
					ret << "(����):" << object.PositionData;
					break;

				case MotionApi::Device::PositionIndirect:
					ret << "(�Ԑ�):" << "OW" << Text::FormatX(4, object.PositionData, "");
					break;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���_���A�̕����𕶎���ɕϊ�
			/// @param[in]		object ���_���A�̕���
			/// @return			���_���A�̕����𕶎���ɕϊ������f�[�^
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(MotionApi::Device::EnumOrigin object)
			{
				std::stringstream ret;

				ret << Logging::ConstSeparator << "���_���A:";
				switch (object)
				{
				case MotionApi::Device::OriginDec1_C:
					ret << "DEC1 + C���p���X����";
					break;

				case MotionApi::Device::OriginZero:
					ret << "ZERO�M������";
					break;

				case MotionApi::Device::OriginDec1_Zero:
					ret << "DEC1 + ZERO�M������";
					break;

				case MotionApi::Device::OriginC:
					ret << "C���p���X����";
					break;

				case MotionApi::Device::OriginDec2_Zero:
					ret << "DEC2 + ZERO�M������";
					break;

				case MotionApi::Device::OriginDec1_L_Zero:
					ret << "DEC1 + LMT + ZERO�M������";
					break;

				case MotionApi::Device::OriginDec2_C:
					ret << "DEC2 + C���M������";
					break;

				case MotionApi::Device::OriginDec1_L_C:
					ret << "DEC1 + LMT + C���M������";
					break;

				case MotionApi::Device::OriginC_Only:
					ret << "�VC���p���X����";
					break;

				case MotionApi::Device::OriginPot_C:
					ret << "POT & C���p���X����";
					break;

				case MotionApi::Device::OriginPot_Only:
					ret << "POT����";
					break;

				case MotionApi::Device::OriginHomeLs_C:
					ret << "HOME LS & C���p���X����";
					break;

				case MotionApi::Device::OriginHomeLs_Only:
					ret << "HOME LS����";
					break;

				case MotionApi::Device::OriginNot_C:
					ret << "NOT & C���p���X����";
					break;

				case MotionApi::Device::OriginNot_Only:
					ret << "NOT����";
					break;

				case MotionApi::Device::OriginInput_C:
					ret << "INPUT & C���p���X����";
					break;

				case MotionApi::Device::OriginInput_Only:
					ret << "INPUT����";
					break;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�ړ��̕����𕶎���ɕϊ�
			/// @param[in]		object �ړ��̕���
			/// @return			�ړ��̕����𕶎���ɕϊ������f�[�^
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(MotionApi::Device::EnumDirection object)
			{
				std::stringstream ret;

				ret << Logging::ConstSeparator << "�ړ�:";
				switch (object)
				{
				case MotionApi::Device::DirectionPositive:
					ret << "������";
					break;

				case MotionApi::Device::DirectionNegative:
					ret << "�t����";
					break;
				}

				return ret.str();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�����̑����𕶎���ɕϊ�
			/// @param[in]		object �����̑���
			/// @return			�����̑����𕶎���ɕϊ������f�[�^
			////////////////////////////////////////////////////////////////////////////////
			inline std::string Convert(MotionApi::Device::EnumComplete object)
			{
				std::stringstream ret;

				ret << Logging::ConstSeparator << "�����̑���:";
				switch (object)
				{
				case MotionApi::Device::CompleteDistribution:
					ret << "�����o������";
					break;

				case MotionApi::Device::CompletePositioning:
					ret << "�ʒu���ߊ���";
					break;

				case MotionApi::Device::CompleteStart:
					ret << "�w�ߊJ�n";
					break;
				}

				return ret.str();
			}
		}
	}
}