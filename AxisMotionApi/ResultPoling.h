#pragma once

#include "Signal/Result.h"

namespace Standard
{
	namespace Axis
	{
		namespace Result
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CServo
			/// @brief      �T�[�{����̌��ʃN���X
			///				�� ��Ԃ��Ď��������ʂ̊�b�N���X����p��
			////////////////////////////////////////////////////////////////////////////////
			class CServo
				: virtual public Signal::Result::CBasis
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CServo() : Signal::Result::CBasis()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CServo& operator = (const CServo& object) noexcept
				{
					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CServo()
				{

				}
			};
		}

		//! �T�[�{����̊������ɔ�������֐�
		typedef void(OnServo)(std::shared_ptr<Result::CServo>);
	}
}