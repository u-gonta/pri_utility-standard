#pragma once

#include "Signal/Result.h"

namespace Standard
{
	namespace Axis
	{
		namespace Write
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CBasis
			/// @brief      �����Ď��������ʂ̊�b�N���X
			///				�� �����Ď��������ʂ̃X�[�p�[�N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CBasis
				: virtual public CSuper
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CBasis()
				{
					DeviceHandle = NULL;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CBasis& operator = (const CBasis& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CBasis()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CBasis& object)
				{
					// ����̊֐�
					CSuper::Update(object);

					this->DeviceHandle = object.DeviceHandle;
				}

			public:
				//! �f�o�C�X�̃n���h��
				unsigned long DeviceHandle;
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CInitialize
			/// @brief      ���̏������𐧌䂷����N���X
			///				�� �����Ď��������ʂ̊�b�N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CInitialize
				: virtual public CBasis
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CInitialize()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CInitialize& operator = (const CInitialize& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CInitialize()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CInitialize& object)
				{
					// ����̊֐�
					CBasis::Update(object);
				}
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CDestroy
			/// @brief      ���̔j���𐧌䂷����N���X
			///				�� �����Ď��������ʂ̊�b�N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CDestroy
				: virtual public CBasis
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CDestroy()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CDestroy& operator = (const CDestroy& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CDestroy()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CDestroy& object)
				{
					// ����̊֐�
					CBasis::Update(object);
				}
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CServo
			/// @brief      ���̃T�[�{�𐧌䂷����N���X
			///				�� ���𐧌䂷����̊�b�N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CServo
				: virtual public CBasis
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CServo() : CBasis()
				{
					Mode = Signal::StatusOff;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CServo& operator = (const CServo& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CServo() override
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CServo& object)
				{
					// ����̊֐�
					CBasis::Update(object);

					this->Mode = object.Mode;
				}

			public:
				//! �T�[�{�̏��
				Signal::EnumStatus Mode;
			};
		}
	}
}