/************************************************************************

        ErrorCode.h

        COPYRIGHT (C) YASKAWA ELECTRIC CORPORATION

*************************************************************************/


#ifndef _ERROR_CODE_H
#define _ERROR_CODE_H

/************************************************************************/
/*                                                                      */
/*              << Error code definition >>                             */
/*                                                                      */
/*      File name   :  ErrorCode.h                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/


#define	MP_SUCCESS							0x00000000		/*	API�֐� ����I��									*/
															/*	API function normal completion						*/
#define	MP_FAIL								0x4000ffff		/*	API�֐� �ُ�I��									*/
															/*	API function erroneous completion					*/
#define	WDT_OVER_ERR						0x81000001		/*														*/
															/*														*/
#define	MANUAL_RESET_ERR					0x82000020		/*	Manual reset error									*/
															/*														*/
#define	TLB_MLTHIT_ERR						0x82000140		/*	TLB multi hit error									*/
															/*														*/
#define	UBRK_ERR							0x820001E0		/*	User break execution error							*/
															/*														*/
#define	ADR_RD_ERR							0x820000E0		/*	Address read error error							*/
															/*														*/
#define	TLB_MIS_RD_ERR						0x82000040		/*	TLB read mis error									*/
															/*														*/
#define	TLB_PROTECTION_RD_ERR				0x820000A0		/*	TLB protection read vaiolation error				*/
															/*														*/
#define	GENERAL_INVALID_INS_ERR				0x82000180		/*	General invalid instruction error					*/
															/*														*/
#define	SLOT_ERR							0x820001A0		/*	Slot invalid instruction error						*/
															/*														*/
#define	GENERAL_FPU_DISABLE_ERR				0x82000800		/*	General FPU disable error							*/
															/*														*/
#define	SLOT_FPU_ERR						0x82000820		/*	Slot FPU exception error							*/
															/*														*/
#define	ADR_WR_ERR							0x82000100		/*	Data address write error error						*/
															/*														*/
#define	TLB_MIS_WR_ERR						0x82000060		/*	TLB write mis error									*/
															/*														*/
#define	TLB_PROTECTION_WR_ERR				0x820000C0		/*	TLB protection write vaiolation error				*/
															/*														*/
#define	FPU_EXP_ERR							0x82000120		/*	FPU exception error									*/
															/*														*/
#define	INITIAL_PAGE_EXP_ERR				0x82000080		/*	Initial page write exception error					*/
															/*														*/
#define	ROM_ERR								0x81000041		/*	ROM  error											*/
															/*														*/
#define	RAM_ERR								0x81000042		/*	RAM  error											*/
															/*														*/
#define	MPU_ERR								0x81000043		/*	CPU  error											*/
															/*														*/
#define	FPU_ERR								0x81000044		/*	FPU  error											*/
															/*														*/
#define	CERF_ERR							0x81000049		/*	CERF error											*/
															/*														*/
#define	EXIO_ERR							0x81000050		/*	EXIO error											*/
															/*														*/
#define	BUSIF_ERR							0x8100005F		/*	Common RAM error for OEM							*/
															/*														*/
#define	ALM_NO_ALM							0x00000000		/*	�A���[���Ȃ����																					*/
															/*	No alarm																							*/
#define	ALM_MK_DEBUG						0x67050300		/*	�y�̏�	�f�o�b�O�p�A���[���R�[�h																	*/
															/*	Minor failure: Alarm code for debugging																*/
#define	ALM_MK_ROUND_ERR					0x67050301		/*	�y�̏�	���a�w���1���~�G���[																		*/
															/*	Minor failure: Improper specification of one cycle at radius specification							*/
#define	ALM_MK_FSPEED_OVER					0x67050302		/*	�y�̏�	���葬�x�I�[�o�[																			*/
															/*	Minor failure: Feeding speed exceeded																*/
#define	ALM_MK_FSPEED_NOSPEC				0x67050303		/*	�y�̏�	���葬�x�w�薳��																			*/
															/*	Minor failure: Feeding speed not specified															*/
#define	ALM_MK_PRM_OVER						0x67050304		/*	�y�̏�	�������p�����[�^�̕ϊ���̒l���͈͊O														*/
															/*	Minor failure: Value after conversion of acceleration or deceleration parameter is out of range.	*/
#define	ALM_MK_ARCLEN_OVER					0x67050305		/*	�y�̏�	�ʂ̒�����LONG_MAX�𒴂���																	*/
															/*	Minor failure: Arc length exceeds LONG_MAX.															*/
#define	ALM_MK_VERT_NOSPEC					0x67050306		/*	�y�̏�	���ʎw��̏c���w�薳��																		*/
															/*	Minor failure: Vertical axis by plane specification not specified									*/
#define	ALM_MK_HORZ_NOSPEC					0x67050307		/*	�y�̏�	���ʎw��̉����w�薳��																		*/
															/*	Minor failure: Horizontal axis by plane specification not specified									*/
#define	ALM_MK_TURN_OVER					0x67050308		/*	�y�̏�	�^�[�����w��I�[�o�[																		*/
															/*	Minor failure: Specified number of turns exceeded													*/
#define	ALM_MK_RADIUS_OVER					0x67050309		/*	�y�̏�	���a��LONG_MAX�𒴂���																		*/
															/*	Minor failure: Radius exceeds LONG_MAX.																*/
#define	ALM_MK_CENTER_ERR					0x6705030A		/*	�y�̏�	���S�_�w��G���[																			*/
															/*	Minor failure: Illegal center point specification													*/
#define	ALM_MK_BLOCK_OVER					0x6705030B		/*	�y�̏�	�������block�ړ��ʃI�[�o�[																	*/
															/*	Minor failure: Linear interpolation block moving amount exceeded									*/
#define	ALM_MK_MAXF_NOSPEC					0x6705030C		/*	�y�̏�	maxf����`																					*/
															/*	Minor failure: maxf not defined																		*/
#define	ALM_MK_TDATA_ERR					0x6705030D		/*	�y�̏�	�A�h���XT�f�[�^�G���[																		*/
															/*	Minor failure: Address T data error																	*/
#define	ALM_MK_REG_ERR						0x6705030E		/*	�y�̏�	REG�f�[�^�G���[ PP�ُ�																		*/
															/*	Minor failure: REG data error and PP fault															*/
#define	ALM_MK_COMMAND_CODE_ERR				0x6705030F		/*	�y�̏�	�R�}���h�͈͊O																				*/
															/*	Minor failure: Out-of-range command																	*/
#define	ALM_MK_AXIS_CONFLICT				0x67050310		/*	�y�̏�	�_�����g�p�֎~��																			*/
															/*	Minor failure: Use of logical axis being prohibited													*/
#define	ALM_MK_POSMAX_OVER					0x67050311		/*	�y�̏�	���������AMVM�܂���ABS�̍��W�w�肪POSMAX�𒴂���											*/
															/*	Minor failure: Infinite length axis, MVM or ABS coordinate designation exceeds POSMAX.				*/
#define	ALM_MK_DIST_OVER					0x67050312		/*	�y�̏�	���̈ړ�������(LONG_MIN,LONG_MAX)�ȊO														*/
															/*	Minor failure: Axis moving distance is other than (LONG_MIN, LONG_MAX).								*/
#define	ALM_MK_MODE_ERR						0x67050313		/*	�y�̏�	���䃂�[�h�s��																				*/
															/*	Minor failure: Illegal control mode																	*/
#define	ALM_MK_CMD_CONFLICT					0x67050314		/*	�y�̏�	���[�V�����R�}���h�d���w��																	*/
															/*	Minor failure: Motion command overlapping instruction												*/
#define	ALM_MK_RCMD_CONFLICT				0x67050315		/*	�y�̏�	���[�V�������X�|���X�R�}���h�d���w��														*/
															/*	Minor failure: Motion response command overlapping instruction										*/
#define	ALM_MK_CMD_MODE_ERR					0x67050316		/*	�y�̏�	���[�V�����R�}���h���[�h�s��																*/
															/*	Minor failure: Illegal motion command mode															*/
#define	ALM_MK_CMD_NOT_ALLOW				0x67050317		/*	�y�̏�	�R�}���h�͂��̃��W���[���ł͎��s�ł��Ȃ�													*/
															/*	Minor failure: Command cannot be executed ih this Module.											*/
#define	ALM_MK_CMD_DEN_FAIL					0x67050318		/*	�y�̏�	�����o��������ԂłȂ�																		*/
															/*	Minor failure: Distribution is not completed.														*/
#define	ALM_MK_H_MOVE_ERR					0x67050319		/*	�y�̏�	hMove�s��																					*/
															/*	Minor failure: Illegal hMove																		*/
#define	ALM_MK_MOVE_NOT_SUPPORT				0x6705031A		/*	�y�̏�	���T�|�[�g��Move�̒�`																		*/
															/*	Minor failure: Non-supported Move defined															*/
#define	ALM_MK_EVENT_NOT_SUPPORT			0x6705031B		/*	�y�̏�	���T�|�[�g��Move Event�̒�`																*/
															/*	Minor failure: Non-supported Move Event defined														*/
#define	ALM_MK_ACTION_NOT_SUPPORT			0x6705031C		/*	�y�̏�	���T�|�[�g��Move Action�̒�`																*/
															/*	Minor failure: Non-supported Move Action defined													*/
#define	ALM_MK_MOVE_TYPE_ERR				0x6705031D		/*	�y�̏�	MoveType�w��G���[																			*/
															/*	Minor failure: MoveType specification error															*/
#define	ALM_MK_VTYPE_ERR					0x6705031E		/*	�y�̏�	VelocityType�w��G���[																		*/
															/*	Minor failure: VelocityType specification error														*/
#define	ALM_MK_ATYPE_ERR					0x6705031F		/*	�y�̏�	AccelerationType�w��G���[																	*/
															/*	Minor failure: AccelerationType specification error													*/
#define	ALM_MK_HOMING_METHOD_ERR			0x67050320		/*	�y�̏�	homing_method�w��G���[																		*/
															/*	Minor failure: Homing_method specification error													*/
#define	ALM_MK_ACC_ERR						0x67050321		/*	�y�̏�	�����x�ݒ�G���[																			*/
															/*	Minor failure: Acceleration setting error															*/
#define	ALM_MK_DEC_ERR						0x67050322		/*	�y�̏�	�����x�ݒ�G���[																			*/
															/*	Minor failure: Deceleration setting error															*/
#define	ALM_MK_POS_TYPE_ERR					0x67050323		/*	�y�̏�	�ʒu�w�߃^�C�v�G���[																		*/
															/*	Minor failure: Position reference type error														*/
#define	ALM_MK_INVALID_EVENT_ERR			0x67050324		/*	�y�̏�	�s��EVENT�^�C�v�G���[																		*/
															/*	Minor failure: Illegal EVENT type																	*/
#define	ALM_MK_INVALID_ACTION_ERR			0x67050325		/*	�y�̏�	�s��ACTION�^�C�v�G���[																		*/
															/*	Minor failure: Illegal ACTION type																	*/
#define	ALM_MK_MOVE_NOT_ACTIVE				0x67050326		/*	�y�̏�	�����s��Move�ɑ΂���Action																	*/
															/*	Minor failure: Action for Move that has not been executed											*/
#define	ALM_MK_MOVELIST_NOT_ACTIVE			0x67050327		/*	�y�̏�	�����s��MoveList�ɑ΂���Action																*/
															/*	Minor failure: Action for MoveList that has not been executed										*/
#define	ALM_MK_TBL_INVALID_DATA				0x67050328		/*	�y�̏�	�e�[�u���f�[�^�s���G���[																	*/
															/*	Minor failure: Illegal table data																	*/
#define	ALM_MK_TBL_INVALID_SEG_NUM			0x67050329		/*	�y�̏�	�e�[�u����� ���s�Z�O�����g���s��															*/
															/*	Minor failure: Illegal number of table interpolation execution segments								*/
#define	ALM_MK_TBL_INVALID_AXIS_NUM			0x6705032A		/*	�y�̏�	�e�[�u����� �����w��G���[																	*/
															/*	Minor failure: Illegal number of table interpolation axes specified									*/
#define	ALM_MK_TBL_INVALID_ST_SEG			0x6705032B		/*	�y�̏�	�e�[�u����� �J�n�Z�O�����gNo�s��															*/
															/*	Minor failure: Illegal table interpolation starting segment number									*/
