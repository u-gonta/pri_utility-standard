#pragma once

#include "Common/Logging/Basis.h"

namespace Standard
{
	namespace Transfer
	{
		namespace Setting
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CObject
			/// @brief      �]�����s���ݒ�N���X
			////////////////////////////////////////////////////////////////////////////////
			class CObject
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CObject()
				{
					IpAddress = "127.0.0.1";
					Port = 50001;
					Lower = Logging::LevelInfomation;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CObject& operator = (const CObject& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CObject()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CObject& object)
				{
					this->IpAddress = object.IpAddress;
					this->Port = object.Port;
					this->Lower = object.Lower;
				}

			public:
				//! �]�����IP�A�h���X
				std::string IpAddress;

				//! �]����̃|�[�g
				unsigned short Port;

				//! �o�͂��鉺�����x��
				Logging::EnumLevel Lower;
			};
		}
	}
}