#pragma once

#include "opencv2/opencv.hpp"

#include "Common/Exception/Basis.h"

namespace Standard
{
	namespace Image
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			1ラインのアライメントサイズ(4バイト)を取得 ※インライン関数
		/// @param[in]		depth	ビット深度
		/// @param[in]		width	画像の幅
		/// @return         1ラインのアライメントサイズ(4バイト)
		////////////////////////////////////////////////////////////////////////////////
		inline int Stride(int depth, int width)
		{
			int ret = 0;

			ret = ((depth / 8) * width + 3) & ~3;

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			画像のサイズを取得 ※インライン関数
		/// @param[in]		depth	ビット深度
		/// @param[in]		height	画像の高さ
		/// @param[in]		width	画像の幅
		/// @return			画像のサイズ
		////////////////////////////////////////////////////////////////////////////////
		inline int Size(int depth, int height, int width)
		{
			int ret = 0;

			ret = Stride(depth, width) * height;

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @class      CObject
		/// @brief      画像の情報を保持するクラス
		/// @detail		他フォーマットへ変換なども行う
		////////////////////////////////////////////////////////////////////////////////
		class CObject
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CObject()
			{
				m_depth = 0;
				m_height = 0;
				m_width = 0;
				m_value = nullptr;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			/// @param[in]		depth	ビット深度
			/// @param[in]		height	画像の高さ
			/// @param[in]		width	画像の幅
			/// @param[in]		value	画像のデータ
			////////////////////////////////////////////////////////////////////////////////
			CObject(int depth, int height, int width, const unsigned char* value) : CObject()
			{
				// 更新
				Update(depth, height, width, value);
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コピーコンストラクタ
			/// @param[in]		object	自クラス
			////////////////////////////////////////////////////////////////////////////////
			CObject(const CObject& object) : CObject()
			{
				// 更新
				Update(object.m_depth, object.m_height, object.m_width, object.m_value);
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コピー代入演算子
			/// @return			自クラス
			////////////////////////////////////////////////////////////////////////////////
			CObject& operator = (const CObject& object) noexcept
			{
				// 更新
				Update(object);

				return *this;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デストラクタ
			////////////////////////////////////////////////////////////////////////////////
			~CObject()
			{
				// クリア
				Clear();
			}

		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			変数を初期化
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
			/// @brief			自クラスを更新
			/// @detail			変数を更新して画像のデータを構築する
			/// @param[in]		depth	ビット深度
			/// @param[in]		height	画像の高さ
			/// @param[in]		width	画像の幅
			/// @param[in]		value	画像のデータ
			////////////////////////////////////////////////////////////////////////////////
			void Update(int depth, int height, int width, const unsigned char* value)
			{
				do
				{
					std::stringstream message;

					// ビット深度を確認
					if (depth <= 0 || depth % 8)
					{
						// ビット深度が異常
						message << "ビット深度が異常:" << depth;

						// 例外を発砲
						throw Exception::CObject(Exception::CodeImageUpdateDepth, message.str());
					}

					// 高さを確認
					if (height <= 0)
					{
						// 高さが異常
						message << "高さが異常:" << height;

						// 例外を発砲
						throw Exception::CObject(Exception::CodeImageUpdateHeight, message.str());
					}

					// 幅を確認
					if (width <= 0)
					{
						// 幅が異常
						message << "幅が異常:" << width;

						// 例外を発砲
						throw Exception::CObject(Exception::CodeImageUpdateWidth, message.str());
					}

					int size;

					size = Size(depth, height, width);

					// データがない or サイズが異なっているかを確認
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
			/// @brief			自クラスを更新
			/// @detail			OpenCVのMatクラスから画像のデータを構築する
			/// @param[in]		object	Matクラス
			////////////////////////////////////////////////////////////////////////////////
			void Update(const cv::Mat& object)
			{
				do
				{
					std::stringstream message;

					if (object.empty())
					{
						// Matクラスが空
						message << "Matクラスが空";

						// 例外を発砲
						throw Exception::CObject(Exception::CodeImageUpdateMat, message.str());
					}

					cv::Size size = object.size();
					int depth = 8 * object.channels();

					// 更新
					Update(depth, size.height, size.width, object.data);
				} while (false);
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			OpenCVのMatクラスを取得
			/// @detail			自クラスの画像データからOpenCVのMatクラスを構築して返す
			/// @return			OpenCVCのMatクラス
			////////////////////////////////////////////////////////////////////////////////
			// 変換
			cv::Mat Convert() const
			{
				cv::Mat ret;

				do
				{
					std::stringstream message;

					// データを確認
					if (m_value == nullptr)
					{
						// 画像データが空
						message << "画像データが空";

						// 例外を発砲
						throw Exception::CObject(Exception::CodeImageConvertEmpty, message.str());
					}

					ret.create(m_height, m_width, CV_MAKETYPE(m_depth, (m_depth / 8)));

					::memcpy(ret.data, m_value, sizeof(unsigned char) * Size(m_depth, m_height, m_width));
				} while (false);

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			画像ファイルを読み込む
			/// @param[in]		path	ファイルパス
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
			//			// Matクラスが空
			//			message << "Matクラスが空";

			//			// 例外を発砲
			//			throw Exception::CObject(0x9012, Exception::ConstSuccess, message.str());
			//		}

			//		cv::Size size = object.size();
			//		int depth = 8 * object.channels();

			//		// 更新
			//		Update(depth, size.height, size.width, object.data);
			//	} while (false);
			//}

		protected:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			自クラスを更新
			/// @param[in]		object	自クラス
			////////////////////////////////////////////////////////////////////////////////
			void Update(const CObject& object)
			{
				// 更新
				Update(object.m_depth, object.m_height, object.m_width, object.m_value);
			}

		private:
			//! 画像のビット深さ
			int m_depth;

			//! 画像の高さ
			int m_height;

			//! 画像の幅
			int m_width;

			//! 画像のデータ
			unsigned char* m_value;
		};
	}
}