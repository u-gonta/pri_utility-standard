#pragma once

#include "Axis/Poling.h"

#include "PlcMotionApi/Result.h"

namespace Standard
{
	namespace Axis
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CStatus
			/// @brief      ���̏�ԃN���X
			////////////////////////////////////////////////////////////////////////////////
			class CStatus
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CStatus()
				{
					Name = "";
					Handle = 0;
					Address = 0x8000;
					Resolution = 1;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CStatus& operator = (const CStatus& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CStatus()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CStatus& object)
				{
					this->Name = object.Name;
					this->Handle = object.Handle;
					this->Address = object.Address;
					this->Resolution = object.Resolution;
				}

			public:
				//! ���̖���
				std::string Name;

				//! ���̃n���h��
				unsigned long Handle;

				//! ���̃��W�X�^�擪�A�h���X
				unsigned short Address;

				//! �ʒu�̕���\
				double Resolution;
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      ���̐���A���̊Ď��A��Ԃ�ʒm����N���X
			///				�� ���̐���A���̊Ď��A��Ԃ�ʒm����e���v���[�g�̃N���X����p��
			/// @detail		���̐���A���̊Ď��A��Ԃ̒ʒm���s��
			////////////////////////////////////////////////////////////////////////////////
			class CWorker
				: virtual public Worker::CTemplate
			{
			public:
				// �R���X�g���N�^
				CWorker();

				// �f�X�g���N�^
				~CWorker() override;

			protected:
				// ������
				void InitializeDerived() override;

				// �j��
				void DestroyDerived() override;

				// ���g���C���m�F
				bool IsRetry() override;

				// ���g���C�̒x�����Ԃ��擾
				int GetDelayRetry() override;

				// �ǂݍ���
				void ReadDerived() override;

				// ��������
				void WriteDerived() override;

			public:
				// �ڑ��̏�Ԃ��擾
				bool GetConnected();

			private:
				// �ڑ��ς݂��m�F
				void IsConnected();

				// �����J��
				void Clear();

				// ����ǉ�
				void Addition(int id, const CStatus& status);

				// �����擾
				CStatus Get(int id);

				// �S�����擾
				std::map<int, CStatus> Get();

				// �T�[�{�𐧌�
				void Servo(const Surveillance::CSetting& object);

				// �W���O�����s
				void StartJog(const Surveillance::CSetting& object);

				// �W���O���~
				void StopJog(const Surveillance::CSetting& object);

			private:
				//! �r������N���X(�R���g���[���̏��N���X�p)
				std::recursive_mutex m_asyncController;

				//! �R���g���[�����J�������ʃN���X
				MotionApi::Controller::Result::COpen m_controller;

				//! �r������N���X(���̏�ԗp)
				std::recursive_mutex m_asyncAxisStatus;

				//! ���̏�ԃN���X
				std::map<int, CStatus> m_axisStatuses;

				//! �r������N���X(������Ď�����ݒ�̃N���X�p)
				std::recursive_mutex m_asyncSurveillance;

				//! ������Ď�����ݒ�̃N���X
				std::vector<Surveillance::CSetting> m_surveillances;
			};
		}
	}
}
