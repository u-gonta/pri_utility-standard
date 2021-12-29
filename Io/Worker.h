#pragma once

#include "Common/Io/Setting.h"
#include "Common/Io/Status.h"
#include "Common/Tool/Variant.h"
#include "Device/Worker.h"
#include "Signal/Result.h"
#include "Signal/Subject.h"
#include "Signal/Observer.h"
#include "Poling.h"

namespace Standard
{
	namespace Io
	{
		namespace Worker
		{
			namespace Trigger
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CInput
				/// @brief      ���͂��Ď��������ʂ̃N���X
				///				�� ��Ԃ��Ď��������ʂ�ێ�����e���v���[�g�̃N���X����p��
				////////////////////////////////////////////////////////////////////////////////
				class CInput
					: virtual public Signal::Result::CTemplate
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CInput() : Signal::Result::CTemplate()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CInput& operator = (const CInput& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CInput()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CInput& object)
					{
						this->Setting = object.Setting;
					}

				public:
					//! �g���K�[�̐ݒ�N���X
					Setting::CTrigger Setting;
				};

				//! �g���K��ʒm����֐��̒�`
				typedef void(OnWakeup)(const Trigger::CInput&);

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CSetting
				/// @brief      �Ď��̐ݒ�N���X
				///				�� IO�̓��͂��ω�����̂�҂ݒ�N���X����p��
				////////////////////////////////////////////////////////////////////////////////
				class CSetting
					: virtual public Setting::CTrigger
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CSetting() : Setting::CTrigger()
					{
						Wakeup.reset();
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			�g���K�[�̐ݒ�N���X
					////////////////////////////////////////////////////////////////////////////////
					CSetting& operator = (const Setting::CTrigger& object) noexcept
					{
						// ����̊֐�
						Io::Setting::CTrigger::Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CSetting& operator = (const CSetting& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CSetting()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CSetting& object)
					{
						// ����̊֐�
						Io::Setting::CTrigger::Update(object);

						this->Wakeup = object.Wakeup;
					}

				public:
					//! �g���K�[�̒ʒm���Ăяo���֐�
					std::weak_ptr<std::function<OnWakeup>> Wakeup;
				};
			}

			//! ���͂̕ω���ʒm����֐��̒�`
			typedef void(OnChange)(const Status::CInput&);

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      Io�̐�����s���e���v���[�g�̃N���X
			///				�� �f�o�C�X�̐�����s���e���v���[�g�̃N���X����h��
			/// @param[in]	CDefault	�f�t�H���g�̐ݒ�N���X���w��
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
					SetName("Io::");
					m_noticeChange.SetName("Io::Notice::");
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
				/// @brief			���͂̃g���K��ҋ@
				/// @param[in]		object	IO�̓��͂��ω�����̂�҂ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				void TriggerInputPending(const Setting::CInputPending& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoRead;
					int deviceErrorCode = Exception::DeviceCodeSuccess;
					Utility::CStopWatch stopWatch;

					logging.Message << GetName() << "TriggerInputPending()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// ���䂪�\���m�F
					IsControllable();

					try
					{
						// �g���K�[��ҋ@����N���X
						Signal::Observer::CTemplate<Trigger::CInput> observer;

						// �g���K�[��ʒm������֐���o�^
						postscript << "Subject::RegistrationFunction()";
						std::shared_ptr<std::function<Trigger::OnWakeup>> onWakeup = std::make_shared<std::function<Trigger::OnWakeup>>(std::bind(&Signal::Observer::CTemplate<Trigger::CInput>::OnCalled, &observer, std::placeholders::_1));

						// �g���K�[��ҋ@����^�C���A�E�g���X�V
						observer.Setting.Timeout = object.Timeout;

						Trigger::CSetting setting;

						// �g���K�[�̐ݒ���R�s�[
						setting = object.Trigger;

						// �g���K�[��ʒm����֐����X�V
						setting.Wakeup = onWakeup;
						postscript.str("");

						// ���͂��Ď����ăg���K�̒ʒm������֐���o�^
						postscript << "RegistrationTrigger(�A�h���X:" << Specify::Format(setting.Address) << ",�r�b�g�̕���:" << setting.Direction << ")";
						RegistrationTrigger(setting);
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// ����v�����N��
						postscript << "WakeupRequestAction()";
						m_poling.WakeupRequestAction();
						postscript.str("");

						// �g���K�[��҂�
						postscript << "Observer::Wait()";
						observer.Wait();
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// ���͂̌��ʂ��擾
						postscript << "Observer::GetArgument()";
						Trigger::CInput result = observer.GetArgument();
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
				/// @brief			���͂�ǂݍ���
				/// @param[in]		object	���͂̐ݒ�N���X
				/// @return			IO�̏��(����)�N���X
				////////////////////////////////////////////////////////////////////////////////
				Status::CInput ReadInput(const Setting::CInput& object) throw(...)
				{
					Status::CInput ret;

					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoRead;
					int deviceErrorCode = Exception::DeviceCodeSuccess;

					logging.Message << GetName() << "ReadInput()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// ���䂪�\���m�F
					IsControllable();

					try
					{
						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncData);

						// �A�h���X�𑖍�
						for (const auto& address : object.Addresses)
						{
							// �A�h���X���m�F
							if (m_input.Values.count(address) <= 0)
							{
								// �A�h���X�Ȃ� �� ���̃A�h���X��
								continue;
							}

							// �l���m��
							ret.Values[address] = m_input.Values.at(address);
						}
					}
					catch (const std::exception& e)
					{
						// ��O�̏��� �� ��O��ʒm�����C
						throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�o�͂���������
				/// @param[in]		object	IO�̏��(�o��)�N���X
				////////////////////////////////////////////////////////////////////////////////
				void WriteOutput(const Status::COutput& object) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WriteOutput()" << Logging::ConstSeparator;

					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// ���䂪�\���m�F
					IsControllable();

					// �o�͂��X�V
					SetOutput(object);

					// �o�͂���������
					m_poling.RequestWrite(object);
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�������ς݂̗�O�𔭖C
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionInitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionInitialized()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeIoInitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�j���ς݂̗�O�𔭖C
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionDiscarded() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionDiscarded()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeIoDiscarded, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���������̗�O�𔭖C
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionUninitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionUninitialized()" << Logging::ConstSeparator;

					// ��O�𔭖C
					OutputException(Exception::CodeIoUninitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			������
				/// @detail			�������̃^�C�~���O�ŌĂяo�����
				///					���|�[�����O�̗�O��ʒm����֐���o�^�A�����̍X�V�ȂǍs��
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void InitializeDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoInitialize;
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

						// Io���|�[�����O����ׂ̐ݒ���擾
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

						// Io���|�[�����O����ׂ̐ݒ���X�V
						postscript << "Poling::SetSetting()";
						m_poling.SetSetting(polingSetting);
						postscript.str("");

						// Io�̃|�[�����O���J�n
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
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void DestroyDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoDestroy;
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
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void ConnectDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoConnect;
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

						// Io���|�[�����O����ׂ̐ݒ���擾
						postscript << "Poling::GetSetting()";
						Poling::CSetting polingSetting = m_poling.GetSetting();
						postscript.str("");

						// �ڑ��̐ݒ�N���X���擾
						Setting::CConnect connectSetting = GetConnectSetting();

						// �ڑ��̐ݒ���X�V
						polingSetting.Connect = connectSetting;

						// �|�[�����O����@��̎��ʎq
						postscript << "Poling::Setting::Identifier(" << polingSetting.Connect.Identifier << ")";
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �|�[�����O�œǂݍ��ޔ͈͂��m�F
						for (const auto& addressRange : polingSetting.Connect.AddressRanges)
						{
							// �ǂݍ��ޔ͈͂����O�o��
							postscript << "Poling::Setting::AddressRange(";
							postscript << Specify::Format(addressRange.Start);
							postscript << "�` x";
							postscript << addressRange.Size;
							// ���O�o��
							postscript << ")";
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
						}

						// Io���|�[�����O����ׂ̐ݒ���X�V
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
				///					���p�������֐�
				////////////////////////////////////////////////////////////////////////////////
				void DisconnectDerived() override
				{
					Logging::CObject logging;
					std::stringstream postscript;
					Exception::EnumCode errorCode = Exception::CodeIoDisconnect;
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
				/// @brief			�g���K�[�̔��C
				/// @param[in]		object	�A�h���X�w��N���X
				////////////////////////////////////////////////////////////////////////////////
				void WakeupTrigger(const Status::CEntity& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WakeupTrigger()" << Logging::ConstSeparator;

					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncTrigger);

					do
					{
						// �r�b�g���m�F
						if (m_triggers.count(object) <= 0)
						{
							// �o�^�Ȃ�
							break;
						}

						// �r�b�g�̕���
						Status::EnumDirection direction = Status::DirectionRise;

						// �r�b�g���Ď������Ԃ��m�F
						if (m_triggers.at(object).count(Status::DirectionRise))
						{
							// �r�b�g�̗����オ����Ď� �� �r�b�g�������オ���Ă��邩�m�F
							if (Variant::Convert<bool>(object.Value) == false)
							{
								// �r�b�g��OFF
								break;
							}

							// �r�b�g�̕������m��
							direction = Status::DirectionRise;
						}
						else if (m_triggers.at(object).count(Status::DirectionFall))
						{
							// �r�b�g�̗�������Ď� �� �r�b�g�������������Ă��邩�m�F
							if (Variant::Convert<bool>(object.Value))
							{
								// �r�b�g��ON
								break;
							}

							// �r�b�g�̕������m��
							direction = Status::DirectionFall;
						}
						else
						{
							// �r�b�g�̏��Ȃ�
							break;
						}

						Trigger::CInput result;

						// �A�h���X���X�V
						result.Setting.Address = object;
						// �r�b�g�̕������X�V
						result.Setting.Direction = direction;
						// ���f�t���O������
						result.Suspension = false;

						// �o�^�𑖍�
						postscript << "�A�h���X:" << Specify::Format(object) << Logging::ConstSeparator << "�r�b�g:" << Variant::FormatValue(object.Value) << Logging::ConstSeparator << "�g���K��ʒm";
						for (auto iterator = m_triggers.at(object).at(direction).begin(); iterator != m_triggers.at(object).at(direction).end(); iterator++)
						{
							Signal::Subject::CTemplate<Trigger::CInput> subject;

							// �֐���o�^
							subject.RegistrationFunction(*iterator);

							try
							{
								// �֐���ʒm
								subject.CallingFunction(result);
							}
							catch (const std::exception& e)
							{
								// �G���[���b�Z�[�W�𓝍�
								postscript << Logging::ConstSeparator << e.what();
							}
						}

						// �r�b�g�̕������폜
						m_triggers.at(object).clear();

						// �A�h���X���Ƃɓo�^���Ă���֐��̐����m�F
						if (m_triggers.at(object).size() <= 0)
						{
							// �o�^�Ȃ� �� �A�h���X���Ƃ̓o�^���폜
							m_triggers.erase(object);
						}

						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					} while (false);
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
					std::vector<Status::CEntity> triggers;
					bool notify = false;

					// �A�h���X�𑖍�
					for (const auto& value : object.Values)
					{
						postscript << "�A�h���X:" << Specify::Format(value.first);
						postscript << Logging::ConstSeparator << Variant::FormatType(value.second) << ":";

						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncData);

						// �A�h���X���o�^�ς݂��m�F
						if (0 < m_input.Values.count(value.first))
						{
							// �f�[�^���� �� �i�[�ς݂̒l�Ɣ�r
							if (Variant::Equal(m_input.Values.at(value.first), value.second))
							{
								// �l�̕ω��Ȃ� �� ���̃A�h���X��
								postscript.str("");
								continue;
							}

							// �ω�����
							postscript << Variant::FormatValue(m_input.Values.at(value.first)) << " �� ";
						}

						// �V�Kor�ω�����
						postscript << Variant::FormatValue(value.second);
						m_input.Values[value.first] = value.second;
						changedInput.Values[value.first] = value.second;
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �^���m�F
						if (value.second.type() == typeid(bool))
						{
							// �r�b�g�^ �� �g���K�[���m��
							triggers.emplace_back(Status::CEntity(value.first, value.second));
						}

						// �l����
						notify = true;
					}

					// �g���K�[�𔭖C
					for (const auto& trigger : triggers)
					{
						// �g���K�[�̔��C
						WakeupTrigger(trigger);
					}

					// �ʒm����f�[�^���m�F
					if (notify)
					{
						// �ʒm����f�[�^������ �� ���͒l�̕ω���ʒm
						m_noticeChange.RequestNotice(changedInput);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�o�͂��X�V
				/// @param[in]		object	IO�̏��(�o��)�N���X
				////////////////////////////////////////////////////////////////////////////////
				void SetOutput(const Status::COutput& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "SetOutput()" << Logging::ConstSeparator;

					// �f�[�^�𑖍�
					for (const auto& entity : object.Entitys)
					{
						postscript << "�A�h���X:" << Specify::Format(entity);
						postscript << Logging::ConstSeparator << Variant::FormatType(entity.Value) << ":";

						// �r������
						std::lock_guard<std::recursive_mutex> lock(m_asyncData);

						// �A�h���X�̃f�[�^���擾
						auto element = std::find_if(m_output.Entitys.begin(), m_output.Entitys.end(), [&](const auto& element) { return Variant::Equal(element, entity); });

						// �A�h���X���o�^�ς݂��m�F
						if (element != m_output.Entitys.end())
						{
							// �f�[�^����
							postscript << Variant::FormatValue(element->Value) << " �� ";

							// �f�[�^���X�V
							element->Value = entity.Value;
						}
						else
						{
							// �f�[�^�Ȃ� �� �f�[�^��ǉ�
							m_output.Entitys.emplace_back(entity);

							// �\�[�g
							std::sort(m_output.Entitys.begin(), m_output.Entitys.end(), [](auto const& first, auto const& second)
							{
								return first < second;
							});
						}

						// �ω�����
						postscript << Variant::FormatValue(entity.Value);
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���͂̃g���K��ʒm����֐���o�^
				/// @param[in]		object	�Ď��̐ݒ�N���X
				////////////////////////////////////////////////////////////////////////////////
				void RegistrationTrigger(const Trigger::CSetting& object)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncTrigger);

					// �A�h���X���Ƃɓo�^���Ă���֐��̐����m�F
					if (m_triggers.count(object.Address) <= 0)
					{
						// �o�^�Ȃ� �� �A�h���X��o�^
						m_triggers[object.Address];
					}

					// �r�b�g�̕����̓o�^���m�F
					if (m_triggers.at(object.Address).count(object.Direction) <= 0)
					{
						// �o�^�Ȃ� �� �r�b�g�̕�����o�^
						m_triggers.at(object.Address)[object.Direction];
					}

					// �g���K��ʒm����֐���o�^
					m_triggers.at(object.Address).at(object.Direction).emplace_back(object.Wakeup);
				}

			private:
				//! �r������N���X(�f�[�^�p)
				std::recursive_mutex m_asyncData;

				//! IO�̏��(����)�̃N���X
				Status::CInput m_input;

				//! IO�̏��(�o��)�̃N���X
				Status::COutput m_output;

				//! IO�̊Ď��AIO�̐���A��Ԃ�ʒm����N���X
				CPoling m_poling;

				//! ���͂̕ω���ʒm����N���X
				Notice::Worker::CTemplate<OnChange, Status::CInput> m_noticeChange;

				//! �r������N���X(���͂̃g���K��ʒm����֐��p)
				std::recursive_mutex m_asyncTrigger;

				//! ���͂̃g���K��ʒm����֐�
				std::map<Specify::CAddress, std::map<Status::EnumDirection, std::vector<std::weak_ptr<std::function<Trigger::OnWakeup>>>>> m_triggers;
			};
		}
	}
}