#define	ALM_MK_STBL_INVALID_EXE				0x6705032C		/*	�y�̏�	Static table�e�[�u�� �t�@�C���������ݒ��̎��s�G���[											*/
															/*	Minor failure: Execution error during Static table file being written								*/
#define	ALM_MK_DTBL_DUPLICATE_EXE			0x6705032D		/*	�y�̏�	Dynamic table�e�[�u�� �Q�d���s�G���[														*/
															/*	Minor failure: Dynamic table duplicated execution error												*/
#define	ALM_MK_LATCH_CONFLICT				0x6705032E		/*	�y�̏�	LATCH�v���d���w�߃G���[																		*/
															/*	Minor failure: LATCH request overlapping instruction error											*/
#define	ALM_MK_INVALID_AXISTYPE				0x6705032F		/*	�y�̏�	���^�C�v�i�L������/���������j�s��															*/
															/*	Minor failure: Illegal axis type (finite length axis/inifinite length axis)							*/
#define	ALM_MK_NOT_SVCRDY					0x67050330		/*	�y�̏�	���[�V�����h���C�o�^�]����������Ԃł�Move�I�u�W�F�N�g���s									*/
															/*	Minor failure: Move object executed when Motion Driver operation is not ready						*/
#define	ALM_MK_NOT_SVCRUN					0x67050331		/*	�y�̏�	�T�[�{�d��������Ԃł�Move�I�u�W�F�N�g���s													*/
															/*	Minor failure: Move object executed at servo OFF													*/
#define	ALM_MK_MDALARM						0x67050332		/*	�y�̏�	���[�V�����h���C�o�A���[����Ԃł�Move�I�u�W�F�N�g���s										*/
															/*	Minor failure: Move object executed at occurrence of Motion Driver alarm							*/
#define	ALM_MK_SUPERPOSE_MASTER_ERR			0x67050333		/*	�y�̏�	�����o�������I�u�W�F�N�g�}�X�^�������G���[													*/
															/*	Minor failure: Distribution synthetic object master axis condition error							*/
#define	ALM_MK_SUPERPOSE_SLAVE_ERR			0x67050334		/*	�y�̏�	�����o�������I�u�W�F�N�g�X���[�u�������G���[												*/
															/*	Minor failure: Distribution synthetic object slave axis condition error								*/
#define	ALM_MK_MDWARNING					0x57050335		/*	ܰ�ݸ�	���[�V�����h���C�o���[�j���O																*/
															/*	Warning: Motion Driver warning																		*/
#define	ALM_MK_MDWARNING_POSERR				0x57050336		/*	ܰ�ݸ�	���[�V�����h���C�o�΍����[�j���O															*/
															/*	Warning: Motion Driver deviation warning															*/
#define	ALM_MK_NOT_INFINITE_ABS				0x67050337		/*	�y�̏�	�w�肳�ꂽ����ABS���������Ƃ��Ďg�p�\�ł͂Ȃ�												*/
															/*	Minor failure: Specified axis cannot be used as ABS infinite length axis.							*/
#define	ALM_MK_INVALID_LOGICAL_NUM			0x67050338		/*	�y�̏�	�s���Ș_�����ԍ����w�肳�ꂽ																*/
															/*	Minor failure: Illegal logical axis number has been specified.										*/
#define	ALM_MK_MAX_VELOCITY_ERR				0x67050339		/*	�y�̏�	�ō����x�ݒ�G���[																			*/
															/*	Minor failure: Maximum speed setting error															*/
#define	ALM_MK_VELOCITY_ERR					0x6705033A		/*	�y�̏�	���x�ݒ�G���[																				*/
															/*	Minor failure: Speed setting error																	*/
#define	ALM_MK_APPROACH_ERR					0x6705033B		/*	�y�̏�	�A�v���[�`���x�ݒ�G���[																	*/
															/*	Minor failure: Approach speed setting error															*/
#define	ALM_MK_CREEP_ERR					0x6705033C		/*	�y�̏�	�N���[�v���x�ݒ�G���[																		*/
															/*	Minor failure: Creep speed setting error															*/
#define	ALM_MK_OS_ERR_MBOX1					0x83050340		/*	�d�̏�	���C���{�b�N�X�����G���[�i���[�V�����J�[�l�� Move�I�u�W�F�N�g���s�v���p���C���{�b�N�X�j		*/
															/*	Major failure: Mail box creation error (mail box for request for Motion Kernel Move object execution)*/
#define	ALM_MK_OS_ERR_MBOX2					0x83050341		/*	�d�̏�	���C���{�b�N�X�����G���[�i���[�V�����J�[�l�� �A�N�V�������s�v���p���C���{�b�N�X�j			*/
															/*	Major failure: Mail box creation error (mail box for request for Motion Kernel action execution)	*/
#define	ALM_MK_OS_ERR_SEND_MSG1				0x83050342		/*	�d�̏�	OS���x���ł̃��b�Z�[�W���M�ُ�iMK��EM�F�C�x���g���o�ʒm�j									*/
															/*	Major failure: Message sending error at OS level (MK to EM: Notification of event detection)		*/
#define	ALM_MK_OS_ERR_SEND_MSG2				0x83050343		/*	�d�̏�	OS���x���ł̃��b�Z�[�W���M�ُ�iMK��MM�FMove�������b�Z�[�W�j								*/
															/*	Major failure: Message sending error at OS level (MK to MM: Move completion message)				*/
#define	ALM_MK_OS_ERR_SEND_MSG3				0x83050344		/*	�d�̏�	OS���x���ł̃��b�Z�[�W���M�ُ�iEM��MM�FAciton�ʒm�j										*/
															/*	Major failure: Message sending error at OS level (EM to MM: Notification of Action)					*/
#define	ALM_MK_OS_ERR_SEND_MSG4				0x83050345		/*	�d�̏�	OS���x���ł̃��b�Z�[�W���M�ُ�i���̑��j													*/
															/*	Major failure: Message sending error at OS level (Others)											*/
#define	ALM_MK_ACTION_ERR1					0x53050346		/*	ܰ�ݸ�	�s���������b�Z�[�W��M�i�A�N�V�������s�����ʒm�̉����҂���Ԃւ̃A�N�V�������s�v���j		*/
															/*	Warning: Illegal response message received (action execution request to waiting status for response of notification of action execution completion)	*/
#define	ALM_MK_ACTION_ERR2					0x53050347		/*	ܰ�ݸ�	�s���������b�Z�[�W��M�i���ݎ��s����Move�I�u�W�F�N�g�ւ̃A�N�V�����ł͂Ȃ��j				*/
															/*	Warning: Illegal response message received(not an action for Move object currently executed)		*/
#define	ALM_MK_ACTION_ERR3					0x53050348		/*	ܰ�ݸ�	�s���������b�Z�[�W��M�i���ݎ��s����MoveLIST�I�u�W�F�N�g�ւ̃A�N�V�����ł͂Ȃ��j			*/
															/*	Warning: Illegal response message received (not an action for MoveLIST object currently executed)	*/
#define	ALM_MK_RCV_INV_MSG1					0x53050349		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�iMOVE�I�u�W�F�N�g�n���h���ł͂Ȃ��j									*/
															/*	Warning: Illegal command message received (not a MOVE object handle)								*/
#define	ALM_MK_RCV_INV_MSG2					0x5305034A		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�i���[�V�����}�l�[�W���ȊO����̎w�߁j								*/
															/*	Warning: Illegal command message received (command from other than Motion Manager)					*/
#define	ALM_MK_RCV_INV_MSG3					0x5305034B		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�i�w�߃��b�Z�[�W�ł͂Ȃ��j											*/
															/*	Warning: Illegal command message received (not a command message)									*/
#define	ALM_MK_RCV_INV_MSG4					0x5305034C		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�i�w�߁^�����ȊO�̃��b�Z�[�W�j										*/
															/*	Warning: Illegal command message received (message other than command/response)						*/
#define	ALM_MK_RCV_INV_MSG5					0x5305034D		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�i�C�x���g�}�l�[�W���ȊO����̎w�߃��b�Z�[�W�j						*/
															/*	Warning: Illegal command message received (command message from other than Event Manager)			*/
#define	ALM_MK_RCV_INV_MSG6					0x5305034E		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�i�A�N�V�������s�v���ł͂Ȃ��w�߃��b�Z�[�W�j							*/
															/*	Warning: Illegal command message received (command message other than request for action execution)	*/
#define	ALM_MK_RCV_INV_MSG7					0x5305034F		/*	ܰ�ݸ�	�s���������b�Z�[�W��M�i�C�x���g�}�l�[�W���ȊO����̉������b�Z�[�W�j						*/
															/*	Warning: Illegal response message received (response message from other than Event Manager)			*/
#define	ALM_MK_RCV_INV_MSG8					0x53050350		/*	ܰ�ݸ�	�s���������b�Z�[�W��M�i�C�x���g�ʒm��������/�A�N�V���������ʒm�����ȊO�̉������b�Z�[�W�j	*/
															/*	Warning: Illegal response message received (response message other than event notification completion/action completion notification)	*/
#define	ALM_MK_AXIS_HANDLE_ERROR			0x67050360		/*	�y�̏�	���n���h���ԍ����ُ�																		*/
															/*	Minor failure: Axis handle number is incorrect.														*/
#define	ALM_MK_SLAVE_USED_AS_MASTER			0x67050361		/*	�y�̏�	�X���[�u�����}�X�^���Ƃ��Ďg�p���悤�Ƃ���													*/
															/*	Minor failure: An attempt was made to use a slave axis as a master axis.							*/
#define	ALM_MK_MASTER_USED_AS_SLAVE			0x67050362		/*	�y�̏�	�}�X�^�����X���[�u���Ƃ��Ďg�p���悤�Ƃ���													*/
															/*	Minor failure: An attempt was made to use a master axis as a slave axis.							*/
#define	ALM_MK_SLAVE_HAS_2_MASTERS			0x67050363		/*	�y�̏�	�����X���[�u���ɂQ�ȏ�̃}�X�^�����w�肵��												*/
															/*	Minor failure: More than two master axes were specified for the same slave axis.					*/
#define	ALM_MK_SLAVE_HAS_NOT_WORK			0x67050364		/*	�y�̏�	�X���[�u���̃��[�N���������m�ۂł��Ȃ�														*/
															/*	Minor failure: Work axis cannot be assured for a slave axis.										*/
#define	ALM_MK_PARAM_OUT_OF_RANGE			0x67050365		/*	�y�̏�	�p�����[�^�͔͈͊O																			*/
															/*	Minor failure: Parameter is out of range.															*/
#define	ALM_MK_NNUM_MAX_OVER				0x67050366		/*	�y�̏�	���ω��񐔍ő�l�I�[�o�[																	*/
															/*	Minor failure: Maximum number of averaging times exceeded											*/
#define	ALM_MK_FGNTBL_INVALID				0x67050367		/*	�y�̏�	FGN �e�[�u���̓��e���s��																	*/
															/*	Minor failure: Contents of the FGN table are illegal.												*/
#define	ALM_MK_PARAM_OVERFLOW				0x67050368		/*	�y�̏�	�ݒ�l�̓I�[�o�[�t���[																		*/
															/*	Minor failure: Set value overflows.																	*/
#define	ALM_MK_ALREADY_COMMANDED			0x67050369		/*	�y�̏�	CAM �܂��� GEAR �͂��łɎ��s��																*/
															/*	Minor failure: CAM or GEAR has already been under execution.										*/
#define	ALM_MK_MULTIPLE_SHIFTS				0x6705036A		/*	�y�̏�	Position Offset/Cam Shift ���s���� Position Offset/Cam Shift �����s����						*/
															/*	Minor failure: Position Offset/Cam Shift was executed during execution of Position Offset/Cam Shift.*/
#define	ALM_MK_CAMTBL_INVALID				0x6705036B		/*	�y�̏�	CAM �e�[�u�����s���i�A�h���X�A���e�Ȃǁj													*/
															/*	Minor failure: CAM table is illegal (address, contents, etc.).										*/
#define	ALM_MK_ABORTED_BY_STOPMTN			0x6705036C		/*	�y�̏�	STOP MOTION ���ɂ���� CAM/GEAR ���삪�A�{�[�g���ꂽ										*/
															/*	Minor failure: CAM/GEAR is aborted by STOP MOTION, etc.												*/
#define	ALM_MK_HMETHOD_INVALID				0x6705036D		/*	�y�̏�	���T�|�[�g�̌��_���A����																	*/
															/*	Minor failure: Non-supported zero point return method												*/
