#pragma once

#include "Axis/ResultPoling.h"

namespace Standard
{
	namespace Axis
	{
		namespace Result
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CInitialize
			/// @brief      �����������������ʃN���X
			///				�� �����Ď��������ʂ̃X�[�p�[�N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CInitialize
				: virtual public Result::CSuper
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
					CSuper::Update(object);
				}
			};

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CServo
			/// @brief      ���̃T�[�{�𐧌䂵�����ʃN���X
			///				�� ���̃T�[�{�𐧌䂵�����ʂ̊�b�N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CServo
				: virtual public Result::Servo::CBasis
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CServo() : CBasis()
				{

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
					Result::Servo::CBasis::Update(object);
				}
			};
		}
	}
}