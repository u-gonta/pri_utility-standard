#pragma once

#include "Grabber/Worker.h"
#include "Parameter.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      �J�����̐�����s���N���X
		///				�� �J�����̐�����s���e���v���[�g�̃N���X����p��
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Worker::CTemplate
		{
		public:
			// �R���X�g���N�^
			CWorker();

			// �f�X�g���N�^
			~CWorker() override;

		public:
			// �R�[���o�b�N����Ăяo���֐�
			void ReceiveNode(GenApi::INode* node);

		protected:
			// ������
			void InitializeDerived() override;

			// �j��
			void DestroyDerived() override;

			// �ڑ�
			void ConnectDerived() override;

			// �ؒf
			void DisconnectDerived() override;

			// �ڑ��̏�Ԃ��擾
			bool GetConnected() override;

			// �A�i���O�Q�C����ǂݍ���
			double ReadAnalogGainDerived() override;

			// �A�i���O�Q�C������������
			void WriteAnalogGainDerived(double value) override;

			// �f�W�^���Q�C����ǂݍ���
			double ReadDigitalGainDerived() override;

			// �f�W�^���Q�C������������
			void WriteDigitalGainDerived(double value) override;

			// �I�����Ԃ�ǂݍ���
			double ReadExposureTimeDerived() override;

			// �I�����Ԃ���������
			void WriteExposureTimeDerived(double value) override;

			// ��荞��
			Image::CObject CaptureDerived() override;

		private:
			// �f�o�C�X���X�g���m�F
			bool IsDeviceLost();

			// �f�o�C�X���m�F
			bool IsDevice(const Setting::CConnect& setting, const GenApi::INodeMap& nodeMap, const uint32_t index);

			// �p�����[�^���f�t�H���g�ɖ߂�
			void DefaultParameter(const GenApi::INodeMap& nodeMap, const Setting::CDefault& setting) throw(...);

			// HeartBeat���X�V
			void UpdateHeartbeat(const GenApi::INodeMap& nodeMap, double timeout) throw(...);

			// DeviceLost�̃R�[���o�b�N��o�^
			void RegisterDeviceLost(const GenApi::INodeMap& nodeMap) throw(...);

			// �ڑ��ς݂��m�F
			void IsConnected();

		private:
			//! �r������N���X(���C�u�����p)
			std::recursive_mutex m_asyncLibrary;

			//! �V�X�e��
			StApi::CIStSystemPtr m_stSystem;

			//! �C���^�[�t�F�[�X
			StApi::CIStDevicePtr m_stDevice;

			//! �f�[�^�X�g���[��
			StApi::CIStDataStreamPtr m_stDataStream;
		};
	}
}