#define	ALM_MK_MASTER_INVALID				0x6705036E		/*	�y�̏�	�}�X�^�������j�^�p�Ƃ��Ďw�肳��Ă��Ȃ�													*/
															/*	Minor failure: Master axis is not specified for monitoring.											*/
#define	ALM_MK_DATA_HANDLE_INVALID			0x6705036F		/*	�y�̏�	���W�X�^�^�O���[�o���f�[�^�n���h�����s��													*/
															/*	Minor failure: Register/global data handle is illegal.												*/
#define	ALM_MK_UNKNOWN_CAM_GEAR_ERR			0x67050370		/*	�y�̏�	CAM/GEAR �֘A�̕s���ȃG���[																	*/
															/*	Minor failure: Unclear error related to CAM/GEAR													*/
#define	ALM_MK_REG_SIZE_INVALID				0x67050371		/*	�y�̏�	���W�X�^�n���h���̃T�C�Y���s��																*/
															/*	Minor failure: Register handle size is illegal.														*/
#define	ALM_MK_ACTION_HANDLE_ERROR			0x67050372		/*	�y�̏�	�A�N�V�����n���h�����s��																	*/
															/*	Minor failure: Action handle is illegal.															*/
#define	ALM_MM_OS_ERR_MBOX1					0x83040380		/*	�d�̏�	���C���{�b�N�X�����G���[�i���[�V�����}�l�[�W�� �N���p���C���{�b�N�X�j						*/
															/*	Major failure: Mail box creation error (mail box to start up Motion Manager)						*/
#define	ALM_MM_OS_ERR_SEND_MSG1				0x83040381		/*	�d�̏�	OS���x���ł̃��b�Z�[�W���M�ُ�i���[�V�����}�l�[�W�������[�V�����J�[�l���j					*/
															/*	Major failure: Message sending error at OS level (Motion Manager to Motion Kernel)					*/
#define	ALM_MM_OS_ERR_SEND_MSG2				0x83040382		/*	�d�̏�	OS���x���ł̃��b�Z�[�W���M�ُ�i���[�V�����}�l�[�W�����C�x���g�}�l�[�W���j					*/
															/*	Major failure: Message sending error at OS level (Motion Manager to Event Manager)					*/
#define	ALM_MM_OS_ERR_RCV_MSG1				0x83040383		/*	�d�̏�	OS���x���ł̃��b�Z�[�W��M�ُ�																*/
															/*	Major failure: Message receiving error at OS level													*/
#define	ALM_MM_MK_BUSY						0x67040384		/*	�y�̏�	���[�V�����J�[�l�����S�Ďg�p��																*/
															/*	Minor failure: All Motion Kernels are in use.														*/
#define	ALM_MM_RCV_INV_MSG1					0x53040385		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�i�n���h���s���P�FhMOVE�ł͂Ȃ��j										*/
															/*	Warning: Illegal command message received (illegal handle 1: Not hMOVE.)							*/
#define	ALM_MM_RCV_INV_MSG2					0x53040386		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�i�n���h���s���Q�FhMOVE�����݂��Ȃ��j									*/
															/*	Warning: Illegal command message received (illegal handle 2: hMOVE does not exist.)					*/
#define	ALM_MM_RCV_INV_MSG3					0x53040387		/*	ܰ�ݸ�	�s���w�߃��b�Z�[�W��M�i�X�^�[�g�A�N�V�������s�v���ł͂Ȃ��j								*/
															/*	Warning: Illegal command message received (Not request for start action execution)					*/
#define	ALM_MM_RCV_INV_MSG4					0x53040388		/*	ܰ�ݸ�	�s���������b�Z�[�W��M�i�C�x���g�ʒm���������ȊO�̉������b�Z�[�W�j							*/
															/*	Warning: Illegal response message received (response message other than event notification completion)	*/
#define	ALM_MM_RCV_INV_MSG5					0x53040389		/*	ܰ�ݸ�	�s���������b�Z�[�W��M�i�A�N�V���������������b�Z�[�W�ł��̑��̃��b�Z�[�W����M�j			*/
															/*	Warning: Illegal response message received (Other messages received with action completion response message)	*/
#define	ALM_IM_DEVICEID_ERR					0x53060480		/*	DeviceID�ُ�܂��͂���Device�����T�|�[�g			*/
															/*	DeviceID error or non-supported Device				*/
#define	ALM_IM_REGHANDLE_ERR				0x53060481		/*	���W�X�^�n���h���ُ�								*/
															/*	Register handle error								*/
#define	ALM_IM_GLOBALHANDLE_ERR				0x53060482		/*	�O���[�o���f�[�^�n���h���ُ�						*/
															/*	Global data handle error							*/
#define	ALM_IM_DEVICETYPE_ERR				0x53060483		/*	�f�[�^�^�C�v�����T�|�[�g							*/
															/*	Non-supported data type								*/
#define	ALM_IM_OFFSET_ERR					0x53060484		/*	�I�t�Z�b�g�l�ُ�									*/
															/*	Incorrect offset value								*/
#define	AM_ER_UNDEF_COMMAND					0x57020501		/*	�R�}���h�R�[�h�s��									*/
															/*	Illegal command code								*/
#define	AM_ER_UNDEF_CMNDTYPE				0x57020502		/*	�R�}���h��ʕs��									*/
															/*	Illegal command type								*/
#define	AM_ER_UNDEF_OBJTYPE					0x57020503		/*	�I�u�W�F�N�g�^�C�v�s��								*/
															/*	Illegal object type									*/
#define	AM_ER_UNDEF_HANDLETYPE				0x57020504		/*	�n���h����ʕs��									*/
															/*	Illegal handle type									*/
#define	AM_ER_UNDEF_PKTDAT					0x57020505		/*	�p�P�b�g�f�[�^�s��									*/
															/*	Illegal packet data									*/
#define	AM_ER_UNDEF_AXIS					0x57020506		/*	������`											*/
															/*	axis not defined									*/
#define	AM_ER_MSGBUF_GET_FAULT				0x57020510		/*	���b�Z�[�W�o�b�t�@�Ǘ��e�[�u���擾���s				*/
															/*	Acquisition failure of  message buffer managed table*/
#define	AM_ER_ACTSIZE_GET_FAULT				0x57020511		/*	ACT�T�C�Y�擾���s									*/
															/*	Acquisition failure of ACT size						*/
#define	AM_ER_APIBUF_GET_FAULT				0x57020512		/*	API�o�b�t�@�Ǘ��e�[�u���擾���s						*/
															/*	Acquisition failure of API buffer managed table		*/
#define	AM_ER_MOVEOBJ_GET_FAULT				0x57020513		/*	MOVE�I�u�W�F�N�g�Ǘ��e�[�u���擾���s				*/
															/*	Acquisition failure of MOVE object managed table	*/
#define	AM_ER_EVTTBL_GET_FAULT				0x57020514		/*	�C�x���g�Ǘ��e�[�u���擾���s						*/
															/*	Acquisition failure of event managed table			*/
#define	AM_ER_ACTTBL_GET_FAULT				0x57020515		/*	�A�N�V�����Ǘ��e�[�u���擾���s						*/
															/*	Acquisition failure of Action managed table			*/
#define	AM_ER_1BY1APIBUF_GET_FAULT			0x57020516		/*	�������s�^API�Ǘ��e�[�u���擾���s					*/
															/*	Acquisition failure of Sequence managed table		*/
#define	AM_ER_AXSTBL_GET_FAULT				0x57020517		/*	AXIS�n���h���Ǘ��e�[�u���l�����s					*/
															/*	Acquisition failure of AXIS handle managed table	*/
#define	AM_ER_SUPERPOSEOBJ_GET_FAULT		0x57020518		/*	�����o�������I�u�W�F�N�g�Ǘ��e�[�u���l�����s		*/
															/*	Acquisition failure of Distribution synthetic object managed table	*/
#define	AM_ER_SUPERPOSEOBJ_CLEAR_FAULT		0x57020519		/*	�����o�������I�u�W�F�N�g�폜���s					*/
															/*	Deletion failure of Distribution synthetic object	*/
#define	AM_ER_AXIS_IN_USE					0x5702051A		/*	���g�p��											*/
															/*	axis in use											*/
#define	AM_ER_HASHTBL_GET_FAULT				0x5702051B		/*	�����̊Ǘ��p�n�b�V���e�[�u���擾���s				*/
															/*	Hash table acquisition failure for axial name management	*/
#define	AM_ER_UNMATCH_OBJHNDL				0x57020530		/*	MOVE�I�u�W�F�N�g�n���h���s��v						*/
															/*	MOVE object handle mismatched 						*/
#define	AM_ER_UNMATCH_OBJECT				0x57020531		/*	�I�u�W�F�N�g�s��v									*/
															/*	Object mismatched									*/
#define	AM_ER_UNMATCH_APIBUF				0x57020532		/*	API�o�b�t�@�s��v									*/
															/*	API buffer mismatched								*/
#define	AM_ER_UNMATCH_MSGBUF				0x57020533		/*	���b�Z�[�W�o�b�t�@�s��v							*/
															/*	Message buffer mismatched							*/
#define	AM_ER_UNMATCH_ACTBUF				0x57020534		/*	�A�N�V�������s�Ǘ��o�b�t�@�s��v					*/
															/*	Action execution management buffer mismatched		*/
#define	AM_ER_UNMATH_SEQUENCE				0x57020535		/*	�V�[�P���X�ԍ��s��v								*/
															/*	Sequence number mismatched							*/
#define	AM_ER_UNMATCH_1BY1APIBUF			0x57020536		/*	�������s�^API�Ǘ��e�[�u���s��v						*/
															/*	Sequential API management table mismatched			*/
#define	AM_ER_UNMATCH_MOVEOBJTABLE			0x57020537		/*	MOVE�I�u�W�F�N�g�Ǘ��e�[�u���s��v					*/
															/*	MOVE object management table mismatched				*/
#define	AM_ER_UNMATCH_MOVELISTTABLE			0x57020538		/*	MOVE LIST�Ǘ��e�[�u���s��v							*/
															/*	MOVE LIST management table mismatched				*/
#define	AM_ER_UNMATCH_MOVELIST_OBJECT		0x57020539		/*	MOVE LIST�I�u�W�F�N�g�s��v							*/
															/*	MOVE LIST object mismatched							*/
#define	AM_ER_UNMATCH_MOVELIST_OBJHNDL		0x5702053A		/*	MOVE LIST�I�u�W�F�N�g�n���h���s��v					*/
															/*	MOVE LIST object handle mismatched					*/
#define	AM_ER_UNGET_MOVEOBJTABLE			0x57020550		/*	MOVE�I�u�W�F�N�g�Ǘ��e�[�u�����m��					*/
															/*	MOVE object management table not assured			*/
#define	AM_ER_UNGET_MOVELISTTABLE			0x57020551		/*	MOVE LIST�I�u�W�F�N�g�Ǘ��e�[�u�����m��				*/
															/*	MOVE LIST object management table not assured		*/
#define	AM_ER_UNGET_1BY1APIBUFTABLE			0x57020552		/*	�������s�^API�Ǘ��e�[�u�����m��						*/
															/*	Sequential API management table not assured			*/
#define	AM_ER_NOEMPTYTBL_ERROR				0x57020560		/*	��ԃe�[�u���ɋ󂫃e�[�u���Ȃ�						*/
															/*	No unused table among interpolation tables			*/
#define	AM_ER_NOTGETSEM_ERROR				0x57020561		/*	AM-MK �Z�}�t�H�擾���s (Dynamic)					*/
															/*	Failure to get AM-MK semaphore  (Dynamic)			*/
#define	AM_ER_NOTGETTBLADD_ERROR			0x57020562		/*	��ԃe�[�u���A�h���X�擾���s						*/
															/*	Failure to get interpolation table address			*/
#define	AM_ER_NOTWRTTBL_ERROR				0x57020563		/*	���s���e�[�u���������ݎ��s (Static)					*/
															/*	Failure to write in table at execution (Static)		*/
#define	AM_ER_TBLINDEX_ERROR				0x57020564		/*	Index�̐ݒ�ُ� (Static)							*/
															/*	Index setting error (Static)						*/
#define	AM_ER_ILLTBLTYPE_ERROR				0x57020565		/*	�����ȃe�[�u���^�C�v�̎w��							*/
															/*	Invalid table type specified						*/
