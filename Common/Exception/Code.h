#pragma once

namespace Standard
{
	namespace Exception
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @enum			��O�R�[�h
		////////////////////////////////////////////////////////////////////////////////
		enum EnumCode
		{
			CodeSuccess = 0,							//! ����
#pragma region �o�[�W����(0x7000�`)
			CodeVersion = 0x7000,						//! �o�[�W�����̎擾�Ɏ��s
#pragma endregion
#pragma region ���O(0x7100�`0x71FF)
			CodeTransferInitialized = 0x7100,			//! ���O�]���̏����������{�ς�
			CodeTransferDiscarded = 0x7101,				//! ���O�]���̔j�������{�ς�
#pragma endregion
#pragma region �J����(0x7200�`0x72FF)
			CodeGrabberDeviceLost = 0x7200,				//! �J�����̐ؒf
			CodeGrabberInitialized = 0x7201,			//! �J�����̏����������{�ς�
			CodeGrabberUninitialized = 0x7202,			//! �J�����̖�������
			CodeGrabberDiscarded = 0x7203,				//! �J�����̔j�������{�ς�
			CodeGrabberInterface = 0x7204,				//! �J�����̃C���^�[�t�F�[�X�Ȃ�
			CodeGrabberDataStream = 0x7205,				//! �J�����̃f�[�^�X�g���[���Ȃ�
			CodeGrabberDevice = 0x7206,					//! �J�����̐ڑ��Ȃ�
			CodeGrabberInitialize = 0x7210,				//! �J�����̏������Ɏ��s
			CodeGrabberDestroy = 0x7220,				//! �J�����̔j���Ɏ��s
			CodeGrabberConnect = 0x7230,				//! �J�����̐ڑ��Ɏ��s
			CodeGrabberDisconnect = 0x7240,				//! �J�����̐ؒf�Ɏ��s
			CodeGrabberParameterGet = 0x7250,			//! �J�����̃p�����[�^�擾�Ɏ��s
			CodeGrabberParameterSet = 0x7260,			//! �J�����̃p�����[�^�X�V�Ɏ��s
			CodeGrabberCapture = 0x7270,				//! �J�����̎�荞�݂Ɏ��s
			CodeGrabberCaptureNotStart = 0x7271,		//! �J�����̎�荞�݂������{
			CodeGrabberCaptureTimeout = 0x7272,			//! �J�����̎�荞�݃^�C���A�E�g
			CodeGrabberCaptureNotImage = 0x7273,		//! �J�����̎�荞�݂ŉ摜�f�[�^�Ȃ�
			CodeGrabberCaptureNotFormat = 0x7274,		//! �J�����̎�荞�݂ŉ摜�t�H�[�}�b�g���Ή�
#pragma endregion
#pragma region IO(0x7300�`0x73FF)
			CodeIoInitialized = 0x7301,					//! IO�̏����������{�ς�
			CodeIoUninitialized = 0x7302,				//! IO�̖�������
			CodeIoDiscarded = 0x7303,					//! IO�̔j�������{�ς�
			CodeIoInitialize = 0x7310,					//! IO�̏������Ɏ��s
			CodeIoDestroy = 0x7320,						//! IO�̔j���Ɏ��s
			CodeIoConnect = 0x7330,						//! IO�̐ڑ��Ɏ��s
			CodeIoDisconnect = 0x7340,					//! IO�̐ؒf�Ɏ��s
			CodeIoRead = 0x7350,						//! IO�̓ǂݍ��݂Ɏ��s
			CodeIoWrite = 0x7360,						//! IO�̏������݂Ɏ��s
#pragma endregion
#pragma region ��(0x7400�`0x74FF)
			CodeAxisInitialized = 0x7401,				//! ���̏����������{�ς�
			CodeAxisUninitialized = 0x7402,				//! ���̖�������
			CodeAxisDiscarded = 0x7403,					//! ���̔j�������{�ς�
			CodeAxisInitialize = 0x7410,				//! ���̏������Ɏ��s
			CodeAxisDestroy = 0x7420,					//! ���̔j���Ɏ��s
			CodeAxisConnect = 0x7430,					//! ���̐ڑ��Ɏ��s
			CodeAxisDisconnect = 0x7440,				//! ���̐ؒf�Ɏ��s
			CodeAxisRead = 0x7450,						//! ���̏�Ԋm�F�Ɏ��s
			CodeAxisWrite = 0x7460,						//! ���̐���Ɏ��s
#pragma endregion
#pragma region Plc(0x7500�`0x75FF)
			CodePlcInitialized = 0x7501,				//! PLC�̏����������{�ς�
			CodePlcUninitialized = 0x7502,				//! PLC�̖�������
			CodePlcDiscarded = 0x7503,					//! PLC�̔j�������{�ς�
			CodePlcInitialize = 0x7510,					//! PLC�̏������Ɏ��s
			CodePlcDestroy = 0x7520,					//! PLC�̔j���Ɏ��s
			CodePlcConnect = 0x7530,					//! PLC�̐ڑ��Ɏ��s
			CodePlcDisconnect = 0x7540,					//! PLC�̐ؒf�Ɏ��s
			CodePlcRead = 0x7550,						//! PLC�̏�Ԋm�F�Ɏ��s
			CodePlcWrite = 0x7560,						//! PLC�̐���Ɏ��s
#pragma endregion
#pragma region �摜(0x9000�`0x901F)
			CodeImageUpdateDepth = 0x9000,				//! �摜�̍X�V�Ɏ��s(�r�b�g�[�x���ُ�)
			CodeImageUpdateHeight = 0x9001,				//! �摜�̍X�V�Ɏ��s(�������ُ�)
			CodeImageUpdateWidth = 0x9002,				//! �摜�̍X�V�Ɏ��s(�����ُ�)
			CodeImageUpdateMat = 0x9003,				//! �摜�̍X�V�Ɏ��s(Mat����)
			CodeImageConvertEmpty = 0x9004,				//! �摜�̕ϊ��Ɏ��s(�摜�f�[�^����)
#pragma endregion
#pragma region Udp(0x9020�`0x902F)
			CodeUdpConnect = 0x9020,					//! Udp�̐ڑ��Ɏ��s
			CodeUdpWrite = 0x9021,						//! Udp�̑��M�Ɏ��s
#pragma endregion
			CodeUnknown = 0x9999						//! �s���ȃG���[
		};

		////////////////////////////////////////////////////////////////////////////////
		/// @enum			�@��̗�O�R�[�h
		////////////////////////////////////////////////////////////////////////////////
		enum EnumDeviceCode
		{
			DeviceCodeSuccess = 0,			//! ����
		};
	}
}