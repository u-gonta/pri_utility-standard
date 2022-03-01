#include "Poling.h"
#include "Common/Tool/Variant.h"
#include "Common/Io/Setting.h"

#include "PlcMotionApi/Parameter.h"
#include "PlcMotionApi/ExecutionController.h"
#include "PlcMotionApi/ExecutionRegister.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			CWorker::CWorker() : Worker::CTemplate()
			{
				m_controller.Handle = MotionApi::ConstDisconnected;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�f�X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			CWorker::~CWorker()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�ڑ��̏�Ԃ��擾
			/// @return			true:�ڑ��ς� / false:���ڑ�
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::GetConnected()
			{
				bool ret = false;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncController);

				// �n���h�����m�F
				if (m_controller.Handle != MotionApi::ConstDisconnected)
				{
					ret = true;
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�ڑ��ς݂��m�F
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::IsConnected()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoConnect;
				int deviceErrorCode = Exception::DeviceCodeSuccess;

				logging.Message << GetName() << "IsConnected()" << Logging::ConstSeparator;

				try
				{
					// �ڑ��̏�Ԃ��擾
					postscript << "GetConnected()";
					if (GetConnected() == false)
					{
						// ���ڑ� �� ��O�𔭖C
						throw std::exception("���ڑ�");
					}
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O��ʒm�����C
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			������
			/// @detail			���C���X���b�h���珉�����̃^�C�~���O�ŌĂяo�����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::InitializeDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CInitialize trigger;

				logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

				try
				{
					Poling::CSetting polingSetting = GetSetting();
					Plc::Controller::Setting::CInitialize setting;

					// ���̂��X�V
					setting.Name = GetName();

					// �R���g���[�����J���ݒ���擾
					postscript << "OpenController";
					boost::any buffer = polingSetting.Connect.Other.GetValue<Plc::EnumParameter>(Plc::ParameterOpenController);

					// �R���g���[�����J���ݒ肪�L�����m�F
					if (Variant::IsValue<std::string>(buffer))
					{
						// �R���g���[�����J���ݒ���X�V
						setting.Open.Input(Variant::Convert<std::string>(buffer));
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
					}
					postscript.str("");

					// Api�̃^�C���A�E�g���擾
					postscript << "ApiTimeout";
					buffer = polingSetting.Connect.Other.GetValue<Plc::EnumParameter>(Plc::ParameterApiTimeout);

					// Api�̃^�C���A�E�g���L�����m�F
					if (Variant::IsValue<std::string>(buffer))
					{
						// Api�̃^�C���A�E�g���X�V
						setting.ApiTimeout.Input(Variant::Convert<std::string>(buffer));
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
					}

					// �R���g���[����������
					postscript << "Plc::Controller::Initialize()";
					auto resultInitialize = Plc::Controller::Initialize(setting);
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncController);

					// �R���g���[���̏����X�V
					m_controller = resultInitialize.Controller;

					// �������̊������N��
					postscript << "WakeupInitializeComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupInitializeComplete(trigger);
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �������̊������N��
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupInitializeComplete(trigger);

					// ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�j��
			/// @detail			���C���X���b�h����j���̃^�C�~���O�ŌĂяo�����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::DestroyDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoDestroy;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CDestroy trigger;

				logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

				try
				{
					do
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncController);

						// �ڑ��̏�Ԃ��m�F
						if (GetConnected() == false)
						{
							// ���ڑ�
							postscript << "���ڑ�";
							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
							break;
						}

						Plc::Controller::Setting::CDestroy setting;

						// ���̂��X�V
						setting.Name = GetName();

						// �n���h�����X�V
						setting.Close.Handle = m_controller.Handle;

						// �R���g���[����j��
						postscript << "Plc::Controller::Destroy()";
						Plc::Controller::Destroy(setting);
						postscript.str("");

						// �n���h����������
						m_controller.Handle = MotionApi::ConstDisconnected;
					} while (false);

					// �j���̊������N��
					postscript << "WakeupDestroyComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupDestroyComplete(trigger);
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �j���̊������N��
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupDestroyComplete(trigger);

					// ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���g���C���m�F
			///	@detail			����ɓǂݍ��݂��s����܂Ő����񐔂̊Ԃ̓��g���C��L��
			///					���p�������֐�
			/// @return			true:���g���C���L�� / false:���g���C������
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::IsRetry()
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "IsRetry()" << Logging::ConstSeparator;

				Poling::CSetting polingSetting = GetSetting();
				int retry = GetRetry();

				logging.Message << "���g���C" << retry << "���" << Logging::ConstSeparator;

				// ���g���C�񐔂𔻒�
				if (retry <= polingSetting.Retry)
				{
					// ���g���C�񐔂��ݒ�ȉ� �� ���g���C��L��
					ret = true;
					logging.Message << "�L��";
				}
				else
				{
					// ���g���C�񐔂��ݒ���傫��
					logging.Message << "����";
				}
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�ǂݍ���
			/// @detail			���C���X���b�h����ǂݍ��݂̃^�C�~���O�ŌĂяo�����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::ReadDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoRead;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "ReadDerived()" << Logging::ConstSeparator;

				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncController);

					// �ڑ��ς݂��m�F
					postscript << "IsConnected()";
					IsConnected();
					postscript.str("");

					Poling::CSetting setting = GetSetting();

					Status::CInput input;

					// �^�͈̔͂𑖍�
					for (auto addressRange : setting.Connect.AddressRanges)
					{
						Plc::Register::CSpecify specify(addressRange.Start.Category);
						Plc::Register::Setting::CGetContent content;

						// ���̂��X�V
						content.Name = GetName();

						// ���W�X�^�̎�ʂ��m��
						content.Category = addressRange.Start.Category;
						// ���W�X�^�̔ԍ����m��
						content.Index = addressRange.Start.Index;
						// ���W�X�^�͈̔͂��m��
						content.Size = addressRange.Size;

						// ���W�X�^�̓ǂݍ���
						postscript << "Plc::Register::Get()";
						auto result = Plc::Register::Get(content);
						postscript.str("");

						// ���W�X�^�̒l���i�[
						for (const auto& bit : result.Bits)
						{
							// �r�b�g�^
							Io::Specify::CAddress address = addressRange.Start;

							// �A�h���X�̕ϊ����m��
							address.Hex = true;
							address.Margin = specify.GetMargin() + 1;

							// �A�h���X���m��
							address.Index += bit.first;
							input.Values[address] = (bool)(bit.second != 0 ? true : false);
						}
						for (const auto& numeric : result.Words)
						{
							Io::Specify::CAddress address = addressRange.Start;

							// �A�h���X�̕ϊ����m��
							address.Hex = true;
							address.Margin = specify.GetMargin();

							// �A�h���X���m��
							address.Index += numeric.first;
							input.Values[address] = (__int16)numeric.second;
						}
						for (const auto& numeric : result.Longs)
						{
							Io::Specify::CAddress address = addressRange.Start;

							// �A�h���X�̕ϊ����m��
							address.Hex = true;
							address.Margin = specify.GetMargin();

							// �A�h���X���m��
							address.Index += numeric.first;
							input.Values[address] = (__int32)numeric.second;
						}
					}

					// �ǂݍ��݂�ʒm
					NotifyArgument(input);

					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			��������
			/// @detail			���C���X���b�h���珑�����݂̃^�C�~���O�ŌĂяo�����
			///					���p�������֐�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::WriteDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeIoWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "WriteDerived()" << Logging::ConstSeparator;

				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				try
				{
					std::lock_guard<std::recursive_mutex> lock(m_asyncController);

					// �ڑ��ς݂��m�F
					postscript << "IsConnected()";
					IsConnected();
					postscript.str("");

					do
					{
						// �������݃N���X���擾
						auto write = GetWriteFirst();

						// �������݃N���X���m�F
						if (write == nullptr)
						{
							break;
						}

						// ���W�X�^�̒l���X�V
						for (const auto& entity : write->Entitys)
						{
							Plc::Register::CSpecify specify(entity.Category);
							Plc::Register::Setting::CSetContent setting;

							// ���̂��X�V
							setting.Name = GetName();

							// ���W�X�^�̎�ʂ��m��
							setting.Category = entity.Category;
							// ���W�X�^�̔ԍ����m��
							setting.Index = entity.Index;
							// ���W�X�^�̒l���m�F
							if (entity.Value.type() == typeid(bool))
							{
								// �r�b�g�^ �� ���W�X�^�̃f�[�^���m��
								setting.Values.emplace_back((short)Variant::Convert<bool>(entity.Value) ? 1 : 0);
							}
							else if (entity.Value.type() == typeid(__int16))
							{
								// ����(2�o�C�g)�^ �� ���W�X�^�̃f�[�^���m��
								setting.Values.emplace_back((short)Variant::Convert<__int16>(entity.Value));
							}
							else if (entity.Value.type() == typeid(__int32))
							{
								// ����(4�o�C�g)�^ �� ���W�X�^�̃f�[�^���m��
								setting.Values.emplace_back((short)(Variant::Convert<__int32>(entity.Value) >> 0 & 0xFFFF));
								setting.Values.emplace_back((short)(Variant::Convert<__int32>(entity.Value) >> 16 & 0xFFFF));
							}

							// ���W�X�^���X�V
							postscript << "Plc::Register::Set()";
							Plc::Register::Set(setting);
							postscript.str("");
						}

						// �擪�ɓo�^���Ă���f�[�^���폜
						PopWriteFirst();

						// ���O�o��
						postscript << "Write()" << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					} while (false);
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