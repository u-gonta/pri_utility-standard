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
			/// @class      CSurveillance
			/// @brief      �����Ď�����ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			class CSurveillance
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @enum			�Ď��̎��
				////////////////////////////////////////////////////////////////////////////////
				enum EnumStatus
				{
					StatusFinish		// �I�����Ď�
				};

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CSurveillance()
				{
					Status = StatusFinish;
					Wakeup.reset();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CSurveillance& operator = (const CSurveillance& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CSurveillance()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CSurveillance& object)
				{
					this->Status = object.Status;
					this->Ids.clear();
					std::copy(object.Ids.begin(), object.Ids.end(), std::back_inserter(this->Ids));
					this->Wakeup = object.Wakeup;
				}

			public:
				//! �Ď��̎��
				EnumStatus Status;

				//! ���̎��ʎq
				std::vector<int> Ids;

				//! ���䂵�����ʂ̒ʒm���Ăяo���֐�
				std::weak_ptr<std::function<Execution::OnWakeup>> Wakeup;
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

				// ���̏�Ԃ�ǉ�
				void AdditionStatus(int id, const CStatus& object);

				// ���̏�Ԃ��擾
				CStatus GetStatus(int id);

				// �S���̏�Ԃ��擾
				std::map<int, CStatus> GetStatus();

				// �����Ď����鐔���擾
				size_t GetSurveillanceSize();

				// ���̊Ď����擾
				CSurveillance GetSurveillance(size_t index);

				// ���̊Ď���ǉ�
				void AdditionSurveillance(const CSurveillance& object);

				// ���̊Ď����폜
				void EraseSurveillance(const std::vector<size_t>& indexs);

				// ���̓��W�X�^�̒l���@��̏�Ԃ�ێ�����N���X�֕ϊ�
				Axis::Status::Input::CDriver ConvertInput(const Plc::Register::Result::CContent& object);

				// �o�̓��W�X�^�̒l���@��̏�Ԃ�ێ�����N���X�֕ϊ�
				Axis::Status::Input::CDriver ConvertOutput(const Plc::Register::Result::CContent& object);

				// ������~
				bool Suspend(const Execution::CSetting& object);

				// �T�[�{�𐧌�
				bool Servo(const Execution::CSetting& object);

				// ���_���A�����s
				bool MoveOrigin(const Execution::CSetting& object);

				// �W���O���J�n
				bool StartJog(const Execution::CSetting& object);

				// �W���O���~
				bool StopJog(const Execution::CSetting& object);

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
				std::vector<CSurveillance> m_surveillances;
			};
		}
	}
}
