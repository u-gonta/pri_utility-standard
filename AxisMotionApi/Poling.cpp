#include "Poling.h"
#include "Common/Tool/Variant.h"

#include "WrapperMotionApi/Status.h"
#include "PlcMotionApi/Parameter.h"
#include "PlcMotionApi/ExecutionController.h"
#include "PlcMotionApi/ExecutionRegister.h"
#include "PlcMotionApi/ExecutionAxis.h"
#include "PlcMotionApi/ExecutionDevice.h"

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
					auto buffer = polingSetting.Connect.Other.GetValue<Plc::EnumParameter>(Plc::ParameterOpenController);

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
					postscript << "Plc::Controller::Initialize()";
					auto resultInitialize = Plc::Controller::Initialize(settingController);
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
						postscript << "Plc::Axis::Initialize()";
						auto result = Plc::Axis::Initialize(settingAxis);
						postscript.str("");

						// ���̖��̂��X�V
						status.Name = driver.second.Name;

						// ���̃n���h�����X�V
						status.Handle = result.Axis.Handle;

						// �擪�A�h���X���X�V
						status.Address = settingAxis.Declare.Address;

						// ����\���X�V
						status.Resolution = settingAxis.Declare.Resolution;

						// ���̏�Ԃ�ǉ�
						postscript << "Addition()";
						AdditionStatus(driver.first, status);
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
						postscript << "Plc::Controller::Destroy()";
						Plc::Controller::Destroy(settingController);
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
				//@@@Transfer::Output(Logging::Join(logging, postscript.str()));
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

					// �S���̏�Ԃ��擾
					postscript << "GetStatus()";
					auto statuses = GetStatus();
					postscript.str("");

					// �Ď�����ԍ����擾
					auto size = GetSurveillanceSize();
					std::map<size_t, bool> completes;

					// �Ď��𑖍�
					for (size_t index = 0; index < size; index++)
					{
						// �Ď��������ɐݒ�
						completes[index] = true;
					}

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

						// ���̓��W�X�^�̓ǂݍ���
						postscript << "Plc::Register::Get()";
						auto result = Plc::Register::Get(content);
						postscript.str("");

						// ���̓��W�X�^�̒l���@��̏�Ԃ�ێ�����N���X�֕ϊ�
						auto driver = ConvertInput(result);

						// ���W�X�^�̎�ʂ��X�V
						content.Category = Plc::Register::ConstCategoryO;
						content.Category += Plc::Register::ConstTypeWord;

						// �o�̓��W�X�^�̓ǂݍ���
						postscript << "Plc::Register::Get()";
						result = Plc::Register::Get(content);
						postscript.str("");

						// �o�̓��W�X�^�̒l���@��̏�Ԃ�ێ�����N���X�֕ϊ�
						driver += ConvertOutput(result);

						// ���̂��X�V
						driver.Name = status.second.Name;

						// �ʒu�̒P�ʂ�ϊ�
						driver.Coordinate[Status::Input::CoordinateNegativeLimit] *= status.second.Resolution;
						driver.Coordinate[Status::Input::CoordinatePositiveLimit] *= status.second.Resolution;
						driver.Coordinate[Status::Input::CoordinateCurrent] *= status.second.Resolution;

						// �@��̏�Ԃ�ێ�����N���X���X�V
						input.Drivers[status.first] = driver;

						// �Ď��𑖍�
						for (int index = 0; index < size; index++)
						{
							// �Ď����擾
							auto surveillance = GetSurveillance(index);

							// �Ď��̎��𑖍�
							for (const auto& id : surveillance.Ids)
							{
								// �Ď��̏�Ԃ��m�F
								switch (surveillance.Status)
								{
								case CSurveillance::StatusFinish:
									// �I��
									if (driver.Motion[Status::Input::MotionCompleted] == false && driver.Motion[Status::Input::MotionAbnormal] == false)
									{
										// �ُ�I���Ǝ��s����������OFF
										completes[index] = false;
									}
									break;
								}
							}
						}
					}

					std::vector<size_t> indexs;

					// �Ď��̏�Ԃ𑖍�
					for (const auto& complete : completes)
					{
						// �Ď��̊������m�F
						if (complete.second)
						{
							// �Ď������� �� �Ď����擾
							auto surveillance = GetSurveillance(complete.first);

							Execution::CFinish finish;

							// �Ď��̊�����ʒm
							WakeupFinish(finish, surveillance.Wakeup);

							// �Ď��̍폜�ɓo�^
							indexs.emplace_back(complete.first);
						}
					}

					// �Ď����폜
					EraseSurveillance(indexs);

					// �ǂݍ��݂�ʒm
					NotifyArgument(input);

					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					//@@@Transfer::Output(Logging::Join(logging, postscript.str()));
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
						std::vector<size_t> erases;

						// �������ރN���X�����擾
						auto size = GetWriteSize();

						for (int index = 0; index < size; index++)
						{
							// �������ރN���X���擾
							auto write = GetWrite(index);

							// �������݃N���X���m�F
							if (write == nullptr)
							{
								// �������ރN���X���Ȃ� �� ���̃N���X��
								continue;
							}

							bool valid = true;

							// ���̎w�߂𔻒�
							if (write->Output.Command.type() == typeid(Status::Output::CServo))
							{
								// �T�[�{�𐧌�
								valid = Servo(*write);
							}
							else if (write->Output.Command.type() == typeid(Status::Output::CMoveOrigin))
							{
								// ���_���A
								valid = MoveOrigin(*write);
							}
							else if (write->Output.Command.type() == typeid(Status::Output::CStartJog))
							{
								// �W���O�����s
								valid = StartJog(*write);
							}
							else if (write->Output.Command.type() == typeid(Status::Output::CStopJog))
							{
								// �W���O���~
								valid = StopJog(*write);
							}

							// ���s�̊J�n���m�F
							if (valid)
							{
								// �폜����f�[�^��ǉ�
								erases.emplace_back(index);
							}
						}

						// �o�^���Ă���f�[�^���폜
						PopWrite(erases);

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
				postscript << "Plc::Axis::Destroy()";
				Plc::Axis::Destroy(setting);
				postscript.str("");

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// ���̏�Ԃ��N���A
				m_axisStatuses.clear();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���̏�Ԃ�ǉ�
			/// @param[in]		id		���̎��ʎq
			/// @param[in]		object	���̏�ԃN���X
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::AdditionStatus(int id, const CStatus& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "AdditionStatus()" << Logging::ConstSeparator;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// ���̏�Ԃ�ǉ�
				postscript << "ID:" << id;
				postscript << Logging::ConstSeparator << "����:" << object.Name;
				postscript << Logging::ConstSeparator << "�n���h��:" << Plc::FormatHandle(object.Handle);
				postscript << Logging::ConstSeparator << "�A�h���X:" << Text::FormatX(4, object.Address);
				postscript << Logging::ConstSeparator << "����\:" << object.Resolution;
				m_axisStatuses[id] = object;
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���̏�Ԃ��擾
			/// @param[in]		id	���̎��ʎq
			/// @return			���̏�ԃN���X
			////////////////////////////////////////////////////////////////////////////////
			CStatus CWorker::GetStatus(int id)
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
			/// @brief			�S���̏�Ԃ��擾
			/// @return			�S���̏�ԃN���X
			////////////////////////////////////////////////////////////////////////////////
			std::map<int, CStatus> CWorker::GetStatus()
			{
				std::map<int, CStatus> ret;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncAxisStatus);

				// ���̏�Ԃ��擾
				ret = m_axisStatuses;

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�����Ď����鐔���擾
			/// @return			�����Ď����鐔
			////////////////////////////////////////////////////////////////////////////////
			size_t CWorker::GetSurveillanceSize()
			{
				size_t ret = 0;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillance);

				// �����Ď����鐔���擾
				ret = m_surveillances.size();

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���̊Ď����擾
			/// @param[in]		index	�����Ď�����ԍ�
			/// @return			�����Ď�����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			CSurveillance CWorker::GetSurveillance(size_t index)
			{
				CSurveillance ret;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillance);

				// �����Ď����鐔���m�F
				if (index < 0 || m_surveillances.size() <= index)
				{
					// ���̊Ď�����ԍ����͈͊O
					std::stringstream message;
					message << "�����Ď�����ԍ����͈͊O:" << index;
					throw std::exception(message.str().c_str());
				}

				// �����Ď�����ݒ�N���X���擾
				ret = m_surveillances.at(index);

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�����Ď���ǉ�
			/// @param[in]		object		�����Ď�����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::AdditionSurveillance(const CSurveillance& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "AdditionSurveillance()" << Logging::ConstSeparator;

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillance);

				// ���̊Ď���ǉ�
				for (const auto& id : object.Ids)
				{
					// ���̏�Ԃ��擾
					auto status = GetStatus(id);

					postscript << status.Name << Logging::ConstSeparator << "�Ď����J�n";
					switch (object.Status)
					{
					case CSurveillance::StatusFinish:
						postscript << "(����)";
						break;
					}
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				}
				m_surveillances.emplace_back(object);
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�����Ď����폜
			/// @param[in]		indexs		�����Ď�����ԍ�
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::EraseSurveillance(const std::vector<size_t>& indexs)
			{
				Logging::CObject logging;
				std::stringstream postscript;

				logging.Message << GetName() << "EraseSurveillance()" << Logging::ConstSeparator;

				std::vector<size_t> targets;

				// �C���f�b�N�X�ԍ����R�s�[
				std::copy(indexs.begin(), indexs.end(), std::back_inserter(targets));

				// �C���f�b�N�X�ԍ����~���Ń\�[�g
				std::sort(targets.begin(), targets.end(), std::greater<size_t>());

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillance);

				// �C���f�b�N�X�ԍ��𑖍�
				for (const auto& index : targets)
				{
					// �������ރf�[�^�̃N���X�����m�F
					if (m_surveillances.size() <= index)
					{
						// �������ރf�[�^�͈̔͊O
						continue;
					}

					for (const auto& id : m_surveillances.at(index).Ids)
					{
						// ���̏�Ԃ��擾
						auto status = GetStatus(id);

						postscript << status.Name << Logging::ConstSeparator << "�Ď����I��";
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// �f�[�^���폜
					m_surveillances.erase(m_surveillances.begin() + index);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���̓��W�X�^�̒l���@��̏�Ԃ�ێ�����N���X�֕ϊ�
			/// @param[in]		object	���W�X�^�̒l�N���X
			/// @return			�@��̏�Ԃ�ێ�����N���X
			////////////////////////////////////////////////////////////////////////////////
			Axis::Status::Input::CDriver CWorker::ConvertInput(const Plc::Register::Result::CContent& object)
			{
				Axis::Status::Input::CDriver ret;

				unsigned long warning = 0;
				unsigned long alarm = 0;
				unsigned long coordinate = 0;

				// ���W�X�^�̒l���i�[
				for (const auto& numeric : object.Words)
				{
					switch (numeric.first)
					{
					case 0x00:
						// �^�]�X�e�[�^�X �� �^�]�X�e�[�^�X�̐M���𑖍�
						for (const auto& iterator : MotionApi::Status::RunningIterator())
						{
							// �M���𕪉�
							bool buffer = ((unsigned short)numeric.second >> iterator & 0x01 ? true : false);

							switch (iterator)
							{
							case MotionApi::Status::RunningServo:
								// �^�]��(�T�[�{ON) �� �T�[�{ON
								ret.Motion[Status::Input::MotionServo] = buffer;
								break;
							}
						}
						break;

					case 0x02:
						// ���[�j���O �� ���ʂ��X�V
						warning = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x03:
						// ���[�j���O �� ��ʂ��X�V
						warning |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// ���[�j���O�̐M���𑖍�
						for (const auto& iterator : MotionApi::Status::WarningIterator())
						{
							// ���[�j���O���X�V
							ret.Warning[iterator] = (warning >> iterator & 0x01 ? true : false);
						}
						break;

					case 0x04:
						// �A���[�� �� ���ʂ��X�V
						alarm = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x05:
						// �A���[�� �� ��ʂ��X�V
						alarm |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// �A���[���̐M���𑖍�
						for (const auto& iterator : MotionApi::Status::AlarmIterator())
						{
							// �A���[�����X�V
							ret.Alarm[iterator] = (alarm >> iterator & 0x01 ? true : false);
						}
						break;

					case 0x09:
						// ���[�V�����R�}���h�X�e�[�^�X	�� �R�}���h�X�e�[�^�X�̐M���𑖍�
						for (const auto& iterator : MotionApi::Status::CommandIterator())
						{
							// �M���𕪉�
							bool buffer = ((unsigned short)numeric.second >> iterator & 0x01 ? true : false);

							switch (iterator)
							{
							case MotionApi::Status::CommandRunning:
								// ���s�� �� ���s��
								ret.Motion[Status::Input::MotionRunning] = buffer;
								break;

							case MotionApi::Status::CommandAbnormalFinish:
								// �ُ�I����� �� �ُ�
								ret.Motion[Status::Input::MotionAbnormal] = buffer;
								break;

							case MotionApi::Status::CommandCompleted:
								// ���s���� �� ���s����
								ret.Motion[Status::Input::MotionCompleted] = buffer;
								break;
							}
						}
						break;

					case 0x0C:
						// �ʒu�X�e�[�^�X �� �ʒu�X�e�[�^�X�̐M���𑖍�
						for (const auto& iterator : MotionApi::Status::PositionIterator())
						{
							// �M���𕪉�
							bool buffer = ((unsigned short)numeric.second >> iterator & 0x01 ? true : false);

							switch (iterator)
							{
							case MotionApi::Status::PositionOriginCompleted:
								// ���_���A���� �� ���_���A����
								ret.Motion[Status::Input::MotionOrigin] = buffer;
								break;
							}
						}
						break;

					case 0x16:
						// �@�B���W�n�t�B�[�h�o�b�N�ʒu(APOS) �� ���ʂ��X�V
						coordinate = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x17:
						// �@�B���W�n�t�B�[�h�o�b�N�ʒu(APOS) �� ��ʂ��X�V
						coordinate |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// ���݂̍��W���X�V
						ret.Coordinate[Status::Input::CoordinateCurrent] = (double)(signed long)coordinate;
						break;
					}
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�o�̓��W�X�^�̒l���@��̏�Ԃ�ێ�����N���X�֕ϊ�
			/// @param[in]		object	���W�X�^�̒l�N���X
			/// @return			�@��̏�Ԃ�ێ�����N���X
			////////////////////////////////////////////////////////////////////////////////
			Axis::Status::Input::CDriver CWorker::ConvertOutput(const Plc::Register::Result::CContent& object)
			{
				Axis::Status::Input::CDriver ret;

				unsigned long negativeLimit = 0;
				unsigned long positiveLimit = 0;

				// ���W�X�^�̒l���i�[
				for (const auto& numeric : object.Words)
				{
					switch (numeric.first)
					{
					case 0x66:
						// �������\�t�g���~�b�g �� ���ʂ��X�V
						positiveLimit = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x67:
						// �������\�t�g���~�b�g �� ��ʂ��X�V
						positiveLimit |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// �������\�t�g���~�b�g���X�V
						ret.Coordinate[Status::Input::CoordinatePositiveLimit] = (double)(signed long)positiveLimit;
						break;

					case 0x68:
						// �������\�t�g���~�b�g �� ���ʂ��X�V
						negativeLimit = (unsigned long)numeric.second & 0x0000FFFF;
						break;

					case 0x69:
						// �������\�t�g���~�b�g �� ��ʂ��X�V
						negativeLimit |= (unsigned long)numeric.second << 16 & 0xFFFF0000;

						// �������\�t�g���~�b�g���X�V
						ret.Coordinate[Status::Input::CoordinateNegativeLimit] = (double)(signed long)negativeLimit;
						break;
					}
				}

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			������~
			/// @param[in]		object	��������s����ݒ�̃N���X
			/// @return			true:���s���J�n
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::Suspend(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

				logging.Message << GetName() << "Suspend()" << Logging::ConstSeparator;

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

					// ����̒�~���m�F
					if (object.Output.Command.type() != typeid(Status::Output::CSuspend))
					{
						// �T�[�{�̐���ł͂Ȃ�
						postscript << "���̐���N���X�ƕs��v�F" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// ��Ԃ̒�~���w�肷��N���X�ɕϊ�
					auto output = boost::any_cast<Status::Output::CSuspend>(object.Output.Command);

					Plc::Device::Setting::CSuspend setting;

					// ���̂��X�V
					setting.Name = GetName();

					// ���𑖍�
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Suspend::CAxis axis;

						// ���̏�Ԃ��擾
						auto status = GetStatus(driver.first);

						// ���̃n���h�����X�V
						axis.Handle = status.Handle;

						// ���̃��W�X�^�擪�A�h���X���X�V
						axis.Address = status.Address;

						// ����ǉ�
						setting.Axises.emplace_back(axis);
					}

					// �^�C���A�E�g[ms]���擾
					auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterSuspendTimeout);

					// �^�C���A�E�g[ms]���L�����m�F
					if (Variant::IsIntValue(buffer))
					{
						// �^�C���A�E�g[ms]���X�V
						setting.Timeout = Variant::ConvertInt(buffer);
					}

					// ������~
					postscript << "Plc::Device::Suspend()";
					Plc::Device::Suspend(setting);
					postscript.str("");

					// �����̊������N��
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					ret = true;
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

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�T�[�{�𐧌�
			/// @param[in]		object	��������s����ݒ�̃N���X
			/// @return			true:���s���J�n
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::Servo(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

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

					// �T�[�{�̏�Ԃ��w�肷��N���X�ɕϊ�
					auto output = boost::any_cast<Status::Output::CServo>(object.Output.Command);

					Plc::Device::Setting::CServo setting;

					// ���̂��X�V
					setting.Name = GetName();

					// ������X�V
					setting.Mode = output.Mode;

					// ���𑖍�
					for (const auto& driver : output.Drivers)
					{
						// ���̏�Ԃ��擾
						auto status = GetStatus(driver.first);

						// ���̃n���h����ǉ�
						setting.Handles.emplace_back(status.Handle);

						postscript << status.Name << Logging::ConstSeparator << "�T�[�{:";
						postscript << (setting.Mode == Signal::EnumStatus::StatusOn ? "ON" : "OFF");
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// �^�C���A�E�g[ms]���擾
					auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterServoTimeout);

					// �^�C���A�E�g[ms]���L�����m�F
					if (Variant::IsIntValue(buffer))
					{
						// �^�C���A�E�g[ms]���X�V
						setting.Timeout = Variant::ConvertInt(buffer);
					}

					// �T�[�{�𐧌�
					postscript << "Plc::Device::Servo()";
					Plc::Device::Servo(setting);
					postscript.str("");

					// �����̊������N��
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					ret = true;
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

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���_���A�����s
			/// @param[in]		object	��������s����ݒ�̃N���X
			/// @return			true:���s���J�n
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::MoveOrigin(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

				logging.Message << GetName() << "MoveOrigin()" << Logging::ConstSeparator;

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

					// ���_���A�̎��s���m�F
					if (object.Output.Command.type() != typeid(Status::Output::CMoveOrigin))
					{
						// ���_���A�̎��s�ł͂Ȃ�
						postscript << "���̐���N���X�ƕs��v�F" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// ���_���A���w�肷��N���X�ɕϊ�
					auto output = boost::any_cast<Status::Output::CMoveOrigin>(object.Output.Command);

					Plc::Device::Setting::CMoveOrigin setting;
					CSurveillance surveillance;

					// ���̂��X�V
					setting.Name = GetName();

					// ���𑖍�
					for (auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Origin::CAxis axis;

						// ���̌��_���A���s���ݒ�N���X���擾
						auto buffer = driver.second.Other.GetValue<Plc::EnumParameter>(Plc::ParameterOriginAxis);

						// ���̌��_���A���s���ݒ�N���X���L�����m�F
						if (Variant::IsValue<MotionApi::Device::Setting::Origin::CAxis>(buffer))
						{
							// ���̌��_���A���s���ݒ�N���X���X�V
							axis = Variant::Convert<MotionApi::Device::Setting::Origin::CAxis>(buffer);
						}

						// ���̏�Ԃ��擾
						auto status = GetStatus(driver.first);

						// ���̃n���h�����X�V
						axis.Handle = status.Handle;

						// �֐ߎw��̃f�[�^�����W�X�^�̐擪�A�h���X����̒l�ɍX�V
						axis.Reflection(status.Address);

						// �|�W�V�����̃^�C�v���m�F
						if (axis.PositionType == MotionApi::Device::PositionIndirect)
						{
							// �|�W�V�������֐ߎw�� �� ���_���A�ŏI���s�����̃A�h���X�֍X�V
							axis.PositionData = status.Address + 0x42;
						}

						// ���̐ݒ��ǉ�
						setting.Axises.emplace_back(axis);

						// �Ď����鎲�̎��ʎq��ǉ�
						surveillance.Ids.emplace_back(driver.first);

						postscript << status.Name << Logging::ConstSeparator << "���_���A";
						// ���O�o��
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// �^�C���A�E�g[ms]���擾
					auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterMoveOriginTimeout);

					// �^�C���A�E�g[ms]���L�����m�F
					if (Variant::IsIntValue(buffer))
					{
						// �^�C���A�E�g[ms]���X�V
						setting.Timeout = Variant::ConvertInt(buffer);
					}

					// ���_���A�����s
					postscript << "Plc::Device::MoveOrigin()";
					Plc::Device::MoveOrigin(setting);
					postscript.str("");

					// �Ď��̎�ނ��X�V
					surveillance.Status = CSurveillance::StatusFinish;

					// ���䂵�����ʂ̒ʒm���Ăяo���֐����X�V
					surveillance.Wakeup = object.Wakeup;

					// ���̊Ď���o�^
					AdditionSurveillance(surveillance);

					ret = true;
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

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�W���O���J�n
			/// @param[in]		object	��������s����ݒ�̃N���X
			/// @return			true:���s���J�n
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::StartJog(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

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

					// �W���O�̊J�n���m�F
					if (object.Output.Command.type() != typeid(Status::Output::CStartJog))
					{
						// �W���O�̊J�n�ł͂Ȃ�
						postscript << "���̐���N���X�ƕs��v�F" << object.Output.Command.type().name();
						throw std::exception(postscript.str().c_str());
					}

					// �W���O�̊J�n���w�肷��N���X�ɕϊ�
					auto output = boost::any_cast<Status::Output::CStartJog>(object.Output.Command);

					Plc::Device::Setting::CStartJog setting;

					// ���̂��X�V
					setting.Name = GetName();

					// ���𑖍�
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Jog::CStart axis;

						// ���̃W���O���s���s���ݒ�N���X���擾
						auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterStartJog);

						// ���̃W���O���s���s���ݒ�N���X���L�����m�F
						if (Variant::IsValue<MotionApi::Device::Setting::Jog::CStart>(buffer))
						{
							// ���̃W���O���s���s���ݒ�N���X���X�V
							axis = Variant::Convert<MotionApi::Device::Setting::Jog::CStart>(buffer);
						}

						// ���̃n���h�����X�V
						axis.Handle = GetStatus(driver.first).Handle;

						// ���̐ݒ��ǉ�
						setting.Axises.emplace_back(axis);
					}

					// �W���O�����s
					postscript << "Plc::Device::StartJog()";
					Plc::Device::StartJog(setting);
					postscript.str("");

					// �����̊������N��
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					ret = true;
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

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�W���O���~
			/// @param[in]		object	��������s����ݒ�̃N���X
			/// @return			true:���s���J�n
			////////////////////////////////////////////////////////////////////////////////
			bool CWorker::StopJog(const Execution::CSetting& object)
			{
				bool ret = false;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Execution::CFinish finish;

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

					// �W���O�̒�~���w�肷��N���X�ɕϊ�
					auto output = boost::any_cast<Status::Output::CStopJog>(object.Output.Command);

					Plc::Device::Setting::CStopJog setting;

					// ���̂��X�V
					setting.Name = GetName();

					// ���𑖍�
					for (const auto& driver : output.Drivers)
					{
						MotionApi::Device::Setting::Jog::CStop axis;

						// ���̃W���O��~���s���ݒ�N���X���擾
						auto buffer = output.Other.GetValue<Plc::EnumParameter>(Plc::ParameterStopJog);

						// ���̃W���O��~���s���ݒ�N���X���L�����m�F
						if (Variant::IsValue<std::string>(buffer))
						{
							// ���̃W���O��~���s���ݒ�N���X���X�V
							axis = Variant::Convert<MotionApi::Device::Setting::Jog::CStop>(buffer);
						}

						// ���̃n���h�����X�V
						axis.Handle = GetStatus(driver.first).Handle;

						// ���̐ݒ��ǉ�
						setting.Axises.emplace_back(axis);
					}

					// �W���O���~
					postscript << "Plc::Device::StopJog()";
					Plc::Device::StopJog(setting);
					postscript.str("");

					// �����̊������N��
					postscript << "WakeupFinish()";
					WakeupFinish(finish, object.Wakeup);
					// ���O�o��
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					ret = true;
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

				return ret;
			}
		}
	}
}