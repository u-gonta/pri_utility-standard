#include "Poling.h"
#include "Common/Tool/Variant.h"

#include "WrapperMotionApi/Status.h"
#include "PlcMotionApi/Parameter.h"
#include "PlcMotionApi/Controller.h"

namespace Standard
{
	namespace Axis
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
				Exception::EnumCode errorCode = Exception::CodeAxisConnect;
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
				Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CInitialize trigger;

				logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

				try
				{
					Poling::CSetting polingSetting = GetSetting();
					Plc::Controller::Setting::CInitialize settingController;

					// ���̂��X�V
					settingController.Name = GetName();

					// �R���g���[�����J���ݒ���擾
					postscript << "OpenController";
					boost::any buffer = polingSetting.Connect.Other.GetValue<Plc::EnumParameter>(Plc::ParameterOpenController);

					// �R���g���[�����J���ݒ肪�L�����m�F
					if (Variant::IsValue<std::string>(buffer))
					{
						// �R���g���[�����J���ݒ���X�V
						settingController.Open.Input(Variant::Convert<std::string>(buffer));
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
						settingController.ApiTimeout.Input(Variant::Convert<std::string>(buffer));
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
					}

					// �R���g���[����������
					postscript << "Plc::InitializeController()";
					auto resultInitialize = Plc::InitializeController(settingController);
					postscript.str("");

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncController);

					// �R���g���[���̏����X�V
					m_controller = resultInitialize.Controller;

					// ����j��
					postscript << "Clear()";
					Clear();
					postscript.str("");

					// ����������
					for (auto& driver : polingSetting.Connect.Drivers)
					{
						postscript << "����No:" << driver.first;
						postscript << Logging::ConstSeparator << "����:" << driver.second.Name;

						// ���̃n���h�����쐬����ݒ���擾
						buffer = driver.second.Other.GetValue<Plc::EnumParameter>(Plc::ParameterDeclareAxis);

						// ���̃n���h�����쐬����ݒ肪�L�����m�F
						if (Variant::IsValue<MotionApi::Axis::Setting::CDeclare>(buffer) == false)
						{
							// ���̃n���h�����쐬����ݒ肪����
							throw std::exception("�ݒ�Ȃ�");
						}
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						Plc::Axis::Setting::CInitialize settingAxis;
						CStatus status;

						// ���̂��X�V
						settingAxis.Name = GetName();

						// ���̃n���h�����쐬����N���X�ɕϊ�
						postscript << "Variant::Convert()";
						settingAxis.Declare = Variant::Convert<MotionApi::Axis::Setting::CDeclare>(buffer);
						postscript.str("");

						// ����������
						postscript << "Plc::InitializeAxis()";
						auto result = Plc::InitializeAxis(settingAxis);
						postscript.str("");

						// ���̖��̂��X�V
						status.Name = driver.second.Name;

						// ���̃n���h�����X�V
						status.Handle = result.Axis.Handle;

						// �擪�A�h���X���X�V
						status.Address = settingAxis.Declare.Address;

						// ����\���X�V
						status.Resolution = settingAxis.Declare.Resolution;

						// ����ǉ�
						postscript << "Addition()";
						Addition(driver.first, status);
						postscript.str("");
					}

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
				Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
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

						Plc::Axis::Setting::CDestroy settingAxis;

						// ���̂��X�V
						settingAxis.Name = GetName();

						// �������
						postscript << "Clear()";
						Clear();
						postscript.str("");

						Plc::Controller::Setting::CDestroy settingController;

						// ���̂��X�V
						settingController.Name = GetName();

						// �n���h�����X�V
						settingController.Close.Handle = m_controller.Handle;

