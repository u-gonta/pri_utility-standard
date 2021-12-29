#include "Worker.h"
#include "Common/Utility/StopWatch.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�o�^����R�[���o�b�N�֐�
		/// @param[in]		node	�m�[�h
		/// @param[in]		me	���g�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		void __stdcall OnNodeCallbackFunction(GenApi::INode* node, CWorker* me)
		{
			if (me)
			{
				// �R�[���o�b�N����Ăяo��
				me->ReceiveNode(node);
			}
		};

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CWorker::CWorker() : Worker::CTemplate()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CWorker::~CWorker()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�o�C�X���X�g���m�F
		/// @return			�f�o�C�X���X�g�̏��
		////////////////////////////////////////////////////////////////////////////////
		bool CWorker::IsDeviceLost()
		{
			bool ret = false;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "IsDeviceLost()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				do
				{
					// �C���^�[�t�F�[�X���m�F
					if (m_stDevice.IsValid() == false)
					{
						// �C���^�[�t�F�[�X���m�� �� �f�o�C�X���X�g�Ȃ�
						break;
					}

					// �f�[�^�X�g���[�����m�F
					if (m_stDataStream.IsValid() == false)
					{
						// �f�[�^�X�g���[�����m�� �� �f�o�C�X���X�g�Ȃ�
						break;
					}

					// �f�o�C�X���X�g�̏�Ԃ��擾
					ret = m_stDevice->IsDeviceLost();
				} while (false);
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R�[���o�b�N����Ăяo���֐�
		/// @detail			�R�[���o�b�N�������ɌĂяo���Ă��炤
		/// @param[in]		node	�m�[�h
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::ReceiveNode(GenApi::INode* node)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDeviceLost;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReceiveNode()" << Logging::ConstSeparator;

			try
			{
				// �f�o�C�X���X�g�̏�Ԃ��m�F
				if (IsDeviceLost())
				{
					// �f�o�C�X���X�g����
					postscript << "DeviceLost";

					// ��O�̏o��
					Exception::CObject exception = OutputException(errorCode, logging, postscript.str());

					// ��O��ʒm
					NotifyException(exception);
				}
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm
				OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�o�C�X���m�F
		/// @param[in]		setting	�ڑ��̐ݒ�N���X
		/// @param[in]		nodeMap	�C���^�[�t�F�[�X�̃m�[�h
		/// @param[in]		index	�f�o�C�X�̔ԍ�
		////////////////////////////////////////////////////////////////////////////////
		bool CWorker::IsDevice(const Setting::CConnect& setting, const GenApi::INodeMap& nodeMap, const uint32_t index)
		{
			bool ret = false;

			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << GetName() << "IsDevice(" << index << ")" << Logging::ConstSeparator;

			do
			{
				// �J�����̎��ʎq���m�F
				if (setting.Identifier.empty() == false)
				{
					// ���ʎq���� �� IP�A�h���X�őI��
					GenApi::CIntegerPtr deviceIpAddress(nodeMap.GetNode("GevDeviceIPAddress"));

					// IP�A�h���X���L�����m�F
					if (GenApi::IsAvailable(deviceIpAddress))
					{
						std::string ipAddress = deviceIpAddress->ToString().c_str();

						// IP�A�h���X����v���Ă��邩�m�F
						if (setting.Identifier.compare(ipAddress) != 0)
						{
							// �s��v
							break;
						}
					}
				}
				else
				{
					// ���̑� �� �擪�Ɍ��������f�o�C�X��I��
				}

				// �f�o�C�X���L��
				ret = true;
			} while (false);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�p�����[�^���f�t�H���g�ɖ߂�
		/// @param[in]		nodeMap	�C���^�[�t�F�[�X�̃����[�g�m�[�h
		/// @param[in]		object	�f�t�H���g�Ȑݒ�N���X
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DefaultParameter(const GenApi::INodeMap& nodeMap, const Setting::CDefault& setting)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << GetName() << "DefaultParameter()" << Logging::ConstSeparator;

			try
			{
				CParameter parameter;

				// ���[�U�[�ݒ���f�t�H���g�ɕύX
				parameter.WriteText(nodeMap, "UserSetSelector", "Default");

				// ���[�U�ݒ�����[�h
				parameter.Execute(nodeMap, "UserSetLoad");

				// ��荞�݉񐔂�1��
				parameter.WriteText(nodeMap, "AcquisitionMode", "SingleFrame");

				// �t���[���J�n�g���K�̑I��
				parameter.Selector(nodeMap, "TriggerSelector", "FrameStart");

				// �g���K�𖳌�
				parameter.WriteText(nodeMap, "TriggerMode", "Off");

				//// �g���K���\�t�g�E�F�A�g���K
				//parameter.WriteText(nodeMap, "TriggerSource", "Software");

				// ��荞�݂̏����N���X���擾
				Condition::CCapture captureCondition = GetCaptureCondition();

				// �Z���T�̍ő啝���擾
				captureCondition.WidthMaximum = (int)parameter.ReadInteger(nodeMap, "WidthMax");

				int64_t imageWidth = captureCondition.WidthMaximum;

				// �摜�̕����ݒ肳��Ă��� and �Z���T�̍ő啝��菬�������m�F
				if (0 < setting.ImageWidth && setting.ImageWidth < captureCondition.WidthMaximum)
				{
					// �摜�̕���ύX
					imageWidth = setting.ImageWidth;
				}

				// �摜�̕���ύX
				imageWidth = parameter.WriteInteger(nodeMap, "Width", imageWidth);

				// �I�t�Z�b�gX���v�Z
				int64_t offsetX = (int64_t)std::round((double)(captureCondition.WidthMaximum - imageWidth) / 2);

				// �I�t�Z�b�gX��ύX
				parameter.WriteInteger(nodeMap, "OffsetX", offsetX);

				// �Z���T�̍ő卂�����擾
				captureCondition.HeightMaximum = (int)parameter.ReadInteger(nodeMap, "HeightMax");

				int64_t imageHeight = captureCondition.HeightMaximum;

				// �摜�̍������ݒ肳��Ă��� and �Z���T�̍ő卂����菬�������m�F
				if (0 < setting.ImageHeight && setting.ImageHeight < captureCondition.HeightMaximum)
				{
					// �摜�̍�����ύX
					imageHeight = setting.ImageHeight;
				}

				// �摜�̍�����ύX
				imageHeight = parameter.WriteInteger(nodeMap, "Height", imageHeight);

				// �I�t�Z�b�gY���v�Z
				int64_t offsetY = (int64_t)std::round((double)(captureCondition.HeightMaximum - imageHeight) / 2);

				// �I�t�Z�b�gY��ύX
				parameter.WriteInteger(nodeMap, "OffsetY", offsetY);

				// �A�i���O�Q�C���̑I��
				parameter.Selector(nodeMap, "GainSelector", "AnalogAll");

				// �A�i���O�Q�C���͈̔͂�ǂݍ���
				captureCondition.AnalogGain = parameter.ReadRange(nodeMap, "Gain");

				// �f�W�^���Q�C���̑I��
				parameter.Selector(nodeMap, "GainSelector", "DigitalAll");

				// �f�W�^���Q�C���͈̔͂�ǂݍ���
				captureCondition.DigitalGain = parameter.ReadRange(nodeMap, "Gain");

				// �I�����Ԃ͈̔͂�ǂݍ���
				captureCondition.ExposureTime = parameter.ReadRange(nodeMap, "ExposureTime");

				// ��荞�݂̏������X�V
				SetCaptureCondition(captureCondition);
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			Heartbeat���X�V
		/// @param[in]		nodeMap	�C���^�[�t�F�[�X�̃����[�g�m�[�h
		/// @param[in]		timeout	�^�C���A�E�g[us]
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::UpdateHeartbeat(const GenApi::INodeMap& nodeMap, double timeout)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << GetName() << "UpdateHeartbeat()" << Logging::ConstSeparator;

			try
			{
				// Heartbeat�^�C���A�E�g(�W��)���擾
				GenApi::CValuePtr deviceHeartbeatTimeout(nodeMap.GetNode("DeviceLinkHeartbeatTimeout"));

				// Heartbeat�^�C���A�E�g(�W��)�̗L�����m�F
				if (deviceHeartbeatTimeout == nullptr)
				{
					// Heartbeat�^�C���A�E�g(�W��)�Ȃ� �� Heartbeat�^�C���A�E�g(�񐄏�)�̗L�����m�F
					deviceHeartbeatTimeout = nodeMap.GetNode("GevHeartbeatTimeout");

					// Heartbeat�^�C���A�E�g(�񐄏�)�̗L�����m�F
					if (deviceHeartbeatTimeout.IsValid() == false)
					{
						// Heartbeat�^�C���A�E�g(�񐄏�)�Ȃ� �� ��O�𔭖C
						throw std::exception("Heartbeat�Ȃ�");
					}

					// �^�C���A�E�g�̒P�ʂ�ϊ� [us] �� [ms]
					timeout *= 1000;
				}

				std::stringstream buffer;

				// �^�C���A�E�g�𕶎���ɕϊ�
				buffer << timeout;

				// Heartbeat�^�C���A�E�g���X�V
				postscript << "GenApi::HeartbeatTimeout(" << buffer.str() << ")";
				deviceHeartbeatTimeout->FromString(buffer.str().c_str());
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			DeviceLost�̃R�[���o�b�N��o�^
		/// @param[in]		nodeMap	�C���^�[�t�F�[�X�̃��[�J���m�[�h
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::RegisterDeviceLost(const GenApi::INodeMap& nodeMap)
		{
			Logging::CObject logging;
			std::stringstream postscript;

			logging.Message << GetName() << "RegisterDeviceLost()" << Logging::ConstSeparator;

			try
			{
				// DeviceLost���擾
				postscript << "GenApi::GetNode(EventDeviceLost)";
				GenApi::CNodePtr nodeCallback(nodeMap.GetNode("EventDeviceLost"));
				postscript.str("");

				// DeviceLost���m�F
				if (nodeCallback)
				{
					// �r������
					std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

					// �R�[���o�b�N�֐���o�^
					postscript << "GenApi::RegisterCallback()";
					StApi::RegisterCallback(nodeCallback, &OnNodeCallbackFunction, this);
					postscript.str("");
				}
				else
				{
					// DeviceLost�Ȃ� �� ��O�𔭖C
					throw std::exception("DeviceLost�Ȃ�");
				}
				
				// EventSelector���擾
				postscript << "GenApi::GetNode(EventSelector)";
				GenApi::CEnumerationPtr enumerationEventSelector(nodeMap.GetNode("EventSelector"));
				postscript.str("");

				// DeviceLost���擾
				postscript << "GenApi::GetEntryByName(DeviceLost)";
				GenApi::CEnumEntryPtr enumerationEntryEventSelectorEntry(enumerationEventSelector->GetEntryByName("DeviceLost"));
				postscript.str("");

				// DeviceLost��I��
				postscript << "GenApi::SetIntValue(DeviceLost)";
				enumerationEventSelector->SetIntValue(enumerationEntryEventSelectorEntry->GetValue());
				postscript.str("");

				// EventNotification���擾
				postscript << "GenApi::GetNode(EventNotification)";
				GenApi::CEnumerationPtr enumerationEventNotification(nodeMap.GetNode("EventNotification"));
				postscript.str("");

				// On���擾
				postscript << "GenApi::GetEntryByName(On)";
				GenApi::CEnumEntryPtr enumerationEntryEventNotificationEntry(enumerationEventNotification->GetEntryByName("On"));
				postscript.str("");

				// On��I��
				postscript << "GenApi::SetIntValue(On)";
				enumerationEventNotification->SetIntValue(enumerationEntryEventNotificationEntry->GetValue());
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏���
				std::stringstream message;

				message << postscript.str() << Logging::ConstSeparator << e.what();

				// ��O�𔭖C
				throw std::exception(message.str().c_str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			������
		/// @detail			�������̃^�C�~���O�ŌĂяo�����
		///					�����C�u�����̏������Ȃǂ��s��
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::InitializeDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// ���C�u������������
				postscript << "StApi::StApiInitialize()";
				StApi::StApiInitialize();
				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				StApi::EStSystemVendor_t systemVendor = StApi::EStSystemVendor_t::StSystemVendor_Default;
				StApi::EStInterfaceType_t interfaceType = StApi::EStInterfaceType_t::StInterfaceType_All;

				// �V�X�e�����W���[�����J��
				postscript << "StApi::CreateIStSystem()";
				m_stSystem.Reset(StApi::CreateIStSystem(systemVendor, interfaceType));
				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
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
		///					�����C�u�����̔j���Ȃǂ��s��
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DestroyDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �V�X�e�����J��
				postscript << "StApi::System::Reset()";
				m_stSystem.Reset();
				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// ���C�u������j��
				postscript << "StApi::StApiTerminate()";
				StApi::StApiTerminate();
				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
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
		/// @detail			�ڑ��̃^�C�~���O�ŌĂяo�����
		///					���f�[�^�X�g���[���̊J�n�Ȃǂ��s��
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::ConnectDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberConnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

			try
			{
				// �ڑ��ݒ�N���X���擾
				Setting::CConnect connectSetting = GetConnectSetting();

				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �C���^�[�t�F�[�X�����擾
				uint32_t interfaceCount = m_stSystem->GetInterfaceCount();
				postscript << "Interface:" << interfaceCount;

				// �C���^�[�t�F�[�X�����m�F
				if (interfaceCount <= 0)
				{
					errorCode = Exception::CodeGrabberInterface;

					// ��O�𔭖C
					throw std::exception("�C���^�[�t�F�[�X�Ȃ�");
				}
				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// �C���^�[�t�F�[�X�̊m�F
				bool checkInterface = true;

				// �C���^�[�t�F�[�X�Ɛڑ�����Ă���J���������𑖍� ���C���^�[�t�F�[�X�̊m�F���L���̏ꍇ
				for (uint32_t interfaceIndex = 0; interfaceIndex < interfaceCount && checkInterface; interfaceIndex++)
				{
					// �C���^�[�t�F�[�X���擾
					postscript << "StApi::GetIStInterface(" << interfaceIndex << ")";
					StApi::IStInterface *stInterface = m_stSystem->GetIStInterface(interfaceIndex);
					postscript.str("");

					// �ڑ�����Ă���f�o�C�X(�J����)�����o���A�f�o�C�X(�J����)���X�g���X�V
					postscript << "StApi::GetDeviceCount()";
					stInterface->UpdateDeviceList();
					postscript.str("");

					// �f�o�C�X�����擾
					postscript << "StApi::GetDeviceCount(" << interfaceIndex << ")";
					const uint32_t deviceCount = stInterface->GetDeviceCount();
					postscript << Logging::ConstSeparator << "Device:" << deviceCount;

					// �f�o�C�X�����m�F
					if (deviceCount <= 0)
					{
						// �f�o�C�X�Ȃ� �� ���̃C���^�[�t�F�[�X��
						continue;
					}
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �C���^�[�t�F�[�X�|�C���^�[�̃m�[�h���擾
					postscript << "GenApi::GetINodeMap()";
					GenApi::CNodeMapPtr nodeMap(stInterface->GetIStPort()->GetINodeMap());
					postscript.str("");

					// �f�o�C�X��I��
					postscript << "GenApi::DeviceSelector()";
					GenApi::CIntegerPtr deviceSelector(nodeMap->GetNode("DeviceSelector"));
					postscript.str("");

					// �f�o�C�X�����擾
					postscript << "StApi::GetMax()";
					const int64_t deviceMax = deviceSelector->GetMax();
					postscript << Logging::ConstSeparator << "Device:" << deviceMax;
					// ���O�o��
					postscript << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// �f�o�C�X�����𑖍�
					for (uint32_t indexDevice = 0; indexDevice <= deviceMax; indexDevice++)
					{
						// �f�o�C�X�̑I��
						deviceSelector->SetValue(indexDevice);

						// �f�o�C�X���m�F
						if (IsDevice(connectSetting, *nodeMap, indexDevice) == false)
						{
							// �s��v �� ���̃f�o�C�X��
							continue;
						}

						// �C���^�[�t�F�[�X���X�V
						postscript << "StApi::Interface::CreateIStDevice(" << indexDevice << ")";
						m_stDevice.Reset(stInterface->CreateIStDevice((size_t)indexDevice));
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �f�[�^�X�g���[�����X�V
						postscript << "StApi::Interface::CreateIStDataStream()";
						m_stDataStream.Reset(m_stDevice->CreateIStDataStream());
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �f�t�H���g�̐ݒ�N���X���擾
						Setting::CDefault defaultSetting = GetDefaultSetting();

						// �����[�g�̃m�[�h���擾
						GenApi::CNodeMapPtr remoteNode(m_stDevice->GetRemoteIStPort()->GetINodeMap());

						// �p�����[�^���f�t�H���g�ɖ߂�
						postscript << "DefaultParameter()";
						DefaultParameter(*remoteNode, defaultSetting);
						postscript.str("");

						// Heartbeat���X�V
						postscript << "StApi::Interface::UpdateHeartbeat()";
						UpdateHeartbeat(*remoteNode, defaultSetting.HeartbeatTimeout);
						postscript.str("");

						// ���[�J���̃m�[�h���擾
						GenApi::CNodeMapPtr localNode(m_stDevice->GetLocalIStPort()->GetINodeMap());

						// DeviceLost�̃R�[���o�b�N��o�^
						postscript << "StApi::Interface::RegisterHeartbeat()";
						RegisterDeviceLost(*localNode);
						postscript.str("");

						// �C�x���g�̃X���b�h���J�n
						postscript << "StApi::Interface::StartEventAcquisitionThread()";
						m_stDevice->StartEventAcquisitionThread();
						postscript.str("");

						// �f�[�^�X�g���[���̊J�n
						postscript << "StApi::DataStream::StartAcquisition()";
						m_stDataStream->StartAcquisition();
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// �C���^�[�t�F�[�X�̊m�F������
						checkInterface = false;
						break;
					}
				}

				// �C���^�[�t�F�[�X and �f�[�^�X�g���[�����m�F
				if (m_stDevice.IsValid() == false || m_stDataStream.IsValid() == false)
				{
					errorCode = Exception::CodeGrabberDevice;

					// ��O�𔭖C
					throw std::exception("�Y������J�����Ȃ�");
				}
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ؒf
		/// @detail			�ؒf�̃^�C�~���O�ŌĂяo�����
		///					���f�[�^�X�g���[���̊J���Ȃǂ��s��
		///					���p�������֐�
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::DisconnectDerived()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDisconnect;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �f�o�C�X���X�g
				bool deviceLost = false;

				// �C���^�[�t�F�[�X���m�F
				if (m_stDevice.IsValid())
				{
					// �f�o�C�X���X�g���擾
					deviceLost = m_stDevice->IsDeviceLost();

					// �f�o�C�X���X�g���m�F
					if (deviceLost == false)
					{
						// �f�o�C�X���X�g�Ȃ� �� �C���^�[�t�F�[�X�̎�荞�݂��~
						postscript << "StApi::Interface::AcquisitionStop()";
						//@@@m_stDevice->AcquisitionStop();
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}

					// �C�x���g�̃X���b�h���~
					postscript << "StApi::Interface::StopEventAcquisitionThread()";
					m_stDevice->StopEventAcquisitionThread();
					postscript.str("");
				}

				// �f�[�^�X�g���[�����m�F
				if (m_stDataStream.IsValid())
				{
					// �f�o�C�X���X�g and �B�������m�F
					if (deviceLost == false && m_stDataStream->IsGrabbing())
					{
						// �f�o�C�X���X�g�Ȃ� �� �f�[�^�X�g���[���̎�荞�݂��~
						postscript << "StApi::DataStream::StopAcquisition()";
						//@@@m_stDataStream->StopAcquisition();
						// ���O�o��
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}
				}

				// �f�[�^�X�g���[�������
				postscript << "StApi::DataStream::Reset()";
				m_stDataStream.Reset();
				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// �C���^�[�t�F�[�X���J��
				postscript << "StApi::Interface::Reset()";
				m_stDevice.Reset();
				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ڑ��̏�Ԃ��擾
		///	@detail			�ڑ��̏�Ԃ��擾����^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @return			true:�ڑ��ς� / false:���ڑ�
		////////////////////////////////////////////////////////////////////////////////
		bool CWorker::GetConnected()
		{
			bool ret = false;

			do
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �f�[�^�X�g���[�����m�F
				if (m_stDataStream.IsValid() == false)
				{
					// �f�[�^�X�g���[�����m��
					break;
				}

				// �C���^�[�t�F�[�X���m�F
				if (m_stDevice.IsValid() == false)
				{
					// �C���^�[�t�F�[�X���m��
					break;
				}

				ret = true;
			} while (false);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�ڑ��ς݂��m�F
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::IsConnected()
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberDevice;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "IsConnected()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �f�[�^�X�g���[�����m�F
				if (m_stDataStream.IsValid() == false)
				{
					errorCode = Exception::CodeGrabberDataStream;

					// ��O�𔭖C
					throw std::exception("�f�[�^�X�g���[�����m��");
				}
				postscript.str("");

				// �C���^�[�t�F�[�X���m�F
				if (m_stDevice.IsValid() == false)
				{
					errorCode = Exception::CodeGrabberInterface;

					// ��O�𔭖C
					throw std::exception("�C���^�[�t�F�[�X���m��");
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
		/// @brief			�A�i���O�Q�C����ǂݍ���
		/// @detail			�A�i���O�Q�C���l��ǂݍ��ރ^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @return			�A�i���O�Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadAnalogGainDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadAnalogGainDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �ڑ��ς݂��m�F
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// �����[�g�̃m�[�h���擾
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// �A�i���O�Q�C���̑I��
				postscript << "Parameter::Selector(GainSelector, AnalogAll)";
				parameter.Selector(*nodeMap, "GainSelector", "AnalogAll");
				postscript.str("");

				// �A�i���O�Q�C����ǂݍ���
				postscript << "Parameter::ReadDouble(Gain)";
				ret = parameter.ReadDouble(*nodeMap, "Gain");
				postscript << Logging::ConstSeparator << "�l:" << ret;
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�A�i���O�Q�C������������
		/// @detail			�A�i���O�Q�C���l���������ރ^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @param[in]		value	�A�i���O�Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteAnalogGainDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteAnalogGainDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �ڑ��ς݂��m�F
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// �����[�g�̃m�[�h���擾
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// �A�i���O�Q�C���̑I��
				postscript << "Parameter::Selector(GainSelector, AnalogAll)";
				parameter.Selector(*nodeMap, "GainSelector", "AnalogAll");
				postscript.str("");

				// �A�i���O�Q�C������������
				postscript << "Parameter::WriteDouble()";
				parameter.WriteDouble(*nodeMap, "Gain", value);
				postscript << Logging::ConstSeparator << "�l:" << value;
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�W�^���Q�C����ǂݍ���
		/// @detail			�f�W�^���Q�C���l��ǂݍ��ރ^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @return			�f�W�^���Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadDigitalGainDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadDigitalGainDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �ڑ��ς݂��m�F
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// �����[�g�̃m�[�h���擾
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// �f�W�^���Q�C���̑I��
				postscript << "Parameter::Selector(GainSelector, DigitalAll)";
				parameter.Selector(*nodeMap, "GainSelector", "DigitalAll");
				postscript.str("");

				// �f�W�^���Q�C����ǂݍ���
				postscript << "Parameter::ReadDouble(Gain)";
				ret = parameter.ReadDouble(*nodeMap, "Gain");
				postscript << Logging::ConstSeparator << "�l:" << ret;
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�W�^���Q�C������������
		/// @detail			�f�W�^���Q�C���l���������ރ^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @param[in]		value	�f�W�^���Q�C���l
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteDigitalGainDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteDigitalGainDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �ڑ��ς݂��m�F
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// �����[�g�̃m�[�h���擾
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// �f�W�^���Q�C���̑I��
				postscript << "Parameter::Selector(GainSelector, DigitalAll)";
				parameter.Selector(*nodeMap, "GainSelector", "DigitalAll");
				postscript.str("");

				// �f�W�^���Q�C������������
				postscript << "Parameter::WriteDouble()";
				parameter.WriteDouble(*nodeMap, "Gain", value);
				postscript << Logging::ConstSeparator << "�l:" << value;
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�I�����Ԃ�ǂݍ���
		/// @detail			�I�����Ԃ�ǂݍ��ރ^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @return			�I������
		////////////////////////////////////////////////////////////////////////////////
		double CWorker::ReadExposureTimeDerived()
		{
			double ret = 0;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterGet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "ReadExposureTimeDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �ڑ��ς݂��m�F
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// �����[�g�̃m�[�h���擾
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// �I�����Ԃ�ǂݍ���
				postscript << "Parameter::ReadDouble(ExposureTime)";
				ret = parameter.ReadDouble(*nodeMap, "ExposureTime");
				postscript << Logging::ConstSeparator << "�l:" << ret;
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�I�����Ԃ���������
		/// @detail			�I�����Ԃ��������ރ^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @param[in]		value	�I������
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::WriteExposureTimeDerived(double value)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberParameterSet;
			int deviceErrorCode = Exception::DeviceCodeSuccess;

			logging.Message << GetName() << "WriteExposureTimeDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �ڑ��ς݂��m�F
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				CParameter parameter;

				// �����[�g�̃m�[�h���擾
				postscript << "StApi::Interface::GetRemoteIStPort()::GetINodeMap()";
				GenApi::CNodeMapPtr nodeMap(m_stDevice->GetRemoteIStPort()->GetINodeMap());
				postscript.str("");

				// �f�W�^���Q�C���̑I��
				postscript << "Parameter::Selector(ExposureTimeSelector, Common)";
				parameter.Selector(*nodeMap, "ExposureTimeSelector", "Common");
				postscript.str("");

				// �I�����Ԃ���������
				postscript << "Parameter::WriteDouble()";
				parameter.WriteDouble(*nodeMap, "ExposureTime", value);
				postscript << Logging::ConstSeparator << "�l:" << value;
				// ���O�o��
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			��荞��
		/// @detail			�摜����荞�ރ^�C�~���O�ŌĂяo�����
		///					���p�������֐�
		/// @return			�摜�̃N���X
		////////////////////////////////////////////////////////////////////////////////
		Image::CObject CWorker::CaptureDerived()
		{
			Image::CObject ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeGrabberCapture;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << GetName() << "CaptureDerived()" << Logging::ConstSeparator;

			try
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncLibrary);

				// �ڑ��ς݂��m�F
				postscript << "IsConnected()";
				IsConnected();
				postscript.str("");

				// ��荞�ݒ����m�F
				if (m_stDataStream->IsGrabbing() == false)
				{
					errorCode = Exception::CodeGrabberCaptureNotStart;

					// ��O�𔭖C
					throw std::exception("�f�[�^�X�g���[���̎�荞�ݖ����{");
				}

				// �f�t�H���g�Ȑݒ�N���X���擾
				Setting::CDefault defaultSetting = GetDefaultSetting();

				// ��荞�݂�v��
				postscript << "StApi::Interface::AcquisitionStart()";
				m_stDevice->AcquisitionStart();
				postscript.str("");

				// ��荞�ݑ҂�
				postscript << "StApi::DataStream::RetrieveBuffer(" << defaultSetting.CaptureTimeout << ")";
				StApi::CIStStreamBufferPtr stStreamBuffer(m_stDataStream->RetrieveBuffer((uint32_t)defaultSetting.CaptureTimeout));

				// �摜�̊m�F
				if (stStreamBuffer.IsValid() == false)
				{
					errorCode = Exception::CodeGrabberCaptureTimeout;

					// ��O�𔭖C
					throw std::exception("�^�C���A�E�g");
				}
				postscript.str("");

				// �摜�̏����擾
				postscript << "StApi::DataStreamBuffer::IsImagePresent()";
				if (stStreamBuffer->GetIStStreamBufferInfo()->IsImagePresent() == false)
				{
					errorCode = Exception::CodeGrabberCaptureNotImage;

					// ��O�𔭖C
					throw std::exception("�摜�f�[�^�Ȃ�");
				}
				postscript.str("");

				// �摜�f�[�^���擾
				postscript << "StApi::DataStreamBuffer::GetIStImage()";
				StApi::IStImage *stImage = stStreamBuffer->GetIStImage();

				StApi::EStPixelFormatNamingConvention_t format = stImage->GetImagePixelFormat();
				int depth = 0;
				int width = (int)stImage->GetImageWidth();
				int height = (int)stImage->GetImageHeight();

				// �摜�t�H�[�}�b�g
				postscript << Logging::ConstSeparator << format;

				switch (format)
				{
				case StApi::EStPixelFormatNamingConvention_t::StPFNC_Mono8:
					// ���m�N��8�r�b�g
					depth = 8;
					break;

				default:
					// ���̑� �� ���Ή�
					errorCode = Exception::CodeGrabberCaptureNotFormat;

					// ��O�𔭖C
					throw std::exception("���Ή�");
				}
				postscript.str("");

				// �摜�f�[�^�𐶐�
				postscript << "Image::Update()";
				ret.Update(depth, height, width, (const unsigned char*)stImage->GetImageBuffer());
				postscript.str("");

				// ���O�o��
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}
			catch (const std::exception& e)
			{
				// ��O�̏��� �� ��O��ʒm�����C
				throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
			}

			return ret;
		}
	}
}