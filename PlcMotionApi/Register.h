#pragma once

#include <map>

#include "Common/Io/Setting.h"

namespace Standard
{
	namespace Plc
	{
		namespace Register
		{
			//! S���W�X�^
			const char ConstCategoryS = 'S';

			//! M���W�X�^
			const char ConstCategoryM = 'M';

			//! I���W�X�^
			const char ConstCategoryI = 'I';

			//! O���W�X�^
			const char ConstCategoryO = 'O';

			//! C���W�X�^
			const char ConstCategoryC = 'C';

			//! �r�b�g�^
			const char ConstTypeBit = 'B';

			//! ���[�h�^
			const char ConstTypeWord = 'W';

			//! �����O�^
			const char ConstTypeLong = 'L';

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CSpecify
			/// @brief      ���W�X�^�̎w��f�[�^��ێ�����N���X
			////////////////////////////////////////////////////////////////////////////////
			class CSpecify
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				CSpecify()
				{
					_category = "";
					Index = 0;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R���X�g���N�^
				/// @param[in]		���
				////////////////////////////////////////////////////////////////////////////////
				CSpecify(std::string category) : CSpecify()
				{
					std::stringstream postscript;

					postscript << "���:" << category;

					// ��ʂ��m�F
					if (category.size() < 1)
					{
						// ��ʂ����w��
						postscript << "�����w��";
						throw std::exception(postscript.str().c_str());
					}

					// ��ʂ��X�V
					_category = category;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�R�s�[������Z�q
				/// @return			���N���X
				////////////////////////////////////////////////////////////////////////////////
				CSpecify& operator = (const CSpecify& object) noexcept
				{
					// �X�V
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�X�g���N�^
				////////////////////////////////////////////////////////////////////////////////
				~CSpecify()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�[�^�̃^�C�v��ύX
				/// @param[in]		type �f�[�^�̃^�C�v
				////////////////////////////////////////////////////////////////////////////////
				void ChangeType(const char& type)
				{
					std::stringstream postscript;
					std::stringstream category;

					postscript << "���:" << _category;

					// ��ʂ��擾
					category << GetCategory();
					// �f�[�^�̃^�C�v���m�F
					if (ConstTypeBit == GetType())
					{
						// �r�b�g�^ �� �f�[�^�̃^�C�v���m�F
						if (ConstTypeBit != type)
						{
							// ���[�h�^ or �����O�^ �� �r�b�g�̌����폜
							Index >>= 4;
						}
					}
					else
					{
						// ���[�h�^ or �����O�^ �� �f�[�^�̃^�C�v���m�F
						if (ConstTypeBit == type)
						{
							// �r�b�g�^ �� �r�b�g�̌���ǉ�
							Index <<= 4;
						}
					}
					// �f�[�^�̃^�C�v���X�V
					category << type;

					// ��ʂ��X�V
					_category = category.str();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			��ʂ��擾
				/// @return			���
				////////////////////////////////////////////////////////////////////////////////
				char GetCategory()
				{
					char ret = ConstCategoryM;

					std::stringstream postscript;

					postscript << "���:" << _category;

					// ��ʂ��m�F
					if (_category.size() < 1)
					{
						// ��ʂ����w��
						postscript << "�����w��";
						throw std::exception(postscript.str().c_str());
					}

					// ��ʂ��擾
					ret = _category.at(0);

					switch (ret)
					{
					case ConstCategoryS:
					case ConstCategoryM:
					case ConstCategoryI:
					case ConstCategoryO:
					case ConstCategoryC:
						// S�AM�AI�AO�AC���W�X�^
						break;

					default:
						// ���̑�
						postscript << "���s�K��";
						throw std::exception(postscript.str().c_str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�[�^�̃^�C�v���擾
				/// @return			�^�C�v
				////////////////////////////////////////////////////////////////////////////////
				char GetType()
				{
					char ret = ConstTypeBit;

					std::stringstream postscript;

					postscript << "���:" << _category;

					// ��ʂ��m�F
					if (_category.size() < 2)
					{
						// �f�[�^�^�C�v�����w��
						postscript << "�Ńf�[�^�̃^�C�v�����w��";
						throw std::exception(postscript.str().c_str());
					}

					// �f�[�^�̃^�C�v���擾
					ret = _category.at(1);

					switch (ret)
					{
					case ConstTypeBit:
					case ConstTypeWord:
					case ConstTypeLong:
						// �r�b�g�A���[�h�A�����O�^
						break;

					default:
						// ���̑�
						postscript << "�Ńf�[�^�̃^�C�v���s�K��";
						throw std::exception(postscript.str().c_str());
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�f�[�^�̌������擾
				///					���r�b�g�̌��͏Ȃ�
				/// @return			�f�[�^�̌���
				////////////////////////////////////////////////////////////////////////////////
				int GetMargin()
				{
					int ret = 5;

					// ���W�X�^�̎�ʂ��m�F
					if (ConstCategoryI == GetCategory() || ConstCategoryO == GetCategory())
					{
						// I�AO���W�X�^ �� �f�[�^�^�C�v�ȍ~��4���������W�X�^�ԍ��Ƃ��Ď擾
						ret = 4;
					}

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			�A�h���X���擾
				/// @return			�A�h���X
				////////////////////////////////////////////////////////////////////////////////
				std::string GetAddress()
				{
					std::stringstream ret;
					int margin;

					// ��ʂ��擾
					ret << GetCategory();
					// �f�[�^�̃^�C�v���擾
					ret << GetType();
					// �f�[�^�̌������擾
					margin = GetMargin();
					// �f�[�^�̃^�C�v���m�F
					if (ConstTypeBit == GetType())
					{
						// �r�b�g�^ �� �r�b�g�̌���ǉ�
						margin += 1;
					}
					// �ԍ����m��
					ret << Text::FormatX(margin, Index, "");

					return ret.str();
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			���N���X���X�V
				/// @param[in]		object	���N���X
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CSpecify& object)
				{
					this->_category = object._category;
					this->Index = object.Index;
				}

			private:
				//! ���
				std::string _category;

			public:
				//! �ԍ�
				unsigned long Index;
			};
		}
	}
}