#define	AM_ER_UNSUPORTED_EVENT				0x57020570		/*	�C�x���g��T�|�[�g or �����G���[					*/
															/*	Event not supported or argument error				*/
#define	AM_ER_WRONG_SEQUENCE				0x57020571		/*	�V�[�P���X�ُ�										*/
															/*	Sequence error										*/
#define	AM_ER_MOVEOBJ_BUSY					0x57020572		/*	MOVE�I�u�W�F�N�g���s��								*/
															/*	MOVE object under execution							*/
#define	AM_ER_MOVELIST_BUSY					0x57020573		/*	MOVE LIST���s��										*/
															/*	MOVE LIST under execution							*/
#define	AM_ER_MOVELIST_ADD_FAULT			0x57020574		/*	MOVE OBJ�o�^�s�\									*/
															/*	MOVE OBJ cannot be registered.						*/
#define	AM_ER_CONFLICT_PHI_AXS				0x57020575		/*	�������d��											*/
															/*	Physical axes overlapped							*/
#define	AM_ER_CONFLICT_LOG_AXS				0x57020576		/*	�_�����d��											*/
															/*	Logic axes overlapped								*/
#define	AM_ER_PKTSTS_ERROR					0x57020577		/*	��M�p�P�b�g�̃X�e�[�^�X�ُ�						*/
															/*	Receiving packet status error						*/
#define	AM_ER_CONFLICT_NAME					0x57020578		/*	�����̏d��											*/
															/*	Axis name overlapped								*/
#define	AM_ER_ILLEGAL_NAME					0x57020579		/*	�����̕s��											*/
															/*	Incorrect axis name									*/
#define	AM_ER_SEMAPHORE_ERROR				0x5702057A		/*	�z�X�gPC���荞�ݎ� �Z�}�t�H�ُ�						*/
															/*	Incorrect semaphore at host PC interruption			*/
#define	AM_ER_LOG_AXS_OVER					0x5702057B		/*	�_�����ԍ��͈̓I�[�o								*/
															/*	Logical axis number exceeded						*/
#define	AM_ER_EX_ENCD_AXS_OVER				0x5702057C		/*	�O���G���R�[�_ ���ԍ��͈̓I�[�o						*/
															/*	Exteranal encoder axis number exceeded				*/
#define	IM_STATION_ERR						0x55060B00		/*	ܰ�ݸ�	�����N�`���X�e�[�V�����G���[				*/
															/*	Warning: Link communication station error			*/
#define	IM_IO_ERR							0x55060B01		/*	ܰ�ݸ�	�h�n�G���[									*/
															/*	Warning: I/O error									*/
#define CC_ER_CHANNELOVER					0x47070680		/*	�͈͊O�̃`���l���ԍ�								*/
															/*	Out-of-range channel number.						*/
#define CC_ER_OPEN_CHANNEL_ERR				0x47070681		/*	���ɃI�[�v������Ă���`���l�� 						*/
															/*	The same channel has already been opened.			*/
#define CC_ER_NOTCHANNELHANDLE				0x47070682		/*	�����ȃ`���l���n���h�����w�肳�ꂽ					*/
															/*	Undefined channnnel handle							*/
#define CC_ER_BUSY_CHANNEL_ERR				0x47070683		/*	�g�p���̃`���l�����w�肳�ꂽ						*/
															/*	Channel is in use. 									*/
#define CC_ER_ADDR_ERR						0x47070684		/*	�X�e�[�V�����A�h���X�G���[							*/
															/*	Station address error								*/
#define CC_ER_DATASIZEOVER					0x47070685		/*	�f�[�^�T�C�Y�G���[									*/
															/*	Data size error										*/
#define CC_ER_REG_ADR_ERR					0x47070686		/*	�A�h���X�A�I�t�Z�b�g�G���[							*/
															/*	Address setting error								*/
#define CC_ER_MSG_FC_ERR					0x47070687		/*	�t�@���N�V�����R�[�h�G���[							*/
															/*	Function code error									*/
#define CC_ER_DEVICE_SELECT_ERR				0x47070689		/*	�f�o�C�X�I���G���[									*/
															/*	Device Selection error								*/
#define CC_ER_COM_TIMEOUT					0x4707068A		/*	�^�C���A�E�g										*/
															/*	Time out											*/
#define CC_ER_CHNLHNDL_OVER					0x4707068B		/*	�`���l���n���h���o�^���I�[�o�[						*/
															/*	Number of channel handles exceeds the maximum value.*/
#define CC_ER_BUFSIZE_OVER					0x4707068C		/*	��M�o�b�t�@�T�C�Y�I�[�o�[							*/
															/*	Receive buffer size error							*/
#define CC_ER_INTERNAL_COMM_FMT_NG			0x4707068D		/*	�`�����G���[(�p�����[�^�t�H�[�}�b�g�G���[)			*/
															/*	Communication device error(parameter format error)	*/
#define CC_ER_INTERNAL_COMM_SEQ_NG			0x4707068E		/*	�`�����G���[(�R�}���h�V�[�P���X�G���[)				*/
															/*	Communication device error(command sequence error)	*/
#define CC_ER_INTERNAL_COMM_RST_NG			0x4707068F		/*	�`�����G���[(���Z�b�g���)							*/
															/*	Communication device error(reset status)			*/
#define CC_ER_INTERNAL_COMM_REC_NG			0x47070690		/*	�`�����G���[(�f�[�^��M�G���[)						*/
															/*	Communication device error(data receive error)		*/
#define CC_ER_INTERNAL_COMM_ERR				0x47070691		/*	�`�����G���[(���̑�)								*/
															/*	Communication device error(other error)				*/
#define CC_ER_ILLEGAL_TASK					0x47070692		/*	�s���ȃ^�X�N(ymcOpenChannel���s�^�X�N�ȊO)			*/
															/*	Illegal Task(other than ymcOpenChannel execute task).*/
#define	MP_CNF_NOCOMMUDEVICETYPE			0x47081600		/*	�ʐM�f�o�C�X�^�C�v�̎w��G���[						*/
															/*														*/
#define	MP_CNF_NOTPROTOCOLTYPE				0x47081601		/*	�����ȃv���g�R���^�C�v								*/
															/*														*/
#define	MP_CNF_NOTFUNCMODE					0x47081602		/*	�����ȋ@�\���[�h									*/
															/*														*/
#define	MP_CNF_MYADDROVER					0x47081603		/*	�͈͊O�̎��ǃA�h���X���ݒ肳�ꂽ					*/
															/*														*/
#define	MP_CNF_NOTPORTTYPE					0x47081604		/*	�����ȃ|�[�g�^�C�v									*/
															/*														*/
#define	MP_CNF_NOTPROTMODE					0x47081605		/*	�����ȃL�����N�^�r�b�g��							*/
															/*														*/
#define	MP_CNF_NOTCHARSIZE					0x47081606		/*	�����ȃv���g�R�����[�h�����ȃL�����N�^�r�b�g��		*/
															/*														*/
#define	MP_CNF_NOTPARITYBIT					0x47081607		/*	�����ȃp���e�B�r�b�g								*/
															/*														*/
#define	MP_CNF_NOTSTOPBIT					0x47081608		/*	�����ȃX�g�b�v�r�b�g								*/
															/*														*/
#define	MP_CNF_NOTBAUDRAT					0x47081609		/*	�����ȓ`�����x										*/
															/*														*/
#define	MP_CNF_TRANSDELAYOVER				0x47081610		/*	�͈͊O�̑��M�x�����w�肳�ꂽ						*/
															/*														*/
#define	MP_CNF_PEERADDROVER					0x47081611		/*	�͈͊O�̑����A�h���X���w�肳�ꂽ					*/
															/*														*/
#define	MP_CNF_SUBNETMASKOVER				0x47081612		/*	�͈͊O�̃T�u�l�b�g�}�X�N���w�肳�ꂽ				*/
															/*														*/
#define	MP_CNF_GWADDROVER					0x47081613		/*	�͈͊O��GW�A�h���X���w�肳�ꂽ						*/
															/*														*/
#define	MP_CNF_DIAGPORTOVER					0x47081614		/*	�͈͊O�̐f�f�|�[�g���w�肳�ꂽ						*/
															/*														*/
#define	MP_CNF_PROCRETRYOVER				0x47081615		/*	�͈͊O�̗L�菇���g���C�񐔂��w�肳�ꂽ				*/
															/*														*/
#define	MP_CNF_TCPZEROWINOVER				0x47081616		/*	�͈͊O��TCP�[���E�B���h�E�^�C�}						*/
															/*														*/
#define	MP_CNF_TCPRETRYOVER					0x47081617		/*	�͈͊O��TCP�đ��^�C�}�l								*/
															/*														*/
#define	MP_CNF_TCPFINOVER					0x47081618		/*	�͈͊O�̏I���^�C�}�l								*/
															/*														*/
#define	MP_CNF_IPASSEMBLEOVER				0x47081619		/*	�͈͊O��IP�g�ݍ��݃^�C�}�l							*/
															/*														*/
#define	MP_CNF_MAXPKTLENOVER				0x47081620		/*	�͈͊O�̍ő�p�P�b�g��								*/
															/*														*/
#define	MP_CNF_PEERPORTOVER					0x47081621		/*	�͈͊O�̑����|�[�g								*/
															/*														*/
#define	MP_CNF_MYPORTOVER					0x47081622		/*	�͈͊O�̎��ǃ|�[�g									*/
															/*														*/
#define	MP_CNF_NOTTRANSPROT					0x47081623		/*	�����ȃg�����X�|�[�g�w�v���g�R��					*/
															/*														*/
#define	MP_CNF_NOTAPPROT					0x47081624		/*	�����ȃA�v���P�[�V�����w�v���g�R��					*/
															/*														*/
#define	MP_CNF_NOTCODETYPE					0x47081625		/*	�����ȃR�[�h�^�C�v									*/
															/*														*/
#define	MP_CNF_CYCTIMOVER					0x47081626		/*	�͈͊O�̒ʐM�������w�肳�ꂽ						*/
															/*														*/
#define	MP_CNF_NOTIOMAPCOM					0x47081627		/*	�����ȓ��o�̓R�}���h								*/
															/*														*/
#define	MP_CNF_NOTIOTYPE					0x47081628		/*	������I/O�^�C�v�w��									*/
															/*														*/
#define	MP_CNF_IOOFFSETOVER					0x47081629		/*	�͈͊O��I/O�I�t�Z�b�g������t����ꂽ				*/
															/*														*/
#define	MP_CNF_IOSIZEOVER					0x47081630		/*	�͈͊O��I/O�T�C�Y������t����ꂽ(��)				*/
															/*														*/
#define	MP_CNF_TIOSIZEOVER					0x47081631		/*	�͈͊O��I/O�T�C�Y������t����ꂽ(���v)				*/
															/*														*/
#define	MP_CNF_MEMORY_ERR					0x47081632		/*	�R���g���[���������s��								*/
															/*														*/
#define	MP_CNF_NOTPTR						0x47081633		/*	�����ȃ|�C���^ (�ʐM�f�o�C�X�w��\����/�f�o�C�X�ŗL���/�ΏےʐM�f�o�C�X�n���h���ւ̃|�C���^�ُ�)	*/
															/*														*/
#define	MP_CNF_NOTAUTORECOVER				0x47081634		/*	�����Ȏ�����M�I��									*/
															/*														*/
#define	MP_CNF_NOTINPUTRELADD				0x47081635		/*	�͈͊O�̓��̓����[���W�X�^�A�h���X					*/
															/*														*/
#define	MP_CNF_NOTINPUTRELSIZ				0x47081636		/*	�͈͊O�̓��̓����[���W�X�^���[�h��					*/
															/*														*/
#define	MP_CNF_NOTINPUTREGADD				0x47081637		/*	�͈͊O�̓��̓��W�X�^�A�h���X						*/
															/*														*/
#define	MP_CNF_NOTINPUTREGSIZ				0x47081638		/*	�͈͊O�̓��̓��W�X�^���[�h��						*/
															/*														*/
#define	MP_CNF_NOTCOILADD					0x47081639		/*	�͈͊O�̃R�C�����W�X�^�A�h���X						*/
															/*														*/
#define	MP_CNF_NOTCOILSIZ					0x47081640		/*	�͈͊O�̃R�C�����W�X�^���[�h��						*/
															/*														*/
#define	MP_CNF_NOTHOLDINGREGADD				0x47081641		/*	�͈͊O�̕ێ����W�X�^�A�h���X						*/
															/*														*/
