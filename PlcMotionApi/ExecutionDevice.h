#pragma once

#include "Common/Io/Status.h"
#include "Common/Io/Setting.h"
#include "Execution.h"
#include "SettingDevice.h"
#include "Result.h"

#include "PlcMotionApi/ExecutionRegister.h"

namespace Standard
{
	namespace Plc
	{
		namespace Device
		{
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
				for (auto iterator = object.Handles.begin(); iterator != object.Handles.end(); iterator++)
				{
					// �n���h���̏��Ԃ��擪���ォ�m�F
					if (0 < std::distance(object.Handles.begin(), iterator))
					{
						postscript << Logging::ConstSeparator;
					}
					postscript << FormatHandle(*iterator);
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
			/// @brief			������~
			/// @param[in]		object ������~����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			inline void Suspend(const Device::Setting::CSuspend& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Suspend()" << Logging::ConstSeparator;

				try
				{
					// ���𑖍�
					for (const auto& axis : object.Axises)
					{
						MotionApi::Register::Setting::CDeclare declare;

						postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(axis.Handle);
						Io::Specify::CAddress address;
						address.Category = Plc::Register::ConstCategoryO;
						address.Category += Plc::Register::ConstTypeWord;
						address.Hex = true;
						address.Margin = 4;
						address.Index = axis.Address + 8;
						postscript << Logging::ConstSeparator << "�A�h���X:" << Io::Specify::Format(address);
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// ���W�X�^�̃A�h���X���X�V
						Register::CSpecify specify(address.Category);
						specify.Index = address.Index;
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

						// �f�[�^���i�[
						setting.Buffers.emplace_back(0);

						// �f�[�^�����X�V
						setting.Size = 1;

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
			/// @brief			�T�[�{�𐧌�
			/// @param[in]		object �T�[�{�̐�����s���ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			inline void Servo(const Device::Setting::CServo& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Servo()" << Logging::ConstSeparator;

				try
				{
					// �f�o�C�X�̃n���h�����쐬
					postscript << "DeclareDevice()";
					auto result = DeclareDevice(logging, object);
					postscript.str("");

					try
					{
						// �T�[�{�̐�����s���ݒ�N���X
						MotionApi::Device::Setting::CServo setting = object;

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
			/// @brief			���_���A�����s
			/// @param[in]		object ���_���A�����s����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			inline void MoveOrigin(const Device::Setting::CMoveOrigin& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "MoveOrigin()" << Logging::ConstSeparator;

				try
				{
					MotionApi::Device::Setting::CMoveOrigin setting = object;

					// �f�o�C�X�̃n���h�����쐬
					postscript << "DeclareDevice()";
					auto result = DeclareDevice(logging, setting.GetDeclare());
					postscript.str("");

					try
					{
						// �n���h�����X�V
						setting.Handle = result.Handle;

						// ���𑖍�
						for (const auto& axis : setting.Axises)
						{
							postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(axis.Handle);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert((MotionApi::Device::Setting::Motion::CTemplate)axis);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert((MotionApi::Device::Setting::Position::CTemplate)axis);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert(axis.Method);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert(axis.Direction);
							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
						}

						// ���_���A�����s
						postscript << "MotionApi::MoveOrigin()";
						postscript << Logging::ConstSeparator << "�n���h��:" << Plc::FormatHandle(setting.Handle);
						MotionApi::MoveOrigin(setting);
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
			/// @brief			�W���O���J�n
			/// @param[in]		object �W���O���J�n����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			inline void StartJog(const Device::Setting::CStartJog& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "StartJog()" << Logging::ConstSeparator;

				try
				{
					MotionApi::Device::Setting::CStartJog setting = object;

					// �f�o�C�X�̃n���h�����쐬
					postscript << "DeclareDevice()";
					auto result = DeclareDevice(logging, setting.GetDeclare());
					postscript.str("");

					try
					{
						// �n���h�����X�V
						setting.Handle = result.Handle;

						// ���𑖍�
						for (const auto& axis : setting.Axises)
						{
							postscript << Logging::ConstSeparator << "�n���h��:" << FormatHandle(axis.Handle);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert(axis);
							postscript << Logging::ConstSeparator << "����:" << axis.Direction;
							postscript << Logging::ConstSeparator << "�^�C���A�E�g:" << axis.Timeout;
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
			inline void StopJog(const Device::Setting::CStopJog& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "StopJog()" << Logging::ConstSeparator;

				try
				{
					MotionApi::Device::Setting::CStopJog setting = object;

					// �f�o�C�X�̃n���h�����쐬
					postscript << "DeclareDevice()";
					auto result = DeclareDevice(logging, setting.GetDeclare());
					postscript.str("");

					try
					{
						// �n���h�����X�V
						setting.Handle = result.Handle;

						// ���𑖍�
						for (const auto& axis : setting.Axises)
						{
							postscript << Logging::ConstSeparator << "��:" << FormatHandle(axis.Handle);
							postscript << Logging::ConstSeparator << "����:" << axis.Complete;
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
}