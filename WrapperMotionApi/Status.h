#pragma once

#include "Common/Iterator/Basis.h"

// 1�o�C�g�A���C�������g�ɕύX
#pragma pack(1)

namespace Standard
{
	namespace MotionApi
	{
		namespace Status
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @enum			�^�]�X�e�[�^�X
			////////////////////////////////////////////////////////////////////////////////
			enum EnumRunning
			{
				RunningReady = 0x00,				// �^�]��������
				RunningServo = 0x01,				// �^�]��(�T�[�{ON)
				RunningServoReady = 0x03,			// �T�[�{���f�B
				RunningOrigin = 0xFF				// ���_���A����
			};
			typedef Iterator::CWorker<EnumRunning, RunningReady, RunningServoReady> RunningIterator;

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			���[�j���O
			////////////////////////////////////////////////////////////////////////////////
			enum EnumWarning
			{
				WarningDeviation = 0x00,			// �΍��ُ�
				WarningSettingParameter = 0x01,		// �ݒ�p�����[�^�ُ�
				WarningFixParameter = 0x02,			// �Œ�p�����[�^�ُ�
				WarningDriver = 0x03,				// �T�[�{�h���C�o�ُ�
				WarningCommand = 0x04,				// ���[�V�����R�}���h�ݒ�ُ�
				WarningServoUnfinished = 0x08,		// �T�[�{ON����
				WarningServoCommunication = 0x09,	// �T�[�{�h���C�o�ʐM�x��
				WarningServoStop = 0x0A				// �T�[�{�h���C�o��~�M�����͒�
			};
			typedef Iterator::CWorker<EnumWarning, WarningDeviation, WarningServoStop> WarningIterator;

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			�A���[��
			////////////////////////////////////////////////////////////////////////////////
			enum EnumAlarm
			{
				AlarmDriver = 0x00,					// �T�[�{�h���C�o�ُ�
				AlarmPositiveSoftLimit = 0x03,		// �������\�t�g���~�b�g
				AlarmNegativeSoftLimit = 0x04,		// �������\�t�g���~�b�g
				AlarmServoOff = 0x05,				// �T�[�{OFF
				AlarmPositionTimeover = 0x06,		// �ʒu���߃^�C���I�[�o�[
				AlarmExcessivePosition = 0x07,		// �ʒu���߈ړ��ʉߑ�
				AlarmExcessiveSpeed = 0x08,			// ���x�ߑ�
				AlarmDeviation = 0x09				// �΍��ُ�
			};
			typedef Iterator::CWorker<EnumAlarm, AlarmDriver, AlarmDeviation> AlarmIterator;

			////////////////////////////////////////////////////////////////////////////////
			/// @enum			�ʒu
			////////////////////////////////////////////////////////////////////////////////
			enum EnumPosition
			{
				PositionPayoutCompleted = 0x00,		// �����o������
				PositionCompleted = 0x01,			// �ʒu���ߊ���
				PositionOriginCompleted = 0x05		// ���_���A����
			};
			typedef Iterator::CWorker<EnumPosition, PositionPayoutCompleted, PositionOriginCompleted> PositionIterator;
		}
	}
}

// �o�C�g�A���C�������g��߂�
#pragma pack()