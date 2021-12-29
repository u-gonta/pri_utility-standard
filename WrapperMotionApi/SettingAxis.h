#pragma once

#include "Identifier.h"

// 1�o�C�g�A���C�������g�ɕύX
#pragma pack(1)

namespace Standard
{
	namespace MotionApi
	{
		namespace Axis
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @enum			���̃^�C�v
			////////////////////////////////////////////////////////////////////////////////
			enum EnumAxis
			{
				AxisReal = 1,		// ���T�[�{��
				AxisVirtual = 2,	// ���z�T�[�{��
				AxisExternal = 3	// �O���G���R�[�_
			};

			namespace Setting
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CDeclare
				/// @brief      ���̃n���h�����쐬����ݒ�N���X
				///				��MP3100��SVC(����)�F�X���b�g=0,�T�u�X���b�g=3
				///				��MP3100��SVR(���z)�F�X���b�g=0,�T�u�X���b�g=4
				////////////////////////////////////////////////////////////////////////////////
				class CDeclare
				{
				private:
					//! �V���A���C�Y�̃^�C�g��
					const std::string ConstHeader = "Declare";

				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CDeclare()
					{
						SlotNo = 0;
						SubSlotNo = 3;
						StationNo = 0;
						LogicalNo = 0;
						Type = AxisReal;
						Address = 0;
						Resolution = 1;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CDeclare& operator = (const CDeclare& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CDeclare()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�V���A���C�Y
					////////////////////////////////////////////////////////////////////////////////
					friend class cereal::access;
					template<class T>
					void serialize(T& archive)
					{
						archive(cereal::make_nvp("SlotNo", SlotNo));
						archive(cereal::make_nvp("SubSlotNo", SubSlotNo));
						archive(cereal::make_nvp("StationNo", StationNo));
						archive(cereal::make_nvp("LogicalNo", LogicalNo));
						archive(cereal::make_nvp("Axis", Type));
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			Json�`���ɃV���A���C�Y���������񂩂�X�V
					/// @param[in]		object	Json�`���ɃV���A���C�Y����������
					////////////////////////////////////////////////////////////////////////////////
					void Input(std::string object)
					{
						// Json�`������ϊ�
						Update(Archive::Input<CDeclare>(ConstHeader, object));
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			Json�`���ɃV���A���C�Y������������擾
					/// @return			�V���A���C�Y����������
					////////////////////////////////////////////////////////////////////////////////
					std::string Output()
					{
						std::string ret;

						// Json�`���ɕϊ�
						ret = Archive::Output<CDeclare>(ConstHeader, *this);

						return ret;
					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CDeclare& object)
					{
						this->SlotNo = object.SlotNo;
						this->SubSlotNo = object.SubSlotNo;
						this->StationNo = object.StationNo;
						this->LogicalNo = object.LogicalNo;
						this->Type = object.Type;
						this->Address = object.Address;
						this->Resolution = object.Resolution;
					}

				public:
					//! �X���b�g�ԍ�
					unsigned short SlotNo;

					//! �T�u�X���b�g�ԍ�
					unsigned short SubSlotNo;

					//! ������`����Ă���X�e�[�V�����ԍ�
					unsigned short StationNo;

					//! ���̘_���ԍ�
					unsigned short LogicalNo;

					//! ���̃^�C�v
					EnumAxis Type;

					//! ���W�X�^�̐擪�A�h���X
					unsigned short Address;

					//! �ʒu�̕���\
					double Resolution;
				};

				////////////////////////////////////////////////////////////////////////////////
				/// @class      CClear
				/// @brief      ���̃n���h����j������ݒ�N���X
				///				�� ���ʎq��ێ�����N���X����h��
				////////////////////////////////////////////////////////////////////////////////
				class CClear
					: virtual public CIdentifier
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R���X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					CClear()
					{

					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�R�s�[������Z�q
					/// @return			���N���X
					////////////////////////////////////////////////////////////////////////////////
					CClear& operator = (const CClear& object) noexcept
					{
						// �X�V
						Update(object);

						return *this;
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			�f�X�g���N�^
					////////////////////////////////////////////////////////////////////////////////
					~CClear()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			���N���X���X�V
					/// @param[in]		object	���N���X
					////////////////////////////////////////////////////////////////////////////////
					void Update(const CClear& object)
					{
						// ����̊֐�
						CIdentifier::Update(object);
					}
				};
			}
		}
	}
}

// �o�C�g�A���C�������g��߂�
#pragma pack()