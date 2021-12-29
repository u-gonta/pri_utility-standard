#pragma once

#include <vector>

#include "boost/any.hpp"

namespace Standard
{
	namespace Variant
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CObject
		/// @brief      �C�ӂ̗v�f�Ƃ���ɕR�Â��l��ێ�����N���X
		////////////////////////////////////////////////////////////////////////////////
		class CObject
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			CObject()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			/// @param[in]		TElement	�v�f�̌^(�e���v���[�g�Ŏw��)
			/// @param[in]		TValue	�l�̌^(�e���v���[�g�Ŏw��)
			/// @param[in]		element	�v�f
			/// @param[in]		value	�l
			////////////////////////////////////////////////////////////////////////////////
			template<typename TElement, typename TValue>
			CObject(const TElement& element, const TValue& value)
			{
				this->Element = element;
				this->Value = value;
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
				this->Element = object.Element;
				this->Value = object.Value;
			}

		public:
			//! �v�f
			boost::any Element;

			//! �l
			boost::any Value;
		};

		////////////////////////////////////////////////////////////////////////////////
		/// @class      CManager
		/// @brief      �C�ӂ̗v�f�Ƃ���ɕR�Â��l��ێ�����N���X���Ǘ�����N���X
		/// @detail		�C�ӂ̌^�֕ϊ��Ȃǂ��s��
		////////////////////////////////////////////////////////////////////////////////
		class CManager
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			CManager()
			{
				Objects.clear();
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R�s�[������Z�q
			/// @return			���N���X
			////////////////////////////////////////////////////////////////////////////////
			CManager& operator = (const CManager& object) noexcept
			{
				// �X�V
				Update(object);

				return *this;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�f�X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			~CManager()
			{

			}

		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�v�f����v�����l��Ԃ�
			/// @detail			�e���v���[�g�ŗv�f�̌^���w��
			/// @param[in]		TElement	�v�f�̌^(�e���v���[�g�Ŏw��)
			/// @param[in]		element	�v�f
			/// @return			�v�f����v�����l ���s��v�̏ꍇ��empty��ԂɂȂ��Ă���
			////////////////////////////////////////////////////////////////////////////////
			template <typename TElement>
			boost::any GetValue(const boost::any& element)
			{
				boost::any ret;

				// ���𑖍�
				for (const auto& object : Objects)
				{
					// �v�f�̈�v���m�F
					if (IsMatch<TElement>(element, object.Element) == false)
					{
						// �s��v
						continue;
					}

					// ��v
					ret = object.Value;
					break;
				}

				return ret;
			}

		protected:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���N���X���X�V
			/// @param[in]		objects	���N���X
			////////////////////////////////////////////////////////////////////////////////
			void Update(const CManager& object)
			{
				this->Objects.clear();
				std::copy(object.Objects.begin(), object.Objects.end(), std::back_inserter(this->Objects));
			}

		private:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�v�f�̈�v���m�F
			/// @detail			�e���v���[�g�ŗv�f�̌^���w��
			/// @param[in]		TElement	�v�f�̌^(�e���v���[�g�Ŏw��)
			/// @param[in]		first	��r���̗v�f
			/// @param[in]		second	��r��̗v�f
			/// @return			true:��v / false:�s��v
			////////////////////////////////////////////////////////////////////////////////
			template <typename TElement>
			bool IsMatch(const boost::any& first, const boost::any& second)
			{
				bool ret = false;

				do
				{
					// �v�f�̋���m�F
					if (first.empty() | second.empty())
					{
						// ��
						break;
					}

					// �v�f�̌^���m�F
					if (first.type() != second.type())
					{
						// �s��v
						break;
					}

					// �v�f�̒l���r
					if (boost::any_cast<TElement>(first) != boost::any_cast<TElement>(second))
					{
						// �s��v
						break;
					}

					// ��v
					ret = true;
				} while (false);

				return ret;
			}

		public:
			//! �C�ӂ̗v�f�Ƃ���ɕR�Â��l��ێ�����N���X
			std::vector<Variant::CObject> Objects;
		};
	}
}