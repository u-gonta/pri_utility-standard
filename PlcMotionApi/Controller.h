#pragma once

#include "Exception/Convert.h"
#include "WrapperMotionApi/include/ErrorCode.h"
#include "WrapperMotionApi/Exception.h"
#include "WrapperMotionApi/WrapperMotionApi.h"
#include "WrapperMotionApi/Result.h"
#include "Setting.h"
#include "SettingController.h"
#include "SettingRegister.h"
#include "SettingAxis.h"
#include "SettingDevice.h"
#include "Register.h"
#include "Result.h"

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

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���g���[����������
		/// @param[in]		object �R���g���[��������������ݒ�N���X
		/// @return			�R���g���[�����������������ʂ�ێ�����N���X
		////////////////////////////////////////////////////////////////////////////////
		inline Controller::Result::CInitialize InitializeController(const Controller::Setting::CInitialize& object)
		{
			Controller::Result::CInitialize ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::InitializeController()" << Logging::ConstSeparator;

			try
			{
				// �R���g���[�����J���ݒ�N���X
				MotionApi::Controller::Setting::COpen setting = object.Open;

				// �R���g���[���Ɛڑ�
				postscript << "MotionApi::OpenController()";
				ret.Controller = MotionApi::OpenController(setting);
				// ���O�o��
				postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(ret.Controller.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// Api�^�C���A�E�g�̐ݒ�N���X
				MotionApi::Controller::Setting::CApiTimeout apiTimeout = object.ApiTimeout;

				// Api�̃^�C���A�E�g��ݒ�
				postscript << "MotionApi::SetApiTimeout()" << Logging::ConstSeparator << "�^�C���A�E�g:" << apiTimeout.Value;
				MotionApi::SetApiTimeout(apiTimeout);
				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���g���[����j��
		/// @param[in]		object �R���g���[����j������ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		inline void DestroyController(const Controller::Setting::CDestroy& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::DestroyController()" << Logging::ConstSeparator;

			try
			{
				do
				{
					// �R���g���[�������ݒ�N���X
					MotionApi::Controller::Setting::CClose setting = object.Close;

					// �n���h�����m�F
					if (setting.Handle == MotionApi::ConstDisconnected)
					{
						// ���O�o��
						postscript << Logging::ConstSeparator << "��������";
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
						break;
					}

					// �R���g���[���Ɛؒf
					postscript << "MotionApi::CloseController()";
					MotionApi::CloseController(setting);
					// ���O�o��
					postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(setting.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				} while (false);
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���W�X�^�̒l���擾
		/// @param[in]		object ���W�X�^�̒l���擾����ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		inline Register::Result::CContent GetRegister(const Register::Setting::CGetContent& object)
		{
			Register::Result::CContent ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcRead;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::GetRegister()" << Logging::ConstSeparator;

			try
			{
				Register::CSpecify specify(object.Category);

				// �ԍ����X�V
				specify.Index = object.Index;

				// �f�[�^�̃^�C�v
				auto type = specify.GetType();
				// �擾���鐔
				unsigned long size = object.Size;
				// �A�h���X
				std::string address = specify.GetAddress();

				// ���W�X�^�̃f�[�^�^�C�v���m�F
				if (Register::ConstTypeBit == type)
				{
					// �r�b�g�^ �� ���W�X�^���擾����T�C�Y�����[�h�^�̃T�C�Y�ōX�V
					size /= 16;
					// ���擪��0�r�b�g�ȊO���l������2�]���Ɋm��
					size += 2;
				}
				else if (Register::ConstTypeLong == type)
				{
					// �����O�^ �� ���W�X�^���擾����T�C�Y�����[�h�^�̃T�C�Y�ōX�V
					size *= 2;
				}

				// �f�[�^�̃^�C�v�����[�h�^�ɍX�V
				postscript << "Register::CSpecify::ChangeType(Register::ConstTypeWord)";
				specify.ChangeType(Register::ConstTypeWord);
				postscript.str("");

				MotionApi::Register::Setting::CDeclare declare;

				// ���W�X�^�̃A�h���X���X�V
				postscript << "Register::CSpecify::GetAddress()";
				declare.Address = specify.GetAddress();
				postscript.str("");

				// ���W�X�^�̃n���h�����쐬
				postscript << "MotionApi::DeclareRegister()";
				postscript << Logging::ConstSeparator << "�A�h���X:" << declare.Address;
				auto result = MotionApi::DeclareRegister(declare);
				// ���O�o��
				postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(result.Handle);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				MotionApi::Register::Setting::CGetContent setting;

				// �n���h�����X�V
				setting.Handle = result.Handle;

				// �f�[�^�����X�V
				setting.Size = size;

				// ���W�X�^�̒l���擾
				postscript << "MotionApi::GetRegisterData()" << Logging::ConstSeparator << "�n���h��:" << FormatHandle(setting.Handle);
				postscript << Logging::ConstSeparator << "�v���T�C�Y:" << setting.Size;
				auto content = MotionApi::GetRegisterData(setting);
				// ���O�o��
				postscript << Logging::ConstSeparator << "�擾�T�C�Y:" << content.Size;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// �f�[�^�𑖍�
				for (unsigned long index = 0; index < content.Size; index++)
				{
					unsigned long target = index;

					// ���W�X�^�̃f�[�^�^�C�v���m�F
					if (Register::ConstTypeBit == type)
					{
						// �r�b�g�^ �� ���W�X�^�̐擪�r�b�g�ԍ����擾
						auto first = std::stoi(std::string(&address.back()), nullptr, 16);
						// ���擪�̃��W�X�^�ȊO��0�r�b�g�ڂ��擪
						auto bit = index == 0 ? first : 0;

						// 1���[�h���𑖍�
						while (bit < 16)
						{
							// �f�[�^�̊i�[����m��
							target = (index * 16) + bit - first;

							// �i�[����m�F
							if (object.Size <= target)
							{
								// �T�C�Y�𒴂��� �� �����𔲂���
								break;
							}

							// �f�[�^���i�[
							ret.Bits[target] = content.Buffers.at(index) >> bit & 0x1;

							// ���̃r�b�g��
							bit++;
						}
					}
					else if (Register::ConstTypeWord == type)
					{
						// ���[�h�^ �� �f�[�^���i�[
						ret.Words[target] = content.Buffers.at(index);
					}
					else if (Register::ConstTypeLong == type)
					{
						// �����O�^ �� �f�[�^�̊i�[����m��
						target = index / 2 * 2;
						// �f�[�^���m�F
						if (ret.Longs.count(target) <= 0)
						{
							// ���o�^
							ret.Longs[target];
						}

						// �i�[����m�F
						if (index % 2)
						{
							// ��ʃ��[�h �� 1���[�h���V�t�g���čX�V
							ret.Longs[target] |= ((unsigned long)content.Buffers.at(index)) << 16;
						}
						else
						{
							// ���ʃ��[�h
							ret.Longs[target] |= (unsigned long)content.Buffers.at(index);
						}
					}
				}
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���W�X�^�̒l���X�V
		/// @param[in]		object	���W�X�^�̒l���X�V����ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		inline void SetRegister(const Register::Setting::CSetContent& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcWrite;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::SetRegister()" << Logging::ConstSeparator;

			try
			{
				Register::CSpecify specify(object.Category);

				// �ԍ����X�V
				specify.Index = object.Index;

				// �f�[�^�̃^�C�v
				auto type = specify.GetType();
				// �X�V���鐔
				unsigned long size = (unsigned long)object.Values.size();
				// �������މ�
				unsigned long loop = 1;

				// ���W�X�^�̃f�[�^�^�C�v���m�F
				if (Register::ConstTypeBit == type)
				{
					// �r�b�g�^ �� 1�f�[�^���̃T�C�Y�ōX�V
					size = 1;
					// �J��Ԃ��񐔂��m��
					loop = size;
				}
				else if (Register::ConstTypeLong == type)
				{
					// �����O�^ �� ���W�X�^���X�V����T�C�Y�����[�h�^�̃T�C�Y�ōX�V
					size *= 2;
					// �f�[�^�̃^�C�v�����[�h�^�ɍX�V
					specify.ChangeType(Register::ConstTypeWord);
				}

				for (unsigned long index = 0; index < loop; index++)
				{
					MotionApi::Register::Setting::CDeclare declare;

					// ���W�X�^�̃A�h���X���X�V
					postscript << "Register::CSpecify::GetAddress()";
					declare.Address = specify.GetAddress();
					postscript.str("");

					// ���W�X�^�̃n���h�����쐬
					postscript << "MotionApi::DeclareRegister()";
					postscript << Logging::ConstSeparator << "�A�h���X:" << declare.Address;
					auto result = MotionApi::DeclareRegister(declare);
					// ���O�o��
					postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(result.Handle);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					MotionApi::Register::Setting::CSetContent setting;

					// �n���h�����X�V
					setting.Handle = result.Handle;

					// ���W�X�^�̃f�[�^�^�C�v���m�F
					if (Register::ConstTypeBit == type)
					{
						// �r�b�g�^	�� ���W�X�^�̃r�b�g�ԍ����擾
						auto first = std::stoi(std::string(&specify.GetAddress().back()), nullptr, 16);
						// ���擪�̃��W�X�^�ȊO��0�r�b�g�ڂ��擪
						auto bit = index == 0 ? first : 0;

						// �f�[�^���i�[
						setting.Buffers.emplace_back(object.Values[index]);
					}
					else if (Register::ConstTypeWord == type || Register::ConstTypeLong == type)
					{
						// ���[�h�^ or �����O�^ �� �f�[�^���i�[
						std::copy(object.Values.begin(), object.Values.end(), std::back_inserter(setting.Buffers));
					}

					// �f�[�^�����X�V
					setting.Size = size;

					// ���W�X�^�̒l���X�V
					postscript << "MotionApi::SetRegisterData()" << Logging::ConstSeparator << "�n���h��:" << FormatHandle(setting.Handle);
					postscript << Logging::ConstSeparator << "�X�V�T�C�Y:" << setting.Size;
					MotionApi::SetRegisterData(setting);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			����������
		/// @param[in]		object ��������������ݒ�N���X
		/// @return			�����������������ʃN���X
		////////////////////////////////////////////////////////////////////////////////
		inline Axis::Result::CInitialize InitializeAxis(const Axis::Setting::CInitialize& object)
		{
			Axis::Result::CInitialize ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::InitializeAxis()" << Logging::ConstSeparator;

			try
			{
				// ���̃n���h�����쐬����ݒ�N���X
				MotionApi::Axis::Setting::CDeclare setting = object.Declare;

				// ���̃n���h�����쐬
				postscript << "MotionApi::DeclareAxis()";
				postscript << Logging::ConstSeparator << "�X���b�gNo:" << object.Declare.SlotNo;
				postscript << Logging::ConstSeparator << "�T�u�X���b�gNo:" << object.Declare.SubSlotNo;
				postscript << Logging::ConstSeparator << "�X�e�[�V����No:" << object.Declare.StationNo;
				postscript << Logging::ConstSeparator << "�_����No:" << object.Declare.LogicalNo;
				postscript << Logging::ConstSeparator << "���^�C�v:" << object.Declare.Type;
				ret.Axis = MotionApi::DeclareAxis(setting);
				// ���O�o��
				postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(ret.Axis.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			����j��
		/// @param[in]		object ����j������ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		inline void DestroyAxis(const Axis::Setting::CDestroy& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::DestroyAxis()" << Logging::ConstSeparator;

			try
			{
				// ���̃n���h����j��
				postscript << "MotionApi::ClearAllAxis()";
				MotionApi::ClearAllAxis();
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�o�C�X�̃n���h�����쐬
		/// @param[in]		logging ���O�̏���ێ�����N���X
		/// @param[in]		object �f�o�C�X�̃n���h�����쐬����ݒ�N���X
		/// @return			�f�o�C�X�̃n���h�����쐬�������ʃN���X
		////////////////////////////////////////////////////////////////////////////////
		MotionApi::Device::Result::CDeclare DeclareDevice(const Logging::CObject& logging, const MotionApi::Device::Setting::CDeclare& object)
		{
			MotionApi::Device::Result::CDeclare ret;
			std::stringstream postscript;

			// �f�o�C�X�̃n���h�����쐬
			postscript << "MotionApi::DeclareDevice()";
			postscript << Logging::ConstSeparator << "���n���h��:";
			for (const auto& handle : object.Handles)
			{
				// �n���h�����擾
				postscript << Logging::ConstSeparator << FormatHandle(handle);
			}
			ret = MotionApi::DeclareDevice(object);
			// ���O�o��
			postscript << Logging::ConstSeparator << "�f�o�C�X�n���h��:" << FormatHandle(ret.Handle);
			Transfer::Output(Logging::Join(logging, postscript.str()));
			postscript.str("");

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�o�C�X�̃n���h�����N���A
		/// @param[in]		logging ���O�̏���ێ�����N���X
		/// @param[in]		object ���̃n���h�����쐬�������ʃN���X
		////////////////////////////////////////////////////////////////////////////////
		void ClearDevice(const Logging::CObject& logging, const MotionApi::Device::Result::CDeclare& object)
		{
			std::stringstream postscript;

			MotionApi::Device::Setting::CClear setting;

			// �n���h�����X�V
			setting.Handle = object.Handle;

			// �f�o�C�X�̃n���h�����N���A
			postscript << "MotionApi::ClearDevice()";
			postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(setting.Handle);
			MotionApi::ClearDevice(setting);
			// ���O�o��
			Transfer::Output(Logging::Join(logging, postscript.str()));
			postscript.str("");
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�T�[�{�𐧌�
		/// @param[in]		object �T�[�{�̐�����s���ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		inline void AxisServo(const Device::Setting::CServoControl& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisWrite;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::AxisServo()" << Logging::ConstSeparator;

			try
			{
				// �f�o�C�X�̃n���h�����쐬
				postscript << "DeclareDevice()";
				auto result = DeclareDevice(logging, object.Device);
				postscript.str("");

				try
				{
					// �T�[�{�̐�����s���ݒ�N���X
					MotionApi::Device::Setting::CServo setting = object.Status;

					// �n���h�����X�V
					setting.Handle = result.Handle;

					// �T�[�{�𐧌�
					postscript << "MotionApi::ServoControl()";
					postscript << Logging::ConstSeparator << "�n���h��:" << Plc::FormatHandle(setting.Handle);
					postscript << Logging::ConstSeparator << "���:" << (setting.Mode == Signal::StatusOn ? "ON" : "OFF");
					postscript << Logging::ConstSeparator << "�^�C���A�E�g:" << setting.Timeout;
					MotionApi::ServoControl(setting);
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �f�o�C�X�̃n���h�����N���A
					postscript << "ClearDevie()";
					ClearDevice(logging, result);
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �f�o�C�X�̃n���h�����N���A
					ClearDevice(logging, result);
					throw;
				}
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�W���O�����s
		/// @param[in]		object �W���O�����s����ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		inline void AxisStartJog(const Device::Setting::CStartJog& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisWrite;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::AxisStartJog()" << Logging::ConstSeparator;

			try
			{
				MotionApi::Device::Setting::CStartJog setting = object.Device;

				// �f�o�C�X�̃n���h�����쐬
				postscript << "DeclareDevice()";
				auto result = DeclareDevice(logging, setting.GetDeclare());
				postscript.str("");

				try
				{
					// �n���h�����X�V
					setting.Handle = result.Handle;

					// ���𑖍�
					for (const auto& motion : setting.Motions)
					{
						postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(motion.Handle);
						postscript << Logging::ConstSeparator << "���W�n:" << motion.Coordinate;
						postscript << Logging::ConstSeparator << "����^�C�v:" << motion.MoveType;
						postscript << Logging::ConstSeparator << "���x�^�C�v:" << motion.VelocityType;
						postscript << Logging::ConstSeparator << "�������^�C�v:" << motion.AccDecType;
						postscript << Logging::ConstSeparator << "�������t�B���^�^�C�v:" << motion.FilterType;

						// �f�[�^�^�C�v�𑖍�
						for (const auto& iterator : MotionApi::Device::DataIterator())
						{
							// �f�[�^�^�C�v�̓o�^���m�F
							if (motion.Datas.count(iterator) <= 0)
							{
								// �f�[�^�^�C�v�����o�^ �� ���̃f�[�^�^�C�v��
								continue;
							}

							postscript << Logging::ConstSeparator;

							switch (iterator)
							{
							case MotionApi::Device::DataMaxVelocity:
								postscript << "���x�̏���l:";
								break;

							case MotionApi::Device::DataAcceleration:
								postscript << "�����x:";
								break;

							case MotionApi::Device::DataDeceleration:
								postscript << "�����x:";
								break;

							case MotionApi::Device::DataFilterTime:
								postscript << "�t�B���^����:";
								break;

							case MotionApi::Device::DataVelocity:
								postscript << "���x:";
								break;

							case MotionApi::Device::DataApproachVelocity:
								postscript << "���_���A�̃A�v���[�`���x:";
								break;

							case MotionApi::Device::DataCreepVelocity:
								postscript << "���_���A�̃N���[�v���x:";
								break;
							}
							postscript << motion.Datas.at(iterator);
						}
						postscript << Logging::ConstSeparator << "����:" << motion.Direction;
						postscript << Logging::ConstSeparator << "�^�C���A�E�g:" << motion.Timeout;
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// �W���O�����s
					postscript << "MotionApi::StartJog()";
					postscript << Logging::ConstSeparator << "�n���h��:" << Plc::FormatHandle(setting.Handle);
					MotionApi::StartJog(setting);
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �f�o�C�X�̃n���h�����N���A
					postscript << "ClearDevie()";
					ClearDevice(logging, result);
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �f�o�C�X�̃n���h�����N���A
					ClearDevice(logging, result);
					throw;
				}
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�W���O���~
		/// @param[in]		object �W���O���~����ݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		inline void AxisStopJog(const Device::Setting::CStopJog& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisWrite;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::AxisStopJog()" << Logging::ConstSeparator;

			try
			{
				MotionApi::Device::Setting::CStopJog setting = object.Device;

				// �f�o�C�X�̃n���h�����쐬
				postscript << "DeclareDevice()";
				auto result = DeclareDevice(logging, setting.GetDeclare());
				postscript.str("");

				try
				{
					// �n���h�����X�V
					setting.Handle = result.Handle;

					// ���𑖍�
					for (const auto& motion : setting.Motions)
					{
						postscript << Logging::ConstSeparator << "��:" << FormatHandle(motion.Handle);
						postscript << Logging::ConstSeparator << "����:" << motion.Completed;
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// �W���O���~
					postscript << "MotionApi::StopJog()";
					postscript << Logging::ConstSeparator << "�n���h��:" << Plc::FormatHandle(setting.Handle);
					MotionApi::StopJog(setting);
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �f�o�C�X�̃n���h�����N���A
					postscript << "ClearDevie()";
					ClearDevice(logging, result);
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �f�o�C�X�̃n���h�����N���A
					ClearDevice(logging, result);
					throw;
				}
			}
			catch (const MotionApi::CException& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O�𔭖C
				throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
			}
		}
	}
}