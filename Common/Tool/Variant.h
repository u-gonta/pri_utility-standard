#pragma once

#include "boost/any.hpp"

#include "Common/Logging/Basis.h"

namespace Standard
{
	namespace Variant
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�l�̌^����v���Ă��邩�m�F
		/// @param[in]		TValue	�l�̌^(�e���v���[�g�Ŏw��)
		/// @param[in]		value	�l
		/// @return			true:��v / false:�s��v
		////////////////////////////////////////////////////////////////////////////////
		template <typename TValue>
		inline bool IsValue(const boost::any& value)
		{
			bool ret = false;

			do
			{
				// �l�̋���m�F
				if (value.empty())
				{
					// ��
					break;
				}

				// �^���m�F
				if (value.type() != typeid(TValue))
				{
					// �s��v
					break;
				}

				// ��v
				ret = true;
			} while (false);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			int�^����v���Ă��邩�m�F
		/// @param[in]		value	�l
		/// @return			true:��v / false:�s��v
		////////////////////////////////////////////////////////////////////////////////
		inline bool IsIntValue(const boost::any& value)
		{
			return IsValue<int>(value);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�l��ϊ�
		/// @param[in]		TValue	�l�̌^(�e���v���[�g�Ŏw��)
		/// @param[in]		value	�l
		/// @return			�ϊ������l
		////////////////////////////////////////////////////////////////////////////////
		template <typename TValue>
		inline TValue Convert(const boost::any& value)
		{
			TValue ret;

			// �ϊ�
			ret = boost::any_cast<TValue>(value);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�l��int�^�֕ϊ�
		/// @param[in]		value	�l
		/// @return			int�^�ɕϊ����ꂽ�l
		////////////////////////////////////////////////////////////////////////////////
		inline int ConvertInt(const boost::any& value)
		{
			return Convert<int>(value);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�l�̌^����v���Ă��邩�m�F
		/// @param[in]		source	��r��
		/// @param[in]		destination	��r��
		/// @return			true:��v / false:�s��v
		////////////////////////////////////////////////////////////////////////////////
		inline bool Equal(const boost::any& source, const boost::any& destination)
		{
			bool ret = false;

			do
			{
				// �l�̋���m�F
				if (source.empty() || destination.empty())
				{
					// ��
					std::stringstream message;
					message << "�f�[�^����";
					message << Logging::ConstSeparator << "��r��:" << (source.empty() ? "" : source.type().name());
					message << Logging::ConstSeparator << "��r��:" << (destination.empty() ? "" : destination.type().name());
					throw std::exception(message.str().c_str());
				}

				// �l�̌^���m�F
				if (source.type() != destination.type())
				{
					// �^���Ⴄ
					std::stringstream message;
					message << "�f�[�^�̌^���s��v";
					message << Logging::ConstSeparator << "��r��:" << source.type().name();
					message << Logging::ConstSeparator << "��r��:" << destination.type().name();
					throw std::exception(message.str().c_str());
				}

				// �^���m�F
				if (source.type() == typeid(bool))
				{
					// �r�b�g�^ �� �l���r
					if (Convert<bool>(source) != Convert<bool>(destination))
					{
						// �s��v
						break;
					}
				}
				else if (source.type() == typeid(__int16))
				{
					// ����(2�o�C�g)�^ �� �l���r
					if (Convert<__int16>(source) != Convert<__int16>(destination))
					{
						// �s��v
						break;
					}
				}
				else if (source.type() == typeid(__int32))
				{
					// ����(4�o�C�g)�^ �� �l���r
					if (Convert<__int32>(source) != Convert<__int32>(destination))
					{
						// �s��v
						break;
					}
				}
				else if (source.type() == typeid(__int64))
				{
					// ����(8�o�C�g)�^ �� �l���r
					if (Convert<__int64>(source) != Convert<__int64>(destination))
					{
						// �s��v
						break;
					}
				}
				else if (source.type() == typeid(float))
				{
					// �P���x���������_�����^ �� �l���r
					if (Convert<float>(source) != Convert<float>(destination))
					{
						// �s��v
						break;
					}
				}
				else if (source.type() == typeid(double))
				{
					// �{���x���������_�����^ �� �l���r
					if (Convert<double>(source) != Convert<double>(destination))
					{
						// �s��v
						break;
					}
				}

				// ��v
				ret = true;
			} while (false);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�l�̌^�𕶎���֕ϊ�
		/// @param[in]		value	�l
		/// @return			������ɕϊ����ꂽ�l�̌^
		////////////////////////////////////////////////////////////////////////////////
		inline std::string FormatType(const boost::any& value)
		{
			std::stringstream ret;

			// �^���m�F
			if (value.type() == typeid(bool))
			{
				ret << "�r�b�g";
			}
			else if (value.type() == typeid(__int16))
			{
				ret << "����(2�o�C�g)";
			}
			else if (value.type() == typeid(__int32))
			{
				ret << "����(4�o�C�g)";
			}
			else if (value.type() == typeid(__int64))
			{
				ret << "����(8�o�C�g)";
			}
			else if (value.type() == typeid(float))
			{
				ret << "�P���x���������_����";
			}
			else if (value.type() == typeid(double))
			{
				ret << "�{���x���������_����";
			}
			else
			{
				ret << "���Ή�(" << value.type().name() << ")";
			}

			return ret.str();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�l�𕶎���֕ϊ�
		/// @param[in]		value	�l
		/// @return			������ɕϊ����ꂽ�l
		////////////////////////////////////////////////////////////////////////////////
		inline std::string FormatValue(const boost::any& value)
		{
			std::stringstream ret;

			// �^���m�F
			if (value.type() == typeid(bool))
			{
				// �r�b�g
				ret << Variant::Convert<bool>(value);
			}
			else if (value.type() == typeid(__int16))
			{
				// ����(2�o�C�g)
				ret << Variant::Convert<__int16>(value);
			}
			else if (value.type() == typeid(__int32))
			{
				// ����(4�o�C�g)
				ret << Variant::Convert<__int32>(value);
			}
			else if (value.type() == typeid(__int64))
			{
				// ����(8�o�C�g)
				ret << Variant::Convert<__int64>(value);
			}
			else if (value.type() == typeid(float))
			{
				// �P���x���������_����
				ret << Variant::Convert<float>(value);
			}
			else if (value.type() == typeid(double))
			{
				// �{���x���������_����
				ret << Variant::Convert<double>(value);
			}
			else
			{
				ret << "���Ή�(" << value.type().name() << ")";
				throw std::exception(ret.str().c_str());
			}

			return ret.str();
		}
	}
}