#define	MP_CNF_NOTHOLDINGREGSIZ				0x47081642		/*	�͈͊O�̕ێ����W�X�^���[�h��						*/
															/*														*/
#define	MP_CNF_NOTWRITERANGELO				0x47081643		/*	�͈͊O�̃R�C���^�ێ����W�X�^�����͈͂k�n			*/
															/*														*/
#define	MP_CNF_NOTWRITERANGEHI				0x47081644		/*	�͈͊O�̃R�C���^�ێ����W�X�^�����͈͂g�h			*/
															/*														*/
#define	MP_CNF_MYIPADDROVER					0x47081645		/*	�͈͊O�̎���IP�A�h���X								*/
															/*														*/
#define	MP_CNF_SENDTIMOUTOVER				0x47081646		/*	�͈͊O�̃����o�X�đ��^�C�}							*/
															/*														*/
#define	MP_CNF_SENDRETRYOVER				0x47081647		/*	�͈͊O�̃����o�X�đ���							*/
															/*														*/
#define	MP_CNF_NOTNAMESIZEOVER				0x47081648		/*	�͈͊O�̃R�����g�T�C�Y								*/
															/*														*/
#define	MP_CNF_DUPMYPORT					0x47081649		/*	���ǃ|�[�g�̏d��									*/
															/*														*/
#define	MP_CNF_DUPDSTIP						0x47081650		/*	�����IP,PORT,�g�����X�w�v���g�R�������Əd��		*/
															/*														*/
#define	MP_CNF_NOTETHERADDR					0x47081651		/*	�����ȃC�[�T�l�b�g�A�h���X							*/
															/*														*/
#define	MP_CNF_DUPMYDSTPORT					0x47081652		/*	���ǂƑ���ǂ�PORT�̏d��							*/
															/*														*/
#define	MP_CNF_NOTDSTCONNPAR				0x47081653		/*	�g�����X�|�[�g�w�v���g�R���A�A�v���P�[�V�����w�v���g�R���A�R�[�h�^�C�v�����ƈقȂ�	*/
															/*														*/
#define	MP_CNF_DUPDSTCONN					0x47081654		/*	�R�l�N�V�����̏d��									*/
															/*														*/
#define	MP_CNF_NOTDSTCONN					0x47081655		/*	�����R�l�N�V�������Ȃ�							*/
															/*														*/
#define	MP_CNF_UNDEFINEER					0x47081656		/*	���W���[���̒�`���Ȃ��A���W���[���s��v			*/
															/*														*/
#define	MP_CNF_NOTSSLOTDEVTYPE				0x47081657		/*	����`�̃f�o�C�X�^�C�v								*/
															/*														*/
#define	MP_CNF_PANELCMDERR					0x47081658		/*	PPIF�̃p�l���R�}���h�ɑ΂���ُ퉞��				*/
															/*														*/
#define MP_CNF_AUTOMEMDELAYOVER				0x47081659		/*	�͈͊O�̎�����M�����f�B���C����					*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_AUTOMEMBUFCHNOOVER			0x47081660		/*	�����Ȏ�����M�`���o�b�t�@�`���l���ԍ�				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_AUTOMEMSLVIFSELOVER			0x47081661		/*	�����Ȏ�����M�X���[�u��I/F���W�X�^�ݒ�				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_AUTOMEMSVTBOVER				0x47081662		/*	�����Ȏ�����MS/V T/B �ݒ�							*/	//<API5025>
															/*														*/	//<API5025>
#define MP_CNF_MYNODEADDROVER				0x47081663		/*	�͈͊O�̎��ǃm�[�h�A�h���X�ԍ�						*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGSELOVER					0x47081664		/*	������IO���b�Z�[�W�ʐM�I���i�L��/�����j				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPRMTYPEOVER				0x47081665		/*	������IO���b�Z�[�W�p�����[�^��ʁi�Ǐo/����)		*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGMYOFFSETOVER			0x47081666		/*	�͈͊O��IO���b�Z�[�W�擪���W�X�^�ԍ�				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGDATASIZEOVER			0x47081667		/*	�͈͊O��IO���b�Z�[�W�f�[�^�T�C�Y					*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGUPDATESELOVER			0x47081668		/*	������IO���b�Z�[�W�f�[�^�X�V�^�C�~���O�I��			*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEERREGTYPEOVER			0x47081669		/*	������IO���b�Z�[�W����@�탌�W�X�^���				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEEROFFSETOVER			0x47081670		/*	�͈͊O��IO���b�Z�[�W����@�탌�W�X�^�擪�ԍ�		*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEERNODEADDROVER		0x47081671		/*	�͈͊O��IO���b�Z�[�W����@��m�[�h�A�h���X�ԍ�		*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEERUNITNOOVER			0x47081672		/*	�͈͊O��IO���b�Z�[�W����@�퍆�@�ԍ�				*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGPEERUNITIONOOVER		0x47081673		/*	������IO���b�Z�[�W����@��v���惆�j�b�gI/O�ԍ�		*/	//<API5025>
															/*														*/	//<API5025>
#define	MP_CNF_IOMSGDISABLESELOVER			0x47081674		/*	������IO���b�Z�[�W�f�B�X�G�[�u���ݒ�				*/	//<API5025>
															/*														*/	//<API5025>
#define MP_FILE_ERR_GENERAL 				0x53168001		/*	��ʓI�ȃG���[										*/
															/*	General error. 										*/
#define MP_FILE_ERR_NOT_SUPPORTED 			0x53168002		/*	�w�肵���@�\���T�|�[�g����Ă��Ȃ�					*/
															/*	Feature not supported. 								*/
#define MP_FILE_ERR_INVALID_ARGUMENT		0x53168003		/*	�������s��											*/
															/*	Invalid argument 									*/
#define MP_FILE_ERR_INVALID_HANDLE			0x53168004		/*	�n���h�����s��										*/
															/*	Invalid handle 										*/
#define MP_FILE_ERR_NO_FILE 				0x53168064		/*	�w�肵���t�@�C���܂��̓f�B���N�g�������݂��Ȃ�		*/
															/*	No such file (or directory). 						*/
#define MP_FILE_ERR_INVALID_PATH 			0x53168065		/*	�p�X���ُ�											*/
															/*	Invalid path. 										*/
#define MP_FILE_ERR_EOF 					0x53168066		/*	�t�@�C���̍Ō�ɒB����(EOF)							*/
															/*	End of file detected. 								*/
#define MP_FILE_ERR_PERMISSION_DENIED 		0x53168067		/*	�t�@�C���ی�ᔽ									*/
															/*	Not arrowed to access the file.						*/
#define MP_FILE_ERR_TOO_MANY_FILES 			0x53168068		/*	�����ɊJ����t�@�C�����̌��E�ɒB����				*/
															/*	Too many files opened. 								*/
#define MP_FILE_ERR_FILE_BUSY 				0x53168069		/*	�t�@�C���͎g�p��									*/
															/*	File is in use. 									*/
#define MP_FILE_ERR_TIMEOUT 				0x5316806A		/*	�^�C���A�E�g��������								*/
															/*	Timeout occured. 									*/
#define MP_FILE_ERR_BAD_FS 					0x531680C8		/*	�_���t�@�C���V�X�e�����ُ�							*/
															/*	Invalid or unexepected logical filesystem in the medium. */
#define MP_FILE_ERR_FILESYSTEM_FULL 		0x531680C9		/*	�_���t�@�C���V�X�e�����t��							*/
															/*	LFS (ie the media) is full.							*/
#define MP_FILE_ERR_INVALID_LFM 			0x531680CA		/*	�w�肵���_���t�@�C���V�X�e�����W���[�����s��		*/
															/*	Invalid LFM specified. 								*/
#define MP_FILE_ERR_TOO_MANY_LFM 			0x531680CB		/*	�o�^�\�Ș_���t�@�C���V�X�e�����W���[�����̌��E	*/
															/*	LFM table is full. 									*/
#define MP_FILE_ERR_INVALID_PDM 			0x5316812C		/*	�w�肵�������L���f�o�C�X�h���o���W���[�����s��		*/
															/*	Invalid PDM specified. 								*/
#define MP_FILE_ERR_INVALID_MEDIA 			0x5316812D		/*	�w�肵�����f�B�A���s��								*/
															/*	Invalid media specified. 							*/
#define MP_FILE_ERR_TOO_MANY_PDM 			0x5316812E		/*	�o�^�\�ȕ����L���f�o�C�X�h���o���W���[�����̌��E	*/
															/*	Too many PDM. 										*/
#define MP_FILE_ERR_TOO_MANY_MEDIA 			0x5316812F		/*	�o�^�\�ȃ��f�B�A���̌��E�ɒB����					*/
															/*	Too many media. 									*/
#define MP_FILE_ERR_WRITE_PROTECTED 		0x53168130		/*	���f�B�A���������݋֎~�ł���						*/
															/*	Write protected media. 								*/
#define MP_FILE_ERR_INVALID_DEVICE 			0x53168190		/*	�w�肵���f�o�C�X���s��								*/
															/*	Invalid device specified. 							*/
#define MP_FILE_ERR_DEVICE_IO 				0x53168191		/*	�f�o�C�XI/O�G���[									*/
															/*	Error occured in accessing the device. 				*/
#define MP_FILE_ERR_DEVICE_BUSY 			0x53168192		/*	�f�o�C�X���g�p��									*/
															/*	Device is in use. 									*/
#define MP_FILE_ERR_NO_CARD 				0x5316A711		/*	CF�J�[�h������										*/
															/*	CF CARD not mounted.								*/
#define MP_FILE_ERR_CARD_POWER 				0x5316A712		/*	CF�J�[�h�̓d����OFF���								*/
															/*	CF CARD Power-OFF.									*/
#define	MP_CARD_SYSTEM_ERR					0x53178FFF		/*	�J�[�h�V�X�e���G���[								*/
															/*	Card System Error.									*/
#define	ERROR_CODE_GET_DIREC_OFFSET			0x83001a01		/*	�f�B���N�g���̈�̃I�t�Z�b�g���l���ł��Ȃ�			*/
															/*	Directory area offset cannot be got.				*/
#define	ERROR_CODE_GET_DIREC_INFO			0x83001a02		/*	�f�B���N�g�����̎擾�G���[						*/
															/*	Directory area offset cannot be got.				*/
#define	ERROR_CODE_FUNC_TABLE				0x83001a03		/*	�V�X�e���R�[���֐��e�[�u���̎擾�G���[				*/
															/*	Failure to get directory information				*/
#define	ERROR_CODE_SLEEP_TASK				0x83001a04		/*	�X���[�v�G���[										*/
															/*	Failure to get system call function table			*/
#define	ERROR_CODE_DEVICE_HANDLE_FULL		0x43001a41		/*	�f�o�C�X�n���h�����ő吔�𒴂���					*/
															/*	Sleep error											*/
#define	ERROR_CODE_ALLOC_MEMORY				0x43001a42		/*	�̈�m�ێ��s										*/
															/*	Number of device handles exceeds the maximum value.	*/
#define	ERROR_CODE_BUFCOPY					0x43001a43		/*	MemoryCopy(),name_copy()�̃G���[					*/
															/*	Failure to get the area.							*/
#define	ERROR_CODE_GET_COMMEM_OFFSET		0x43001a44		/*	���L�������I�t�Z�b�g�l�̎擾�G���[					*/
															/*	MemoryCopy(),name_copy() error						*/
#define	ERROR_CODE_CREATE_SEMPH				0x43001a45		/*	�Z�}�t�H�쐬�G���[									*/
															/*	Failure to get common memory offset value			*/
#define	ERROR_CODE_DELETE_SEMPH				0x43001a46		/*	�Z�}�t�H�폜�G���[									*/
															/*	Semaphore creation error							*/
#define	ERROR_CODE_LOCK_SEMPH				0x43001a47		/*	�Z�}�t�H���b�N���̃G���[							*/
															/*	Semaphore deletion error							*/
#define	ERROR_CODE_UNLOCK_SEMPH				0x43001a48		/*	�Z�}�t�H���b�N�������̃G���[						*/
															/*	Error at semaphore lock								*/
#define	ERROR_CODE_PACKETSIZE_OVER			0x43001a49		/*	�p�P�b�g�T�C�Y�̃I�[�o�[							*/
															/*	Error at semaphore release							*/