						// �R���g���[����j��
						postscript << "Plc::DestroyController()";
						Plc::DestroyController(settingController);
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
			///	@detail			���g���C�̔����h����ŋL�q����
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
			/// @brief			���g���C�̒x�����Ԃ��擾
			///	@detail			���g���C�̒x�����Ԃ��擾����
			///					���p�������֐�
			/// @return			���g���C�̒x������[ms]
			////////////////////////////////////////////////////////////////////////////////
			int CWorker::GetDelayRetry()
			{
				int ret = 0;

				// ���g���C�̒x������
				ret = 1000;

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
				Exception::EnumCode errorCode = Exception::CodeAxisRead;
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

					Axis::Status::CInput input;

					// �S�����擾
					postscript << "Get()";
					auto statuses = Get();
					postscript.str("");

					// �S���𑖍�
					for (const auto& status : statuses)
					{
						Plc::Register::Setting::CGetContent content;

						// ���̂��X�V
						content.Name = GetName();

						// ���W�X�^�̎�ʂ��m��
						content.Category = Plc::Register::ConstCategoryI;
						content.Category += Plc::Register::ConstTypeWord;
						// ���W�X�^�̔ԍ����m��
						content.Index = status.second.Address;
						// ���W�X�^�͈̔͂��m��
						content.Size = 0x80;

						// ���W�X�^�̓ǂݍ���
						postscript << "Plc::GetRegister()";
						auto result = Plc::GetRegister(content);
						postscript.str("");

						Plc::Register::CSpecify specify(content.Category);
						Axis::Status::Input::CDriver driver;
						unsigned long warning = 0;
						unsigned long alarm = 0;
						signed long coordinate = 0;

						// ���̂��X�V
						driver.Name = status.second.Name;

						// ���W�X�^�̒l���i�[
						for (const auto& numeric : result.Words)
						{
							Io::Specify::CAddress address;

							address.Category = specify.GetAddress();
							address.Index = content.Index;

							// �A�h���X�̕ϊ����m��
							address.Hex = true;
							address.Margin = specify.GetMargin();

							// �A�h���X���m��
							address.Index += numeric.first;

							switch (numeric.first)
							{
							case 0:
								// �^�]�X�e�[�^�X �� �^�]�X�e�[�^�X�̐M���𑖍�
								for (const auto& iterator : MotionApi::Status::RunningIterator())
								{
									// �^�]�X�e�[�^�X���X�V
									driver.Running[iterator] = (numeric.second >> iterator & 0x01 ? true : false);
								}
								break;

							case 2:
								// ���[�j���O �� ���ʂ��X�V
								warning = numeric.second & 0xFF;
								break;

							case 3:
								// ���[�j���O �� ��ʂ��X�V
								warning |= ((unsigned long)numeric.second << 16 & 0xFFFF0000);

								// ���[�j���O�̐M���𑖍�
								for (const auto& iterator : MotionApi::Status::WarningIterator())
								{
									// ���[�j���O���X�V
									driver.Warning[iterator] = (numeric.second >> iterator & 0x01 ? true : false);
								}
								break;

							case 4:
								// �A���[�� �� ���ʂ��X�V
								alarm = numeric.second;
								break;

							case 5:
								// �A���[�� �� ��ʂ��X�V
								alarm |= ((unsigned long)numeric.second << 16 & 0xFFFF0000);

								// �A���[���̐M���𑖍�
								for (const auto& iterator : MotionApi::Status::AlarmIterator())
								{
									// �A���[�����X�V
									driver.Alarm[iterator] = (numeric.second >> iterator & 0x01 ? true : false);
								}
								break;

							case 12:
								// �ʒu �� �ʒu�̐M���𑖍�
								for (const auto& iterator : MotionApi::Status::PositionIterator())
								{
									// �ʒu���X�V
									driver.Position[iterator] = (numeric.second >> iterator & 0x01 ? true : false);
								}
								break;

							case 16:
								// �@�B���W�n�t�B�[�h�o�b�N�ʒu(APOS) �� ���ʂ��X�V
								coordinate = (signed long)numeric.second;
								break;

							case 17:
								// �@�B���W�n�t�B�[�h�o�b�N�ʒu(APOS) �� ��ʂ��X�V
								coordinate |= (signed long)numeric.second << 16 & 0xFFFF0000;

								// �ʒu�̒P�ʂ�ϊ�
								driver.Coordinate[Status::Input::CoordinateCurrent] = (double)coordinate * status.second.Resolution;
								break;
							}
						}

						// �@��̏�Ԃ�ێ�����N���X���X�V
						input.Drivers[status.first] = driver;
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
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
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

						// ���̎w�߂𔻒�
						if (write->Output.Command.type() == typeid(Status::Output::CServo))
						{
							// �T�[�{�𐧌�
							Servo(*write);
						}
						else if (write->Output.Command.type() == typeid(Status::Output::CStartJog))
						{
							// �W���O�����s
							StartJog(*write);
						}
						else if (write->Output.Command.type() == typeid(Status::Output::CStopJog))
						{
							// �W���O���~
							StopJog(*write);
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

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�����J��
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::Clear()
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "Clear()" << Logging::ConstSeparator;

				Plc::Axis::Setting::CDestroy setting;

				// ���̂��X�V
				setting.Name = GetName();

				// ����j��
				postscript << "Plc::DestroyAxis()";
				Plc::DestroyAxis(setting);
				postscript.str("");

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// ���̏�Ԃ��N���A
				m_axisStatuses.clear();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			����ǉ�
			/// @param[in]		id		���̎��ʎq
			/// @param[in]		status	���̏�ԃN���X
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::Addition(int id, const CStatus& status)
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "Addition()" << Logging::ConstSeparator;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// ���̏�Ԃ�ǉ�
				postscript << "ID:" << id;
				postscript << Logging::ConstSeparator << "����:" << status.Name;
				postscript << Logging::ConstSeparator << "�n���h��:" << Plc::FormatHandle(status.Handle);
				postscript << Logging::ConstSeparator << "�A�h���X:" << Text::FormatX(4, status.Address);
				postscript << Logging::ConstSeparator << "����\:" << status.Resolution;
				m_axisStatuses[id] = status;
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�����擾
			/// @param[in]		id	���̎��ʎq
			/// @return			���̏�ԃN���X
			////////////////////////////////////////////////////////////////////////////////
			CStatus CWorker::Get(int id)
			{
				CStatus ret;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// ���̏�Ԃ��m�F
				if (m_axisStatuses.count(id) <= 0)
				{
					// ���̏�ԂȂ�
					std::stringstream message;
					message << "���̎��ʎq���͈͊O:" << id;
					throw std::exception(message.str().c_str());
				}

				// ���̏�Ԃ��擾
				ret = m_axisStatuses.at(id);

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�S�����擾
			/// @return			�S���̏�ԃN���X
			////////////////////////////////////////////////////////////////////////////////
			std::map<int, CStatus> CWorker::Get()
			{
				std::map<int, CStatus> ret;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// ���̏�Ԃ��擾
				ret = m_axisStatuses;

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�T�[�{�𐧌�
			/// @param[in]		object	������Ď�����ݒ�̃N���X
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::Servo(const Surveillance::CSetting& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Surveillance::CFinish finish;

				logging.Message << GetName() << "Servo()" << Logging::ConstSeparator;

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

					// �T�[�{�̐��䂩�m�F
					if (object.Output.Command.type() != typeid(Status::Output::CServo))
					{
						// �T�[�{�̐���ł͂Ȃ�
						postscript << "���̐���N���X�ƕs��v�F" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// ���̃T�[�{���(�o��)���w�肷��N���X�ɕϊ�
					auto output = boost::any_cast<Status::Output::CServo>(object.Output.Command);

					Plc::Device::Setting::CServoControl setting;

					// ���̂��X�V
					setting.Name = GetName();

					// ���𑖍�
					for (const auto& driver : output.Drivers)
					{
						// ���̃n���h����ǉ� �� �����擾
						setting.Device.Handles.emplace_back(Get(driver.first).Handle);
					}

					// ������X�V
					setting.Status.Mode = output.Mode;

					// �^�C���A�E�g[ms]���擾
					auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterServoTimeout);

					// �^�C���A�E�g[ms]���L�����m�F
					if (Variant::IsIntValue(buffer))
					{
						// �^�C���A�E�g[ms]���X�V
						setting.Status.Timeout = Variant::ConvertInt(buffer);
					}

					// �T�[�{�𐧌�
					postscript << "Plc::AxisServo()";
					Plc::AxisServo(setting);
					postscript.str("");

					// �����̊������N��
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �����̊������G���[
					finish.Suspension = true;
					finish.Message = e.what();
					// �����̊������N��
					WakeupFinish(finish, object.Wakeup);

					// ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�W���O�����s
			/// @param[in]		object	������Ď�����ݒ�̃N���X
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::StartJog(const Surveillance::CSetting& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Surveillance::CFinish finish;

				logging.Message << GetName() << "StartJog()" << Logging::ConstSeparator;

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

					// �W���O�̎��s���m�F
					if (object.Output.Command.type() != typeid(Status::Output::CStartJog))
					{
						// �W���O�̎��s�ł͂Ȃ�
						postscript << "���̐���N���X�ƕs��v�F" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// ���̃W���O���s���w�肷��N���X�ɕϊ�
					auto output = boost::any_cast<Status::Output::CStartJog>(object.Output.Command);

					Plc::Device::Setting::CStartJog setting;

					// ���̂��X�V
					setting.Name = GetName();

					// ���𑖍�
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Motion::Jog::CStart jog;

						// ���̃W���O���s���s���ݒ�N���X���擾
						auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterStartJog);

						// ���̃W���O���s���s���ݒ�N���X���L�����m�F
						if (Variant::IsValue<MotionApi::Device::Setting::Motion::Jog::CStart>(buffer))
						{
							// ���̃W���O���s���s���ݒ�N���X���X�V
							jog = Variant::Convert<MotionApi::Device::Setting::Motion::Jog::CStart>(buffer);
						}

						// ���̃n���h�����X�V
						jog.Handle = Get(driver.first).Handle;

						// ���̐ݒ��ǉ�
						setting.Device.Motions.emplace_back(jog);
					}

					// �W���O�����s
					postscript << "Plc::AxisStartJog()";
					Plc::AxisStartJog(setting);
					postscript.str("");

					// �����̊������N��
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �����̊������G���[
					finish.Suspension = true;
					finish.Message = e.what();
					// �����̊������N��
					WakeupFinish(finish, object.Wakeup);

					// ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�W���O���~
			/// @param[in]		object	������Ď�����ݒ�̃N���X
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::StopJog(const Surveillance::CSetting& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Surveillance::CFinish finish;

				logging.Message << GetName() << "StopJog()" << Logging::ConstSeparator;

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

					// �W���O�̒�~���m�F
					if (object.Output.Command.type() != typeid(Status::Output::CStopJog))
					{
						// �W���O�̒�~�ł͂Ȃ�
						postscript << "���̐���N���X�ƕs��v�F" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// ���̃W���O��~���w�肷��N���X�ɕϊ�
					auto output = boost::any_cast<Status::Output::CStopJog>(object.Output.Command);

					Plc::Device::Setting::CStopJog setting;

					// ���̂��X�V
					setting.Name = GetName();

					// ���𑖍�
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Motion::Jog::CStop jog;

						// ���̃W���O��~���s���ݒ�N���X���擾
						auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterStopJog);

						// ���̃W���O��~���s���ݒ�N���X���L�����m�F
						if (Variant::IsValue<std::string>(buffer))
						{
							// ���̃W���O��~���s���ݒ�N���X���X�V
							jog.Input(Variant::Convert<std::string>(buffer));
						}

						// ���̃n���h�����X�V
						jog.Handle = Get(driver.first).Handle;

						// ���̐ݒ��ǉ�
						setting.Device.Motions.emplace_back(jog);
					}

					// �W���O���~
					postscript << "Plc::AxisStopJog()";
					Plc::AxisStopJog(setting);
					postscript.str("");

					// �����̊������N��
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				catch (const std::exception& e)
				{
					// ��O�̏��� �� �����̊������G���[
					finish.Suspension = true;
					finish.Message = e.what();
					// �����̊������N��
					WakeupFinish(finish, object.Wakeup);

					// ��O�𔭖C
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}
		}
	}
}