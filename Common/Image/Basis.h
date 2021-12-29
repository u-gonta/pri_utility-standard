#pragma once

#include "opencv2/opencv.hpp"

#include "Common/Exception/Basis.h"

namespace Standard
{
	namespace Image
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			1���C���̃A���C�����g�T�C�Y(4�o�C�g)���擾 ���C�����C���֐�
		/// @param[in]		depth	�r�b�g�[�x
		/// @param[in]		width	�摜�̕�
		/// @return         1���C���̃A���C�����g�T�C�Y(4�o�C�g)
		////////////////////////////////////////////////////////////////////////////////
		inline int Stride(int depth, int width)
		{
			int ret = 0;

			ret = ((depth / 8) * width + 3) & ~3;

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�摜�̃T�C�Y���擾 ���C�����C���֐�
		/// @param[in]		depth	�r�b�g�[�x
		/// @param[in]		height	�摜�̍���
		/// @param[in]		width	�摜�̕�
		/// @return			�摜�̃T�C�Y
		////////////////////////////////////////////////////////////////////////////////
		inline int Size(int depth, int height, int width)
		{
			int ret = 0;

			ret = Stride(depth, width) * height;

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @class      CObject
		/// @brief      �摜�̏���ێ�����N���X
		/// @detail		���t�H�[�}�b�g�֕ϊ��Ȃǂ��s��
		////////////////////////////////////////////////////////////////////////////////
		class CObject
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			CObject()
			{
				m_depth = 0;
				m_height = 0;
				m_width = 0;
				m_value = nullptr;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			/// @param[in]		depth	�r�b�g�[�x
			/// @param[in]		height	�摜�̍���
			/// @param[in]		width	�摜�̕�
			/// @param[in]		value	�摜�̃f�[�^
			////////////////////////////////////////////////////////////////////////////////
			CObject(int depth, int height, int width, const unsigned char* value) : CObject()
			{
				// �X�V
				Update(depth, height, width, value);
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R�s�[�R���X�g���N�^
			/// @param[in]		object	���N���X
			////////////////////////////////////////////////////////////////////////////////
			CObject(const CObject& object) : CObject()
			{
				// �X�V
				Update(object.m_depth, object.m_height, object.m_width, object.m_value);
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
				// �N���A
				Clear();
			}

		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�ϐ���������
			////////////////////////////////////////////////////////////////////////////////
			void Clear()
			{
				m_depth = 0;
				m_height = 0;
				m_width = 0;
				if (m_value != nullptr)
				{
					delete[] m_value;
				}
				m_value = nullptr;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���N���X���X�V
			/// @detail			�ϐ����X�V���ĉ摜�̃f�[�^���\�z����
			/// @param[in]		depth	�r�b�g�[�x
			/// @param[in]		height	�摜�̍���
			/// @param[in]		width	�摜�̕�
			/// @param[in]		value	�摜�̃f�[�^
			////////////////////////////////////////////////////////////////////////////////
			void Update(int depth, int height, int width, const unsigned char* value)
			{
				do
				{
					std::stringstream message;

					// �r�b�g�[�x���m�F
					if (depth <= 0 || depth % 8)
					{
						// �r�b�g�[�x���ُ�
						message << "�r�b�g�[�x���ُ�:" << depth;

						// ��O�𔭖C
						throw Exception::CObject(Exception::CodeImageUpdateDepth, message.str());
					}

					// �������m�F
					if (height <= 0)
					{
						// �������ُ�
						message << "�������ُ�:" << height;

						// ��O�𔭖C
						throw Exception::CObject(Exception::CodeImageUpdateHeight, message.str());
					}

					// �����m�F
					if (width <= 0)
					{
						// �����ُ�
						message << "�����ُ�:" << width;

						// ��O�𔭖C
						throw Exception::CObject(Exception::CodeImageUpdateWidth, message.str());
					}

					int size;

					size = Size(depth, height, width);

					// �f�[�^���Ȃ� or �T�C�Y���قȂ��Ă��邩���m�F
					if (m_value == nullptr || ((m_value != nullptr) && size != Size(m_depth, m_height, m_width)))
					{
						if (m_value != nullptr)
						{
							delete[] m_value;
						}
						m_value = new unsigned char[size];
					}

					this->m_depth = depth;
					this->m_height = height;
					this->m_width = width;
					::memcpy(this->m_value, value, sizeof(unsigned char) * size);
				} while (false);
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���N���X���X�V
			/// @detail			OpenCV��Mat�N���X����摜�̃f�[�^���\�z����
			/// @param[in]		object	Mat�N���X
			////////////////////////////////////////////////////////////////////////////////
			void Update(const cv::Mat& object)
			{
				do
				{
					std::stringstream message;

					if (object.empty())
					{
						// Mat�N���X����
						message << "Mat�N���X����";

						// ��O�𔭖C
						throw Exception::CObject(Exception::CodeImageUpdateMat, message.str());
					}

					cv::Size size = object.size();
					int depth = 8 * object.channels();

					// �X�V
					Update(depth, size.height, size.width, object.data);
				} while (false);
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			OpenCV��Mat�N���X���擾
			/// @detail			���N���X�̉摜�f�[�^����OpenCV��Mat�N���X���\�z���ĕԂ�
			/// @return			OpenCVC��Mat�N���X
			////////////////////////////////////////////////////////////////////////////////
			// �ϊ�
			cv::Mat Convert() const
			{
				cv::Mat ret;

				do
				{
					std::stringstream message;

					// �f�[�^���m�F
					if (m_value == nullptr)
					{
						// �摜�f�[�^����
						message << "�摜�f�[�^����";

						// ��O�𔭖C
						throw Exception::CObject(Exception::CodeImageConvertEmpty, message.str());
					}

					ret.create(m_height, m_width, CV_MAKETYPE(m_depth, (m_depth / 8)));

					::memcpy(ret.data, m_value, sizeof(unsigned char) * Size(m_depth, m_height, m_width));
				} while (false);

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�摜�t�@�C����ǂݍ���
			/// @param[in]		path	�t�@�C���p�X
			////////////////////////////////////////////////////////////////////////////////
			//void Load(const std::string& path)
			//{
			//	cv::Mat object;

			//	object = cv::imread(path, cv::IMREAD_UNCHANGED);

			//	do
			//	{
			//		std::stringstream message;

			//		if (object.empty())
			//		{
			//			// Mat�N���X����
			//			message << "Mat�N���X����";

			//			// ��O�𔭖C
			//			throw Exception::CObject(0x9012, Exception::ConstSuccess, message.str());
			//		}

			//		cv::Size size = object.size();
			//		int depth = 8 * object.channels();

			//		// �X�V
			//		Update(depth, size.height, size.width, object.data);
			//	} while (false);
			//}

		protected:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			���N���X���X�V
			/// @param[in]		object	���N���X
			////////////////////////////////////////////////////////////////////////////////
			void Update(const CObject& object)
			{
				// �X�V
				Update(object.m_depth, object.m_height, object.m_width, object.m_value);
			}

		private:
			//! �摜�̃r�b�g�[��
			int m_depth;

			//! �摜�̍���
			int m_height;

			//! �摜�̕�
			int m_width;

			//! �摜�̃f�[�^
			unsigned char* m_value;
		};
	}
}