#define	ERROR_CODE_UNREADY					0x43001a4a		/*	�R���g���[�����������G���[							*/
															/*	Error when controller is being initialized			*/
#define	ERROR_CODE_CPUSTOP					0x43001a4b		/*	�b�o�t�X�g�b�v���G���[								*/
															/*	Error when CPU is stopping							*/
#define	ERROR_LOG_NAME						0x43001a4c		/*	���O�t�@�C��(�f�[�^)���Ȃ�							*/
															/*	Log File or Data cannot be found					*/
#define	ERROR_LOG_WRITE_OPEN_ERROR			0x43001a4d		/*	PC���t�@�C��OPEN�G���[								*/
															/*	File in PC open error 								*/
#define	ERROR_LOG_SEQ						0x43001a4e		/*	���O�V�[�P���X�G���[								*/
															/*	Log sequence error									*/
#define	ERROR_LOG_WRITE_FILE				0x43001a4f		/*	PC���t�@�C�������݃G���[							*/
															/*  File in PC write error								*/
#define	ERROR_LOG_TIME_OUT					0x43001a50		/*	���O�^�C���A�E�g									*/
															/*	Log Timeout											*/
#define	ERROR_LOG_OTHER						0x43001a51		/*	���O���̑��G���[									*/
															/*	Log other error										*/
#define	ERROR_LOG_MPFILE_OPEN_ERROR			0x43001a52		/*	MP���t�@�C��OPEN�G���[								*/
															/*	File in MP open error								*/
#define	ERROR_CODE_CNTRNO					0x470B1a81		/*	CPU�ԍ��s��											*/
															/*	CPU number is illegal								*/
#define	ERROR_CODE_SELECTION				0x470B1a82		/*	�I��l(0or1)���s��									*/
															/*	Device number										*/
#define	ERROR_CODE_LENGTH					0x470B1a83		/*	�f�[�^��											*/
															/*	Illegal selected value (0 or 1)						*/
#define	ERROR_CODE_OFFSET					0x470B1a84		/*	�I�t�Z�b�g�l										*/
															/*	Data length											*/
#define	ERROR_CODE_DATACOUNT				0x470B1a85		/*	�f�[�^��											*/
															/*	Offset value										*/
#define	ERROR_CODE_DATREAD					0x46001a86		/*	���L����������̓ǂݏo�����s						*/
															/*	Number of data items								*/
#define	ERROR_CODE_DATWRITE					0x46001a87		/*	���L�������ւ̏������ݎ��s							*/
															/*	Failure to read out from common memory				*/
#define	ERROR_CODE_BITWRITE					0x46001a88		/*	���L�������ւ̃r�b�g�f�[�^�������ݎ��s				*/
															/*	Failure to write in to common memory				*/
#define	ERROR_CODE_DEVCNTR					0x46001a89		/*	DeviceIoControl()���G���[�I��						*/
															/*	Failure to write in bit data to common memory		*/
#define	ERROR_CODE_NOTINIT					0x460F1a8a		/*	�h���C�o�������G���[								*/
															/*	DeviceIoControl() completed erroneously.			*/
#define	ERROR_CODE_SEMPHLOCK				0x41001a8b		/*	�p�P�b�g���M�Z�}�t�H���b�N							*/
															/*	Driver initialization error							*/
#define	ERROR_CODE_SEMPHUNLOCK				0x41001a8c		/*	�p�P�b�g��M�Z�}�t�H�A�����b�N						*/
															/*	Packet sending semaphore locked						*/
#define	ERROR_CODE_DRV_PROC					0x460F1a8d		/*	�h���C�o�̏������G���[�I��							*/
															/*	Packet receiving semaphore not locked				*/
#define	ERROR_CODE_GET_DRIVER_HANDLE		0x460F1a8e		/*	�h���C�o�t�@�C���̃n���h���擾�G���[				*/
															/*	Driver processing completed erroneously.			*/
#define	ERROR_CODE_SEND_MSG					0x450E1ac1		/*	���b�Z�[�W���M�G���[								*/
															/*	Failure to get driver file handle					*/
#define	ERROR_CODE_RECV_MSG					0x450E1ac2		/*	���b�Z�[�W��M�G���[								*/
															/*	Message sending error								*/
#define	ERROR_CODE_INVALID_RESPONSE			0x450E1ac3		/*	��M�p�P�b�g���s��									*/
															/*	Message receiving error								*/
#define	ERROR_CODE_INVALID_ID				0x450E1ac4		/*	��M�p�P�b�g��ID���s��								*/
															/*	Receiving packet illegal							*/
#define	ERROR_CODE_INVALID_STATUS			0x450E1ac5		/*	��M�p�P�b�g�̃X�e�[�^�X���s��						*/
															/*	Receiving packet ID illegal							*/
#define	ERROR_CODE_INVALID_CMDCODE			0x450E1ac6		/*	��M�p�P�b�g�̃R�}���h�R�[�h���s��					*/
															/*	Receiving packet status illegal						*/
#define	ERROR_CODE_INVALID_SEQNO			0x450E1ac7		/*	��M�p�P�b�g�̃V�[�P���X�ԍ����s��					*/
															/*	Receiving packet command code illegal				*/
#define	ERROR_CODE_SEND_RETRY_OVER			0x450E1ac8		/*	���g���C�񐔂𒴂����i�p�P�b�g���M�j				*/
															/*	Receiving packet sequence number illegal			*/
#define	ERROR_CODE_RECV_RETRY_OVER			0x450E1ac9		/*	���g���C�񐔂𒴂����i�p�P�b�g��M�j				*/
															/*	Number of retries exceeded (packet sending)			*/
#define	ERROR_CODE_RESPONSE_TIMEOUT			0x450E1aca		/*	�����҂��^�C���A�E�g�G���[							*/
															/*	Number of retries exceeded (packet receiving)		*/
#define	ERROR_CODE_WAIT_FOR_EVENT			0x450E1acb		/*	�C�x���g�҂��G���[									*/
															/*	Response waiting timeout error						*/
#define	ERROR_CODE_EVENT_OPEN				0x450E1acc		/*	�C�x���g�I�[�v�����s								*/
															/*	Event waiting error									*/
#define	ERROR_CODE_EVENT_RESET				0x450E1acd		/*	�C�x���g���Z�b�g���s								*/
															/*	Failure to open event								*/
#define	ERROR_CODE_EVENT_READY				0x450E1ace		/*	�C�x���g�҂��������s								*/
															/*	Failure to reset event								*/
#define	ERROR_CODE_PROCESSNUM				0x43001b01		/*	�v���Z�X�� ����										*/
															/*	Failure to prepare for waiting for event			*/
#define	ERROR_CODE_GET_PROC_INFO			0x43001b02		/*	�v���Z�X���̎擾									*/
															/*	Number of processes exceeded						*/
#define	ERROR_CODE_THREADNUM				0x43001b03		/*	�X���b�h�� ����										*/
															/*	Process information getting error					*/
#define	ERROR_CODE_GET_THRD_INFO			0x43001b04		/*	�X���b�h���̎擾									*/
															/*	Number of threads exceeded							*/
#define	ERROR_CODE_CREATE_MBOX				0x43001b05		/*	���[���{�b�N�X�̐����G���[							*/
															/*	Thread information getting error					*/
#define	ERROR_CODE_DELETE_MBOX				0x43001b06		/*	���[���{�b�N�X�̍폜�G���[							*/
															/*	Mail box creation error								*/
#define	ERROR_CODE_GET_TASKID				0x83001b07		/*	�^�X�NID�擾�G���[									*/
															/*	Mail box deletion error								*/
#define	ERROR_CODE_NO_THREADINFO			0x43001b08		/*	�w�肳�ꂽ�X���b�h��񂪑��݂��Ȃ�					*/
															/*	Failure to get task ID								*/
#define	ERROR_CODE_COM_INITIALIZE			0x43001b09		/*	COM�̏������G���[									*/
															/*	Specified thread information does not exist.		*/
#define	ERROR_CODE_COMDEVICENUM				0x430F1b41		/*	ComDevice�� ����									*/
															/*	COM initialization error							*/
#define	ERROR_CODE_GET_COM_DEVICE_HANDLE	0x430F1b42		/*	ComDevice���\���̂̎擾�G���[						*/
															/*	Number of ComDevice exceeded						*/
#define	ERROR_CODE_COM_DEVICE_FULL			0x430F1b43		/*	ComDevice���ő吔�𒴂���							*/
															/*	Failure to get ComDevice information structure		*/
#define	ERROR_CODE_CREATE_PANELOBJ			0x430F1b44		/*	�p�l���R�}���h�I�u�W�F�N�g�������s					*/
															/*	ComDevice exceeds the maximum number.				*/
#define	ERROR_CODE_OPEN_PANELOBJ			0x430F1b45		/*	�p�l���R�}���h�I�u�W�F�N�g�I�[�v�����s				*/
															/*	Failure to create panel command object				*/
#define	ERROR_CODE_CLOSE_PANELOBJ			0x430F1b46		/*	�p�l���R�}���h�I�u�W�F�N�g�N���[�Y���s				*/
															/*	Failure to open panel command object				*/
#define	ERROR_CODE_PROC_PANELOBJ			0x430F1b47		/*	�p�l���R�}���h�I�u�W�F�N�g�������s					*/
															/*	Failure to close panel command object				*/
#define	ERROR_CODE_CREATE_CNTROBJ			0x430F1b48		/*	�p�l���R�}���h�I�u�W�F�N�g�������s					*/
															/*	Failure to process panel command object				*/
#define	ERROR_CODE_OPEN_CNTROBJ				0x430F1b49		/*	�p�l���R�}���h�I�u�W�F�N�g�I�[�v�����s				*/
															/*	Failure to create panel command object				*/
#define	ERROR_CODE_CLOSE_CNTROBJ			0x430F1b4a		/*	�p�l���R�}���h�I�u�W�F�N�g�N���[�Y���s				*/
															/*	Failure to open panel command object				*/
#define	ERROR_CODE_PROC_CNTROBJ				0x430F1b4b		/*	�p�l���R�}���h�I�u�W�F�N�g�������s					*/
															/*	Failure to close panel command object				*/
#define	ERROR_CODE_CREATE_COMDEV_MUTEX		0x430F1b4c		/*	ComDevice�e�[�u����Mutex�쐬���s					*/
															/*	Failure to process panel command object				*/
#define	ERROR_CODE_CREATE_COMDEV_MAPFILE	0x430F1b4d		/*	ComDevice�e�[�u���p��MapFile�쐬���s				*/
															/*	Failure to create Mutex for ComDevice table			*/
#define	ERROR_CODE_OPEN_COMDEV_MAPFILE		0x430F1b4e		/*	ComDevice�e�[�u���p��MapFile�I�[�v�����s			*/
															/*	Failure to create MapFile for ComDevice table		*/
#define	ERROR_CODE_NOT_OBJECT_TYPE			0x430F1b4f		/*	�I�u�W�F�N�g��ʂ̃G���[							*/
															/*	Failure to open MapFile for ComDevice table			*/
#define	ERROR_CODE_COM_NOT_OPENED			0x430F1b50		/*	���I�[�v��											*/
															/*	Object type error									*/
#define	ERROR_CODE_PNLCMD_CPU_CONTROL		0x43081b80		/*	CPU�R���g���[���̃G���[								*/
															/*	Not opened											*/
#define	ERROR_CODE_PNLCMD_SFILE_READ		0x43081b81		/*	�\�[�X�t�@�C���ǂݏo�����s							*/
															/*	CPU control error									*/
#define	ERROR_CODE_PNLCMD_SFILE_WRITE		0x43081b82		/*	�\�[�X�t�@�C���������ݎ��s							*/
															/*	Failure to read out source file						*/
#define	ERROR_CODE_PNLCMD_REGISTER_READ		0x43081b83		/*	���W�X�^�ǂݏo�����s								*/
															/*	Failure to write in source file						*/
#define	ERROR_CODE_PNLCMD_REGISTER_WRITE	0x43081b84		/*	���W�X�^�������ݎ��s								*/
															/*	Failure to read out register						*/
#define	ERROR_CODE_PNLCMD_API_SEND			0x43081b85		/*	API Send�R�}���h���s								*/
															/*	Failure to write in register						*/
#define	ERROR_CODE_PNLCMD_API_RECV			0x43081b86		/*	API Recv�R�}���h���s								*/
															/*	API Send command error								*/
