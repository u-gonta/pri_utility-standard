#pragma once

#include "Grabber/Worker.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      �Q�C����I�����Ԃ̐���A�B�����s���N���X
		///				�� �J�����̐�����s���e���v���[�g�̃N���X����h��
		/// @detail		�Q�C����I�����Ԃ̐���A�B�����s��
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Worker::CTemplate
		{
		public:
			// �R���X�g���N�^
			CWorker();

			// �f�X�g���N�^
			~CWorker();

		public:
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
				
		private:
			// �ڑ��̏�Ԃ��X�V
			void SetConnected(bool value);

		private:
			//! �r������N���X(�_�~�[�̃f�[�^�p)
			std::recursive_mutex m_asyncDmy;

			//! �ڑ��̏�ԃt���O
			bool m_connected;

			//! �A�i���O�Q�C���̒l
			double m_analogGain;

			//! �f�W�^���Q�C���̒l
			double m_digitalGain;

			//! �I�����Ԃ̒l
			double m_exposureTime;

			//! �摜
			Image::CObject m_image;
		};
	}
}
