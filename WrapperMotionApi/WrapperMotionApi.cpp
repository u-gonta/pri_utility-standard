#include "pch.h"

#include <sstream>

#include "Common/Tool/Text.h"

#include "Exception.h"
#include "WrapperMotionApi.h"

#include "ymcPCAPI.h"

#if defined(_WIN64)
#pragma comment(lib, "ymcPCAPI_x64.lib")
#else
#pragma comment(lib, "ymcPCAPI.lib")
#endif

namespace Standard
{
	namespace MotionApi
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���g���[���ɐڑ�
		/// @param[in]		object	�R���g���[�����J���ݒ�̃N���X
		/// @return			�R���g���[�����J�������ʃN���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Controller::Result::COpen OpenController(const Controller::Setting::COpen& object)
		{
			Controller::Result::COpen ret;
			std::stringstream postscript;

			// COM_DEVICE�\����
			COM_DEVICE device;

			// �\���̂�������
			::memset(&device, 0, sizeof(COM_DEVICE));

			// �ʐM���
			device.ComDeviceType = (WORD)object.Device;

			// �|�[�g�ԍ�
			device.PortNumber = 1;

			// CPU�ԍ�
			device.CpuNumber = 1;

			// �ʐM�^�C���A�E�g
			device.Timeout = 10000;

			HCONTROLLER result;

			// �R���g���[���Ɛڑ�
			postscript << "ymcOpenController()";
			result = ymcOpenController(&device, &ret.Handle);

			// �ڑ����m�F
			if (result != MP_SUCCESS)
			{
				// �ڑ��Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			Api�̃^�C���A�E�g��ݒ�
		/// @param[in]		object	Api�̃^�C���A�E�g�ݒ�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void SetApiTimeout(const Controller::Setting::CApiTimeout& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// ���[�V����Api�̃^�C���A�E�g��ݒ�
			postscript << "ymcSetAPITimeoutValue()";
			result = ymcSetAPITimeoutValue((LONG)object.Value);

			// �ݒ���m�F
			if (result != MP_SUCCESS)
			{
				// �ݒ�Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���g���[���Ɛؒf
		/// @param[in]		object	�R���g���[�������ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void CloseController(const Controller::Setting::CClose& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// �R���g���[���Ɛؒf
			postscript << "ymcCloseController()";
			result = ymcCloseController(object.Handle);

			// �ؒf���m�F
			if (result != MP_SUCCESS)
			{
				// �ؒf�Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���W�X�^�̃n���h�����쐬
		/// @param[in]		object	���W�X�^�̃n���h�����쐬����ݒ�̃N���X
		/// @return			���W�X�^�̃n���h�����쐬�������ʃN���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Register::Result::CDeclare DeclareRegister(const Register::Setting::CDeclare& object)
		{
			Register::Result::CDeclare ret;
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// ���W�X�^�̃n���h�����擾
			postscript << "ymcGetRegisterDataHandle()";
			result = ymcGetRegisterDataHandle((unsigned char *)object.Address.c_str(), &ret.Handle);

			// �擾���m�F
			if (result != MP_SUCCESS)
			{
				// �擾�Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���W�X�^�̒l���擾
		/// @param[in]		object	���W�X�^�̒l���擾����ݒ�N���X
		/// @return			���W�X�^�̒l�N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Register::Result::CContent GetRegisterData(const Register::Setting::CGetContent& object)
		{
			Register::Result::CContent ret;
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// �o�b�t�@�����X�V
			ret.Buffers.resize(object.Size);

			// ���W�X�^�̒l���擾
			postscript << "ymcGetRegisterData()";
			result = ymcGetRegisterData(object.Handle, object.Size, ret.Buffers.data(), &ret.Size);

			// �擾���m�F
			if (result != MP_SUCCESS)
			{
				// �擾�Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���W�X�^�̒l���X�V
		/// @param[in]		object	���W�X�^�̒l���X�V����ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void SetRegisterData(const Register::Setting::CSetContent& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// ���W�X�^�̒l���X�V
			postscript << "ymcSetRegisterData()";
			result = ymcSetRegisterData(object.Handle, object.Size, (void*)object.Buffers.data());

			// �擾���m�F
			if (result != MP_SUCCESS)
			{
				// �擾�Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���̃n���h��������
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void ClearAllAxis()
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// ���̃n���h��������
			postscript << "ymcClearAllAxes()";
			result = ymcClearAllAxes();

			// �������m�F
			if (result != MP_SUCCESS)
			{
				// �����Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���̃n���h�����쐬
		/// @param[in]		object	���̃n���h�����쐬����ݒ�̃N���X
		/// @return			���̃n���h�����쐬�������ʃN���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Axis::Result::CDeclare DeclareAxis(const Axis::Setting::CDeclare& object)
		{
			Axis::Result::CDeclare ret;
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;
			std::stringstream name;

			name << "Axis" << Text::Format(2, object.LogicalNo);

			// ���̃n���h�����擾
			postscript << "ymcDeclareAxis()";
			result = ymcDeclareAxis(1, object.SlotNo, object.SubSlotNo, object.StationNo, object.LogicalNo, (WORD)object.Type, (LPBYTE)name.str().c_str(), &ret.Handle);

			// �擾���m�F
			if (result != MP_SUCCESS)
			{
				// �擾�Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���̃n���h�����N���A
		/// @param[in]		object	���̃n���h����j������ݒ�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void ClearAxis(const Axis::Setting::CClear& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// ���̃n���h�����N���A
			postscript << "ymcClearAxis()";
			result = ymcClearAxis(object.Handle);

			// �j�����m�F
			if (result != MP_SUCCESS)
			{
				// �j���Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�o�C�X�̃n���h�����쐬
		/// @param[in]		object	�f�o�C�X�̃n���h�����쐬����ݒ�̃N���X
		/// @return			�f�o�C�X�̃n���h�����쐬�������ʃN���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Device::Result::CDeclare DeclareDevice(const Device::Setting::CDeclare& object)
		{
			Device::Result::CDeclare ret;
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			std::vector<HAXIS> axises;

			// ���𑖍�
			for (const auto& handle : object.Handles)
			{
				// ���̃n���h����ǉ�
				axises.emplace_back(handle);
			}

			// �f�o�C�X�̃n���h�����擾
			postscript << "ymcDeclareDevice()";
			result = ymcDeclareDevice((WORD)axises.size(), axises.data(), &ret.Handle);

			// �擾���m�F
			if (result != MP_SUCCESS)
			{
				// �擾�Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�o�C�X�̃n���h�����N���A
		/// @param[in]		object	�f�o�C�X�̃n���h����j������ݒ�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void ClearDevice(const Device::Setting::CClear& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// �f�o�C�X�̃n���h�����N���A
			postscript << "ymcClearDevice()";
			result = ymcClearDevice(object.Handle);

			// �j�����m�F
			if (result != MP_SUCCESS)
			{
				// �j���Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�T�[�{�𐧌�
		/// @param[in]		object �T�[�{�̐�����s���ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void ServoControl(const Device::Setting::CServo& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// �T�[�{�𐧌�
			postscript << "ymcServoControl()";
			result = ymcServoControl(object.Handle, object.Mode == Signal::StatusOn ? SERVO_ON : SERVO_OFF, object.Timeout);

			// ������m�F
			if (result != MP_SUCCESS)
			{
				// ����Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���̐�����s���e���v���[�g�̐ݒ�N���X���烉�C�u�����֓n���ϐ��֕ϊ�
		/// @param[in]		object ���̐�����s���e���v���[�g�̐ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_DATA Convert(const Device::Setting::Motion::CTemplate& object)
		{
			MOTION_DATA ret;

			//! ���W�n
			ret.CoordinateSystem = (WORD)object.Coordinate;

			//! ����^�C�v
			ret.MoveType = (SHORT)object.MoveType;

			//! ���x�^�C�v
			ret.VelocityType = (SHORT)object.VelocityType;

			//! �������^�C�v
			ret.AccDecType = (SHORT)object.AccDecType;

			//! �������t�B���^�^�C�v
			ret.FilterType = (SHORT)object.FilterType;

			// �f�[�^�t���O
			ret.DataType = 0;

			// �f�[�^�t���O
			for (const auto& iterator : Device::DataIterator())
			{
				// �f�[�^�^�C�v�̓o�^���m�F
				if (object.Datas.count(iterator) <= 0)
				{
					// �f�[�^�^�C�v�����o�^ �� ���̃f�[�^�^�C�v��
					continue;
				}

				// �r�b�g��ON
				ret.DataType |= 0x01 << iterator;
				switch (iterator)
				{
				case Device::DataMaxVelocity:
					// ���x�̏���l�A����ō����x
					ret.MaxVelocity = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataAcceleration:
					// �����x[�w�ߒP��/sec^2]�^�������萔[ms]
					ret.Acceleration = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataDeceleration:
					// �����x[�w�ߒP��/sec^2]�^�������萔[ms]
					ret.Deceleration = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataFilterTime:
					// EnumFilter��FilterCurve�܂���FilterExp��I�����ɗL��
					ret.FilterTime = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataVelocity:
					// ���x
					ret.Velocity = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataApproachVelocity:
					// ���_���A�̃A�v���[�`���x
					ret.ApproachVelocity = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataCreepVelocity:
					// ���_���A�̃N���[�v���x
					ret.CreepVelocity = (LONG)object.Datas.at(iterator);
					break;
				}
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���̃W���O��������s
		/// @param[in]		object ���̃W���O�����s����ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void StartJog(const Device::Setting::CStartJog& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			std::vector<MOTION_DATA> datas;
			std::vector<WORD> directions;
			std::vector<WORD> timeouts;

			// ���𑖍�
			for (const auto& motion : object.Motions)
			{
				// ���̐�����s����Ԃ�ێ�����N���X���烉�C�u�����֓n���ϐ��֕ϊ�
				datas.emplace_back(Convert(motion));

				// ������ǉ�
				directions.emplace_back((WORD)motion.Direction);

				// �^�C���A�E�g��ǉ�
				timeouts.emplace_back((WORD)motion.Timeout);
			}

			// ���̃W���O��������s
			postscript << "ymcMoveJOG()";
			result = ymcMoveJOG(object.Handle, datas.data(), directions.data(), timeouts.data(), 0, NULL, 0);

			// ������m�F
			if (result != MP_SUCCESS)
			{
				// ����Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���̃W���O������~
		/// @param[in]		object ���̃W���O���~����ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void StopJog(const Device::Setting::CStopJog& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			std::vector<WORD> completeds;

			// ���𑖍�
			for (const auto& motion : object.Motions)
			{
				// �����^�C�v
				completeds.emplace_back((WORD)motion.Completed);
			}

			// ���̃W���O������~
			postscript << "ymcStopJOG()";
			result = ymcStopJOG(object.Handle, 0, NULL, completeds.data(), 0);

			// ������m�F
			if (result != MP_SUCCESS)
			{
				// ����Ɏ��s �� ��O�𔭖C
				throw CException(result, postscript.str());
			}
		}
	}
}