#define	ERROR_CODE_PNLCMD_NO_RESPONSE		0x43081b87		/*	API Recv�ŉ����p�P�b�g���܂��Ȃ�					*/
															/*	API Recv command error								*/
#define	ERROR_CODE_PNLCMD_PACKET_READ		0x43081b88		/*	�p�P�b�g�E���[�h���s								*/
															/*	No response packet is received at API Recv.			*/
#define	ERROR_CODE_PNLCMD_PACKET_WRITE		0x43081b89		/*	�p�P�b�g�E���C�g���s								*/
															/*	Failure to read packet								*/
#define	ERROR_CODE_PNLCMD_STATUS_READ		0x43081b8a		/*	�X�e�[�^�X���[�h���s								*/
															/*	Failure to write packet								*/
#define	ERROR_CODE_NOT_CONTROLLER_RDY		0x440D1ba0		/*	�R���g���[���^�]��������							*/
															/*														*/
#define	ERROR_CODE_DOUBLE_CMD				0x440D1ba1		/*	���[�V�����R�}���h�d���w��							*/
															/*														*/
#define	ERROR_CODE_DOUBLE_RCMD				0x440D1ba2		/*	���[�V�������X�|���X�R�}���h�d���w��				*/
															/*														*/
#define	ERROR_CODE_FILE_NOT_OPENED			0x43001bc1		/*	�t�@�C�����I�[�v������Ă��Ȃ�						*/
															/*	Failure to read status								*/
#define	ERROR_CODE_OPENFILE_NUM				0x43001bc2		/*	�I�[�v���t�@�C���� ����								*/
															/*	File is not opened.									*/
#define	MP_CTRL_SYS_ERROR					0x4308106f		/*	�V�X�e���G���[										*/
															/*														*/
#define	MP_CTRL_PARAM_ERR					0x43081092		/*	�s���ȃp�����[�^�w��								*/
															/*														*/
#define	MP_CTRL_FILE_NOT_FOUND				0x43081094		/*	���݂��Ȃ��v���O�������w�肳�ꂽ					*/
															/*														*/
#define	MP_NOTMOVEHANDLE					0x470B1100		/*	����`��Move�n���h��								*/
															/*	Undefined Move handle								*/
#define	MP_NOTTIMERHANDLE					0x470B1101		/*	����`�̃^�C�}�n���h��								*/
															/*	Undefined timer handle								*/
#define	MP_NOTINTERRUPT						0x470B1102		/*	����`�̉��z���荞�ݔԍ�							*/
															/*	Undefined virtual interruption number				*/
#define	MP_NOTEVENTHANDLE					0x470B1103		/*	����`�̃C�x���g�n���h��							*/
															/*	Undefined event handle								*/
#define	MP_NOTMESSAGEHANDLE					0x470B1104		/*	����`�̃��b�Z�[�W�n���h��							*/
															/*	Undefined message handle							*/
#define	MP_NOTUSERFUNCTIONHANDLE			0x470B1105		/*	����`�̃��[�U�֐��̃n���h��						*/
															/*	Undefined user function handle						*/
#define	MP_NOTACTIONHANDLE					0x470B1106		/*	����`�̃A�N�V�����n���h��							*/
															/*	Undefined action handle								*/
#define	MP_NOTSUBSCRIBEHANDLE				0x470B1107		/*	����`��Subscribe�n���h��							*/
															/*	Undefined Subscribe handle							*/
#define	MP_NOTDEVICEHANDLE					0x470B1108		/*	����`�̃f�o�C�X�n���h��							*/
															/*	Undefined device handle								*/
#define	MP_NOTAXISHANDLE					0x470B1109		/*	����`�̎��n���h��									*/
															/*	Undefined axis handle								*/
#define	MP_NOTMOVELISTHANDLE				0x470B110a		/*	����`��MoveList�n���h��							*/
															/*	Undefined MoveList handle							*/
#define	MP_NOTTRACEHANDLE					0x470B110b		/*	����`��Trace�n���h��								*/
															/*	Undefined Trace handle								*/
#define	MP_NOTGLOBALDATAHANDLE				0x470B110c		/*	����`�̃O���[�o���f�[�^�n���h��					*/
															/*	Undefined global data handle						*/
#define	MP_NOTSUPERPOSEHANDLE				0x470B110d		/*	����`�̃O���[�o���f�[�^�n���h��					*/
															/*	Undefined Superpose handle							*/
#define	MP_NOTCONTROLLERHANDLE				0x470B110e		/*	����`�̃R���g���[���n���h��						*/
															/*	Undefined Controller handle							*/
#define	MP_NOTFILEHANDLE					0x470B110f		/*	����`�̃t�@�C���n���h��							*/
															/*	Undefined file handle								*/
#define	MP_NOTREGISTERDATAHANDLE			0x470B1110		/*	���W�X�^�n���h���̖���`							*/
															/*	Undefined register handle							*/
#define	MP_NOTALARMHANDLE					0x470B1111		/*	�A���[���n���h���̖���`							*/
															/*	Undefined alarm handle								*/
#define	MP_NOTCAMHANDLE						0x470B1112		/*	CAM�n���h���̖���`									*/
															/*	Undefined CAM handle								*/
#define	MP_NOTHANDLE						0x470B11ff		/*	����`�̃n���h��									*/
															/*	Undefined handle									*/
#define	MP_NOTEVENTTYPE						0x470B1200		/*	����`�̃C�x���g�^�C�v								*/
															/*	Undefined event type								*/
#define	MP_NOTDEVICETYPE					0x470B1201		/*	����`�̃f�o�C�X�^�C�v								*/
															/*	Undefined device type								*/
#define	MP_NOTDATAUNITSIZE					0x4B0B1202		/*	����`�̒P�ʃf�[�^�T�C�Y							*/
															/*	Undefined unit data size							*/
#define	MP_NOTDEVICE						0x470B1203		/*	����`�̃f�o�C�X									*/
															/*	Undefined device									*/
#define	MP_NOTACTIONTYPE					0x470B1204		/*	����`�̃A�N�V�����^�C�v							*/
															/*	Undefined action type								*/
#define	MP_NOTPARAMNAME						0x4B0B1205		/*	����`�̃p�����[�^��								*/
															/*	Undefined parameter name							*/
#define	MP_NOTDATATYPE						0x470B1206		/*	����`�̃f�[�^�^�C�v								*/
															/*	Undefined data type									*/
#define	MP_NOTBUFFERTYPE					0x470B1207		/*	����`�̃o�b�t�@�^�C�v								*/
															/*	Undefined buffer type								*/
#define	MP_NOTMOVETYPE						0x4B0B1208		/*	����`��Move�^�C�v									*/
															/*	Undefined Move type									*/
#define	MP_NOTANGLETYPE						0x470B1209		/*	����`��Angle�^�C�v									*/
															/*	Undefined Angle type								*/
#define	MP_NOTDIRECTION						0x4B0B120a		/*	����`�̉�]����									*/
															/*	Undefined rotating direction						*/
#define	MP_NOTAXISTYPE						0x4B0B120b		/*	����`�̎��^�C�v									*/
															/*	Undefined axis type									*/
#define	MP_NOTUNITTYPE						0x4B0B120c		/*	����`�̒P�ʃ^�C�v									*/
															/*	Undefined unit type									*/
#define	MP_NOTCOMDEVICETYPE					0x470B120d		/*	����`��ComDevice���								*/
															/*	Undefined ComDevice type							*/
#define	MP_NOTCONTROLTYPE					0x470B120e		/*	����`��Control���									*/
															/*	Undefined Control type								*/
#define	MP_NOTFILETYPE						0x4B0B120f		/*	����`�̃t�@�C�����								*/
															/*	Undefined file type									*/
#define	MP_NOTSEMAPHORETYPE					0x470B1210		/*	����`�̃Z�}�t�H�^�C�v								*/
															/*	Undefined semaphore type							*/
#define	MP_NOTSYSTEMOPTION					0x470B1211		/*	����`��SystemOption								*/
															/*	Undefined system option								*/
#define	MP_TIMEOUT_ERR						0x470B1212		/*	�^�C���A�E�g �G���[									*/
															/*	Timeout error										*/
#define	MP_TIMEOUT							0x470B1213		/*	�^�C���A�E�g										*/
															/*	Timeout												*/
#define	MP_NOTSUBSCRIBETYPE					0x470B1214		/*	����`��Subscribe�^�C�v								*/
															/*	Undefined scan type									*/
#define	MP_NOTSCANTYPE						0x4B0B1214		/*	����`�̃X�L�����^�C�v								*/
															/*	Undefined scan type									*/
#define	MP_DEVICEOFFSETOVER					0x470B1300		/*	�͈͊O�̃f�o�C�X�I�t�Z�b�g							*/
															/*	Out-of-range device offset							*/
#define	MP_DEVICEBITOFFSETOVER				0x470B1301		/*	�͈͊O�̃r�b�g�I�t�Z�b�g							*/
															/*	Out-of-range bit offset								*/
#define	MP_UNITCOUNTOVER					0x4B0B1302		/*	�͈͊O�̌�										*/
															/*	Out-of-range quantity								*/
#define	MP_COMPAREVALUEOVER					0x4B0B1303		/*	�͈͊O�̔�r�l										*/
															/*	Out-of-range compared value							*/
#define	MP_FCOMPAREVALUEOVER				0x4B0B1304		/*	�͈͊O�̕��������_��r�l							*/
															/*	Out-of-range floating-point compared value			*/
#define	MP_EVENTCOUNTOVER					0x470B1305		/*	�͈͊O�̉��z���荞�ݔԍ�							*/
															/*	Out-of-range virtual interruption number			*/
#define	MP_VALUEOVER						0x470B1306		/*	�͈͊O�̒l											*/
															/*	Out-of-range value									*/
#define	MP_FVALUEOVER						0x470B1307		/*	�͈͊O�̕��������l									*/
															/*	Out-of-range floating point							*/
#define	MP_PSTOREDVALUEOVER					0x470B1308		/*	�͈͊O�̊i�[�ꏊ�̃|�C���^							*/
															/*	Out-of-range storage position pointer				*/
#define	MP_PSTOREDFVALUEOVER				0x470B1309		/*	�͈͊O�̊i�[�ꏊ�̃|�C���^(���������_�l)			*/
															/*	Out-of-range storage position pointer (floating decimal point value)	*/
#define	MP_SIZEOVER							0x470B130a		/*	�͈͊O�̃T�C�Y										*/
															/*	Out-of-range size									*/
#define	MP_RECEIVETIMEROVER					0x470B1310		/*	�͈͊O�̎�M�҂����Ԓl								*/
															/*	Out-of-range waiting time value for receiving		*/
#define	MP_RECNUMOVER						0x470B1311		/*	�͈͊O�̃��R�[�h��(MoveList)						*/
															/*	Out-of-range number of records (MoveList)			*/
#define	MP_PARAMOVER						0x4B0B1312		/*	�͈͊O�̃p�����[�^									*/
															/*	Out-of-range parameter								*/
#define	MP_FRAMEOVER						0x470B1313		/*	�͈͊O�̃p�����[�^									*/
															/*	Out-of-range number of frames						*/
#define	MP_RADIUSOVER						0x4B0B1314		/*	�͈͊O�̔��a										*/
															/*	Out-of-range radius									*/
#define	MP_CONTROLLERNOOVER					0x470B1315		/*	�͈͊O�̋@��ԍ�									*/
															/*	Out-of-range device number							*/
#define	MP_AXISNUMOVER						0x4B0B1316		/*	�͈͊O�̎���										*/
															/*	Out-of-range number of axes							*/
#define	MP_DIGITOVER						0x4B0B1317		/*	�͈͊O�̌���										*/
															/*	Out-of-range number of digits						*/
#define	MP_CALENDARVALUEOVER				0x4B0B1318		/*	�͈͊O�̃J�����_�f�[�^								*/
															/*	Out-of-range calendar data							*/
#define	MP_OFFSETOVER						0x470B1319		/*	�͈͊O�̃I�t�Z�b�g									*/
															/*	Out-of-range offset									*/
#define	MP_NUMBEROVER						0x470B131a		/*	�͈͊O�̃��W�X�^�̐ݒ�����w�肳�ꂽ				*/
															/*	Out-of-range number of registers has been specified.*/
#define	MP_RACKOVER							0x470B131b		/*	�͈͊O�̃��b�N�ԍ����w�肳�ꂽ						*/
															/*	Out-of-range rack number has been specified.		*/
