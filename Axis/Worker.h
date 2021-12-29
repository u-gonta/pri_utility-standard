#pragma once

#include "Common/Axis/Setting.h"
#include "Common/Axis/Status.h"
#include "Device/Worker.h"
#include "Poling.h"

namespace Standard
{
	namespace Axis
	{
		namespace Worker
		{
			//! ���͂̕ω���ʒm����֐��̒�`
			typedef void(OnChange)(const Status::CInput&);

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      ���̐�����s���e���v���[�g�̃N���X
			///				�� �f�o�C�X�̐�����s���e���v���[�g�̃N���X����h��
			/// @param[in]	CDefault	�f�t�H���g�Ȑݒ�N���X���w��
			/// @param[in]	CConnect	�ڑ��̐ݒ�N���X���w��
			////////////////////////////////////////////////////////////////////////////////
			template <typename CPoling>
			class CTemplate
				: virtual public Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>()
				{
					// ���̂��X�V
					SetName("Axis::");
					m_noticeChange.SetName("Axis::Notice::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{
					// ���͂̕ω���ʒm����֐��̉���
					DetachFunctionInput();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��Ԃ̒ʒm���J�n
				/// @detail			�ʒm����֐���o�^����O�ɌĂяo��
				/// @param[in]		cycle	��Ԃ̒ʒm���Ď�����Ԋu[ms]
				////////////////////////////////////////////////////////////////////////////////
				void StartNotice(int cycle = 60000)
				{
					// ��Ԃ̒ʒm���s��������ݒ�
					m_noticeChange.SetCycle(cycle);

					// ��Ԃ̒ʒm���J�n
					m_noticeChange.Start();

					// ��Ԃ̒ʒm�ŏ�������v��
					m_noticeChange.RequestInitialize();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��Ԃ̒ʒm���~
				/// @detail			�ʒm����֐���������ɌĂяo��
				////////////////////////////////////////////////////////////////////////////////
				void StopNotice()
				{
					// ��Ԃ̒ʒm�Ŕj����v��
					m_noticeChange.RequestDestroy();

					// ��Ԃ̒ʒm���~
					m_noticeChange.Stop();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���͂̕ω���ʒm����֐��̓o�^
				/// @detail			���͂̕ω����ɌĂяo���ꂽ���֐����w�肷��
				/// @param[in]		object	�Ăяo���ꂽ���֐�
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionInput(std::function<void(const Status::CInput&)> object)
				{
					// ��Ԃ̒ʒm���J�n
					StartNotice();

					// ���͂̕ω���ʒm����֐��̓o�^
					m_noticeChange.AttachFunctionCalled(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���͂̕ω���ʒm����֐��̉���
				/// @detail			���͂̕ω����ɌĂяo�����֐�����������
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionInput()
				{
					// ���͂̕ω���ʒm����֐��̉���
					m_noticeChange.DetachFunctionCalled();

					// ��Ԃ̒ʒm���~
					StopNotice();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̐����v��
				/// @param[in]		object	���̏��(�o��)���w�肷��N���X
				////////////////////////////////////////////////////////////////////////////////
				void Command(const Status::COutput& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisWrite;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "Command()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// ���䂪�\���m�F
					IsControllable();

					try
					{
						Surveillance::CSetting setting;

						// ���̏��(�o��)���w�肷��N���X���X�V
						setting.Output = object;

						// ���𐧌��o�^
						postscript << "RequestWrite()";
						m_poling.RequestWrite(setting);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O��ʒm�����C
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̐����ҋ@
				/// @param[in]		object	���̏��(�o��)���w�肷��N���X
				////////////////////////////////////////////////////////////////////////////////
				void CommandPending(const Status::COutput& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisWrite;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "CommandPending()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// ���䂪�\���m�F
					IsControllable();

					try
					{
						// �g���K�[��ҋ@����N���X
						Signal::Observer::CTemplate<Surveillance::CFinish> observer;

						// �g���K�[��ʒm������֐���o�^
						postscript << "Subject::RegistrationFunction()";
						std::shared_ptr<std::function<Surveillance::OnWakeup>> onWakeup = std::make_shared<std::function<Surveillance::OnWakeup>>(std::bind(&Signal::Observer::CTemplate<Surveillance::CFinish>::OnCalled, &observer, std::placeholders::_1));

						// �g���K�[��ҋ@����^�C���A�E�g���X�V
						observer.Setting.Timeout = object.Timeout;

						Surveillance::CSetting setting;

						// ���̏��(�o��)���w�肷��N���X���X�V
						setting.Output = object;

						// �g���K�[��ʒm����֐����X�V
						setting.Wakeup = onWakeup;

						// ���𐧌��o�^
						postscript << "RequestWrite()";
						m_poling.RequestWrite(setting);
						postscript.str("");

						// �g���K�[��҂�
						postscript << "Observer::Wait()";
						observer.Wait();
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// ����̌��ʂ��擾
						postscript << "Observer::GetArgument()";
						Surveillance::CFinish result = observer.GetArgument();
						postscript.str("");

						// ���f���m�F
						if (result.Suspension)
						{
							// ���f
							postscript << "Observer::Trigger()" << Logging::ConstSeparator << "���f";

							// ��O�𔭖C
							throw std::exception();
						}
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O��ʒm�����C
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̃T�[�{�𐧌䂵�đҋ@
				/// @param[in]		object	���̃T�[�{���(�o��)���w�肷��N���X
				////////////////////////////////////////////////////////////////////////////////
				void Servo(const Status::Output::CServo& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "Servo()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					Status::COutput output;

					// ���̏��(�o��)���w�肷��N���X���X�V

					output.Command = object;

					// ���̐����ҋ@
					CommandPending(output);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̃W���O�����s���đҋ@
				/// @param[in]		object	���̃W���O���s���w�肷��N���X
				////////////////////////////////////////////////////////////////////////////////
				void StartJog(const Status::Output::CStartJog& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "StartJog()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					Status::COutput output;

					// ���̏��(�o��)���w�肷��N���X���X�V
					output.Command = object;

					// ���̐����ҋ@
					CommandPending(output);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���̃W���O���~���đҋ@
				/// @param[in]		object	���̃W���O��~���w�肷��N���X
				////////////////////////////////////////////////////////////////////////////////
				void StopJog(const Status::Output::CStopJog& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "StopJog()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					Status::COutput output;

					// ���̏��(�o��)���w�肷��N���X���X�V
					output.Command = object;

					// ���̐����ҋ@
					CommandPending(output);
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ς݂̗�O�𔭖C
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionInitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionInitialized()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeAxisInitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���ς݂̗�O�𔭖C
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionDiscarded() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionDiscarded()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeAxisDiscarded, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���������̗�O�𔭖C
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionUninitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionUninitialized()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeAxisUninitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			������
				/// @detail			�������̃^�C�~���O�ŌĂяo�����
				///					���|�[�����O�̗�O��ʒm����֐���o�^�A�����̍X�V�ȂǍs��
				////////////////////////////////////////////////////////////////////////////////
				void InitializeDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					try
					{
						// ��O����M����֐��̓o�^
						postscript << "Poling::AttachFunctionException()";
						m_poling.AttachFunctionException(std::bind(&CTemplate::OnException, this, std::placeholders::_1));
						postscript.str("");

						// ���͂���M����֐��̓o�^
						postscript << "Poling::AttachFunctionCalled()";
						m_poling.AttachFunctionCalled(std::bind(&CTemplate::OnInput, this, std::placeholders::_1));
						postscript.str("");

						// �f�t�H���g�̐ݒ�N���X���擾
						Setting::CDefault defaultSetting = GetDefaultSetting();

						// �|�[�����O�̎������X�V
						postscript << "Poling::SetCycle(" << defaultSetting.Cycle << ")";
						m_poling.SetCycle(defaultSetting.Cycle);
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �����|�[�����O����ׂ̐ݒ���擾
						postscript << "Poling::GetSetting()";
						Poling::CSetting polingSetting = m_poling.GetSetting();
						postscript.str("");

						// �f�t�H���g�̐ݒ���X�V
						polingSetting.Default = defaultSetting;

						// �|�[�����O�̃��g���C��
						postscript << "Poling::Setting::Retry(" << polingSetting.Default.Retry << ")";
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �|�[�����O�̃^�C���A�E�g
						postscript << "Poling::Setting::Timeout(" << polingSetting.Default.Timeout << ")";
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �����|�[�����O����ׂ̐ݒ���X�V
						postscript << "Poling::SetSetting()";
						m_poling.SetSetting(polingSetting);
						postscript.str("");

						// ���̃|�[�����O���J�n
						postscript << "Poling::Start()";
						m_poling.Start();
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O��ʒm�����C
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j��
				/// @detail			�j���̃^�C�~���O�ŌĂяo�����
				///					���|�[�����O�̗�O��ʒm����֐��̉����ȂǍs��
				////////////////////////////////////////////////////////////////////////////////
				void DestroyDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					try
					{
						// �j��
						postscript << "Poling::Destroy()";
						m_poling.Destroy();
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						postscript.str("");

						// ���͂���M����֐��̉���
						postscript << "Poling::DetachFunctionCalled()";
						m_poling.DetachFunctionCalled();
						postscript.str("");

						// ��O����M����֐��̉���
						postscript << "Poling::DetachFunctionException()";
						m_poling.DetachFunctionException();
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O��ʒm�����C
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ڑ�
				///	@detail			�ڑ��̃^�C�~���O�ŌĂяo�����
				////////////////////////////////////////////////////////////////////////////////
				void ConnectDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisConnect;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					try
					{
						// �j��
						postscript << "Poling::Destroy()";
						m_poling.Destroy();
						postscript.str("");

						// �����|�[�����O����ׂ̐ݒ���擾
						postscript << "Poling::GetSetting()";
						Poling::CSetting polingSetting = m_poling.GetSetting();
						postscript.str("");

						// �ڑ��̐ݒ�N���X���擾
						Setting::CConnect connectSetting = GetConnectSetting();

						// �ڑ��̐ݒ���X�V
						polingSetting.Connect = connectSetting;

						// �����|�[�����O����ׂ̐ݒ���X�V
						postscript << "Poling::SetSetting()";
						m_poling.SetSetting(polingSetting);
						postscript.str("");

						// ������
						postscript << "Poling::Initialize()";
						m_poling.Initialize();
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O��ʒm�����C
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�ؒf
				///	@detail			�ؒf�̃^�C�~���O�ŌĂяo�����
				////////////////////////////////////////////////////////////////////////////////
				void DisconnectDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeAxisDisconnect;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					try
					{
						// �j��
						postscript << "Poling::Destroy()";
						m_poling.Destroy();
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O��ʒm�����C
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��O���󂯎��֐�
				/// @param[in]		object	��O�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void OnException(const Exception::CObject& object)
				{
					// ��O��ʒm
					NotifyException(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�M�����X�V
				/// @param[in]		logging		���O�̃N���X
				/// @param[in]		postscript	���b�Z�[�W
				/// @param[in]		before		�X�V�O�̐M�����
				/// @param[in]		after		�X�V��̐M�����
				/// @param[in]		target		�M���̔ԍ�
				/// @return			true:�X�V���� / false:�X�V�Ȃ�
				////////////////////////////////////////////////////////////////////////////////
				bool UpdateInput(Logging::CObject& logging, std::stringstream& postscript, std::map<unsigned short, bool>& before, bool after, unsigned short target)
				{
					bool ret = false;

					do
					{
						// �M�����o�^�ς݂��m�F
						if (0 < before.count(target))
						{
							// �f�[�^���� �� �i�[�ς݂̒l�Ɣ�r
							if (before[target] == after)
							{
								// �l�̕ω��Ȃ� �� �����𔲂���
								break;
							}

							// �ω�����
							postscript << (before[target] ? "ON" : "OFF") << " �� ";
						}

						// �V�Kor�ω�����
						postscript << (after ? "ON" : "OFF");
						before[target] = after;

						ret = true;
					} while (false);

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���͂̃C�x���g
				/// @param[in]		object	IO�̏��(����)�̃N���X
				////////////////////////////////////////////////////////////////////////////////
				void OnInput(const Status::CInput& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "OnInput()" << Logging::ConstSeparator;

					Status::CInput changedInput;
					bool notify = false;

					// ���𑖍�
					for (const auto& driver : object.Drivers)
					{
						// �^�]�X�e�[�^�X�𑖍�
						for (const auto& running : driver.second.Running)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "�^�]�X�e�[�^�X:" << Text::Format(2, running.first);
							postscript << Logging::ConstSeparator;

							// �r������
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// �M�����X�V
							if (UpdateInput(logging, postscript, m_input.Drivers[driver.first].Running, running.second, running.first) == false)
							{
								// �l�̕ω��Ȃ� �� ���̐M����
								postscript.str("");
								continue;
							}

							// �V�Kor�ω�����
							changedInput.Drivers[driver.first].Running[running.first] = running.second;
							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// �l����
							notify = true;
						}

						// ���[�j���O�𑖍�
						for (const auto& warning : driver.second.Warning)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "���[�j���O:" << Text::Format(2, warning.first);
							postscript << Logging::ConstSeparator;

							// �r������
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// �M�����X�V
							if (UpdateInput(logging, postscript, m_input.Drivers[driver.first].Warning, warning.second, warning.first) == false)
							{
								// �l�̕ω��Ȃ� �� ���̐M����
								postscript.str("");
								continue;
							}

							// �V�Kor�ω�����
							changedInput.Drivers[driver.first].Warning[warning.first] = warning.second;
							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// �l����
							notify = true;
						}

						// �A���[���𑖍�
						for (const auto& alarm : driver.second.Alarm)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "�A���[��:" << Text::Format(2, alarm.first);
							postscript << Logging::ConstSeparator;

							// �r������
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// �M�����X�V
							if (UpdateInput(logging, postscript, m_input.Drivers[driver.first].Alarm, alarm.second, alarm.first) == false)
							{
								// �l�̕ω��Ȃ� �� ���̐M����
								postscript.str("");
								continue;
							}

							// �V�Kor�ω�����
							changedInput.Drivers[driver.first].Alarm[alarm.first] = alarm.second;
							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// �l����
							notify = true;
						}

						// �ʒu�𑖍�
						for (const auto& position : driver.second.Position)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "�ʒu:" << Text::Format(2, position.first);
							postscript << Logging::ConstSeparator;

							// �r������
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// �M�����X�V
							if (UpdateInput(logging, postscript, m_input.Drivers[driver.first].Position, position.second, position.first) == false)
							{
								// �l�̕ω��Ȃ� �� ���̐M����
								postscript.str("");
								continue;
							}

							// �V�Kor�ω�����
							changedInput.Drivers[driver.first].Position[position.first] = position.second;
							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// �l����
							notify = true;
						}

						// ���W�𑖍�
						for (const auto& coordinate : driver.second.Coordinate)
						{
							postscript << driver.second.Name;
							postscript << Logging::ConstSeparator << "���W:" << Text::Format(2, coordinate.first);
							postscript << Logging::ConstSeparator;

							// �r������
							std::lock_guard<std::recursive_mutex> lock(m_asyncData);

							// ���W���o�^�ς݂��m�F
							if (0 < m_input.Drivers[driver.first].Coordinate.count(coordinate.first))
							{
								// �f�[�^���� �� �i�[�ς݂̒l�Ɣ�r
								if (::fabs(coordinate.second - m_input.Drivers[driver.first].Coordinate[coordinate.first]) <= 0)
								{
									// �l�̕ω��Ȃ� �� ���̈ʒu��
									postscript.str("");
									continue;
								}

								// �ω�����
								postscript << m_input.Drivers[driver.first].Position[coordinate.first] << " �� ";
							}

							// �V�Kor�ω�����
							postscript << coordinate.second;
							m_input.Drivers[driver.first].Coordinate[coordinate.first] = coordinate.second;
							changedInput.Drivers[driver.first].Coordinate[coordinate.first] = coordinate.second;
							// ���O�o��
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");

							// �l����
							notify = true;
						}
					}

					// �ʒm����f�[�^���m�F
					if (notify)
					{
						// �ʒm����f�[�^������ �� ���͒l�̕ω���ʒm
						m_noticeChange.RequestNotice(changedInput);
					}
				}

			private:
				//! �r������N���X(�f�[�^�p)
				std::recursive_mutex m_asyncData;

				//! ���̏��(����)�̃N���X
				Status::CInput m_input;

				//! ���̊Ď��A���̐���A��Ԃ�ʒm����N���X
				CPoling m_poling;

				//! ���͂̕ω���ʒm����N���X
				Notice::Worker::CTemplate<OnChange, Status::CInput> m_noticeChange;
			};
		}
	}
}