#define	MP_SLOTOVER							0x470B131c		/*	�͈͊O�̃��b�N�ԍ����w�肳�ꂽ						*/
															/*	Out-of-range slot number has been specified.		*/
#define	MP_FIXVALUEOVER						0x470B131d		/*	�Œ菬���_�^�̃f�[�^���͈͊O�ƂȂ���				*/
															/*	Fixed decimal point type data has been out of range.*/
#define	MP_REGISTERINFOROVER				0x470B131e		/*	�͈͊O�̃��W�X�^���̌����w�肳�ꂽ				*/
															/*	Out-of-range number of register infomation has been specified.*/
#define	PC_MEMORY_ERR						0x430B1400		/*	�p�\�R���������s��									*/
															/*	PC memory shortage									*/
#define	MP_NOCOMMUDEVICETYPE				0x470B1500		/*	�ʐM�f�o�C�X�^�C�v�̎w��G���[						*/
															/*	Communication device type specification error		*/
#define	MP_NOTPROTOCOLTYPE					0x470B1501		/*	�����ȃv���g�R���^�C�v								*/
															/*	Invalid protocol type								*/
#define	MP_NOTFUNCMODE						0x470B1502		/*	�����ȋ@�\���[�h									*/
															/*	Invalid function mode								*/
#define	MP_MYADDROVER						0x470B1503		/*	�͈͊O�̎��ǃA�h���X���ݒ肳�ꂽ					*/
															/*	Out-of-range local station address has been set.	*/
#define	MP_NOTPORTTYPE						0x470B1504		/*	�����ȃ|�[�g�^�C�v									*/
															/*	Invalid port type									*/
#define	MP_NOTPROTMODE						0x470B1505		/*	�����ȃv���g�R�����[�h								*/
															/*	Invalid protocol mode								*/
#define	MP_NOTCHARSIZE						0x470B1506		/*	�����ȃL�����N�^�r�b�g��							*/
															/*	Invalid character bit length						*/
#define	MP_NOTPARITYBIT						0x470B1507		/*	�����ȃp���e�B�r�b�g								*/
															/*	Invalid parity bit									*/
#define	MP_NOTSTOPBIT						0x470B1508		/*	�����ȃX�g�b�v�r�b�g								*/
															/*	Invalid stop bit									*/
#define	MP_NOTBAUDRAT						0x470B1509		/*	�����ȓ`�����x										*/
															/*	Invalid transmission speed							*/
#define	MP_TRANSDELAYOVER					0x470B1510		/*	�͈͊O�̑��M�x�����w�肳�ꂽ						*/
															/*	Out-of-range sending delay has been specified.		*/
#define	MP_PEERADDROVER						0x470B1511		/*	�͈͊O�̑����A�h���X���w�肳�ꂽ					*/
															/*	Out-of-range remote station address has been specified.	*/
#define	MP_SUBNETMASKOVER					0x470B1512		/*	�͈͊O�̃T�u�l�b�g�}�X�N���w�肳�ꂽ				*/
															/*	Out-of-range subnet mask has been specified.		*/
#define	MP_GWADDROVER						0x470B1513		/*	�͈͊O��GW�A�h���X���w�肳�ꂽ						*/
															/*	Out-of-range GW address has been specified.			*/
#define	MP_DIAGPORTOVER						0x470B1514		/*	�͈͊O�̐f�f�|�[�g���w�肳�ꂽ						*/
															/*	Out-of-range diagnostic port has been specified.	*/
#define	MP_PROCRETRYOVER					0x470B1515		/*	�͈͊O�̗L�菇���g���C�񐔂��w�肳�ꂽ				*/
															/*	Out-of-range number of retries has been specified.	*/
#define	MP_TCPZEROWINOVER					0x470B1516		/*	�͈͊O��TCP�[���E�B���h�E�^�C�}						*/
															/*	Out-of-range TCP zero window timer					*/
#define	MP_TCPRETRYOVER						0x470B1517		/*	�͈͊O��TCP�đ��^�C�}�l								*/
															/*	Out-of-range TCP resending timer value				*/
#define	MP_TCPFINOVER						0x470B1518		/*	�͈͊O�̏I���^�C�}�l								*/
															/*	Out-of-range completion timer value					*/
#define	MP_IPASSEMBLEOVER					0x470B1519		/*	�͈͊O��IP�g�ݍ��݃^�C�}�l							*/
															/*	Out-of-range IP incorporating timer value			*/
#define	MP_MAXPKTLENOVER					0x470B1520		/*	�͈͊O�̍ő�p�P�b�g��								*/
															/*	Out-of-range maximum packet length					*/
#define	MP_PEERPORTOVER						0x470B1521		/*	�͈͊O�̑����|�[�g								*/
															/*	Out-of-range remote station port					*/
#define	MP_MYPORTOVER						0x470B1522		/*	�͈͊O�̎��ǃ|�[�g									*/
															/*	Out-of-range local station port						*/
#define	MP_NOTTRANSPROT						0x470B1523		/*	�����ȃg�����X�|�[�g�w�v���g�R��					*/
															/*	Invalid transport layer protocol					*/
#define	MP_NOTAPPROT						0x470B1524		/*	�����ȃA�v���P�[�V�����w�v���g�R��					*/
															/*	Invalid application layer protocol					*/
#define	MP_NOTCODETYPE						0x470B1525		/*	�����ȃR�[�h�^�C�v									*/
															/*	Invalid code type									*/
#define	MP_CYCTIMOVER						0x470B1526		/*	�͈͊O�̒ʐM�������w�肳�ꂽ						*/
															/*	Out-of-range communication cycle has been specified.*/
#define	MP_NOTIOMAPCOM						0x470B1527		/*	�����ȓ��o�̓R�}���h								*/
															/*	Invalid I/O commands								*/
#define	MP_NOTIOTYPE						0x470B1528		/*	������I/O�^�C�v�w��									*/
															/*	Invalid I/O type specification						*/
#define	MP_IOOFFSETOVER						0x470B1529		/*	�͈͊O��I/O�I�t�Z�b�g������t����ꂽ				*/
															/*	Out-of-range I/O offset has been allocated.			*/
#define	MP_IOSIZEOVER						0x470B1530		/*	�͈͊O��I/O�T�C�Y������t����ꂽ(��)				*/
															/*	Out-of-range I/O size has been allocated (individualy).	*/
#define	MP_TIOSIZEOVER						0x470B1531		/*	�͈͊O��I/O�T�C�Y������t����ꂽ(���v)				*/
															/*	Out-of-range I/O size has been allocated (total).	*/
#define	MP_MEMORY_ERR						0x470B1532		/*	�R���g���[���������s��								*/
															/*	Controller memory shortage							*/
#define	MP_NOTPTR							0x470B1533		/*	�����ȃ|�C���^ (�ʐM�f�o�C�X�w��\����/�f�o�C�X�ŗL���/�ΏےʐM�f�o�C�X�n���h���ւ̃|�C���^�ُ�)													*/
															/*	Invalid pointer (communication device specification structure/device inherent information/pointer error to objective communication device handle)	*/
#define	MP_NOTTIMEOUTVALUE					0x470B1534		/*	�����ȃ^�C���A�E�g�l								*/
															/*	Invalid time-out value								*/
#define	MP_TABLEOVER						0x43001800		/*	�C�x���g���o�e�[�u�����\�[�X���擾�ł��Ȃ�			*/
															/*	Event detection table resource cannot be got.		*/
#define	MP_ALARM							0x43001801		/*	�A���[������										*/
															/*	Alarm has occurred.									*/
#define	MP_MEMORYOVER						0x43001802		/*	���������\�[�X���l���ł��Ȃ�						*/
															/*	Memory resource cannot be got.						*/
#define	MP_EXEC_ERR							0x470B1803		/*	�A���[������										*/
															/*	System execution error								*/
#define	MP_NOTLOGICALAXIS					0x470B1804		/*	�A���[������										*/
															/*	Logical axis number error							*/
#define	MP_NOTSUPPORTED						0x470B1805		/*	���T�|�[�g											*/
															/*	Not supported										*/
#define	MP_ILLTEXT							0x470B1806		/*	�L���͈͊O�̒����̕��������						*/
															/*	Out-of-range length of character string was input.	*/
#define	MP_NOFILENAME						0x470B1807		/*	�t�@�C���������ݒ�									*/
															/*	File name has not been set.							*/
#define	MP_NOTREGSTERNAME					0x470B1808		/*	�w�肳�ꂽ���W�X�^�[�f�[�^�����Ȃ�					*/
															/*	Specified register data name cannot be found.		*/
#define	MP_FILEALREADYOPEN					0x4B0B1809		/*	�����t�@�C�������ɃI�[�v������Ă���				*/
															/*	The same file has already been opened.				*/
#define	MP_NOFILEPACKET						0x470B180a		/*	�w�肳�ꂽ�\�[�X�t�@�C���p�P�b�g���Ȃ�				*/
															/*	Specified source file packet cannot be found.		*/
#define	MP_NOTFILEPACKETSIZE				0x470B180b		/*	�\�[�X�t�@�C���p�P�b�g�̃T�C�Y���Ⴄ				*/
															/*	Source file packet size is incorrect.				*/
#define	MP_NOTUSERFUNCTION					0x4B0B180c		/*	�w�肳�ꂽ���[�U�֐������݂��Ȃ�					*/
															/*	Specified user funtion does not exist.				*/
#define	MP_NOTPARAMETERTYPE					0x4B0B180d		/*	�w�肳�ꂽ���[�U�֐������݂��Ȃ�					*/
															/*	Specified parameter type does not exist.			*/
#define	MP_ILLREGHANDLETYPE					0x470B180e		/*	�ُ�ȃ��W�X�^�n���h����ʂ̎w��					*/
															/*	Erroneous register handle type specified.			*/
#define	MP_ILLREGTYPE						0x470B1810		/*	�ُ�ȃ��W�X�^��ʂ̎w��							*/
															/*	Erroneous register type specified.					*/
#define	MP_ILLREGSIZE						0x470B1811		/*	�ُ�ȃ��W�X�^�T�C�Y�̎w��(WORD�ȊO)				*/
															/*	Erroneous register size specified.(other than WORD)	*/
#define	MP_REGNUMOVER						0x470B1812		/*	���W�X�^�L���͈͊O									*/
															/*	Out-of-range register								*/
#define	MP_RELEASEWAIT						0x470B1813		/*	�҂���ԉ���										*/
															/*	Waiting status released								*/
#define	MP_PRIORITYOVER						0x470B1814		/*	�ݒ�ł��Ȃ�Priority								*/
															/*	Priority that can not be set						*/
#define	MP_NOTCHANGEPRIORITY				0x470B1815		/*	�҂���ԉ���										*/
															/*	Priority that cannot be changed						*/
#define	MP_SEMAPHOREOVER					0x470B1816		/*	�Z�}�z��`�I�[�o�[									*/
															/*	Semaphore definition over							*/
#define	MP_NOTSEMAPHOREHANDLE				0x470B1817		/*	����`�̃Z�}�z�n���h���w��							*/
															/*	Undefined semaphore handle specification			*/
#define	MP_SEMAPHORELOCKED					0x470B1818		/*	�w��Z�}�z�n���h�����b�N��							*/
															/*	Specified semaphore handle being locked				*/
#define	MP_CONTINUE_RELEASEWAIT				0x470B1819		/*	ymcContinueWaitForCompletion���̑҂���ԉ���		*/
															/*	Waiting status released during ymcContinueWaitForCompletion	*/
#define	MP_NOTTABLENAME						0x4B0B1820		/*	����`��Table����									*/
															/*	Undefined Table name								*/
#define	MP_ILLTABLETYPE						0x470B1821		/*	�s����TableType										*/
															/*	Illegal Table Type									*/
#define	MP_TIMEOUTVALUEOVER					0x470B1822		/*	�͈͊O�̃^�C���A�E�g�l���w�肳�ꂽ					*/
															/*	Out-of-range timeout value has been specified		*/
#define	MP_SYSTASK_CALL_ERR					0x470B1823		/*	call�s�ȃV�X�e���^�X�N����call���ꂽ				*/
															/*														*/
#define	MP_CANNOTUSE_APICALL				0x470B19f0		/*	���Ή�API��call���ꂽ								*/
															/*														*/
#define	MP_OTHER_ERR						0x470B19ff		/*	���̑��̃G���[										*/
															/*	Other errors										*/

#endif	/*	#ifndef	_ERROR_CODE_H	*/
