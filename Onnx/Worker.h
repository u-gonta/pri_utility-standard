#pragma once

#include <mutex>

#include <onnxruntime_cxx_api.h>

#include "opencv2/opencv.hpp"

#pragma comment(lib,"onnxruntime.lib")

namespace Standard
{
	namespace Onnx
	{
		//! 画像の幅
		static constexpr const int ConstImageWidth = 28;

		//! 画像の高さ
		static constexpr const int ConstImageHeight = 28;

		class CWorker
		{
		public:
			// コンストラクタ
			CWorker();

			// デストラクタ
			~CWorker();

			// モデル読み込み
			void LoadModel(const std::string& path);

			// モデルを探索
			void Search(const cv::Mat& object);

		private:
			//! 
			Ort::Env m_env;

			//! 排他制御クラス(???用)
			std::mutex m_asyncOrt;

			//!
			std::unique_ptr<Ort::Session> m_session{ nullptr };

			//!
			Ort::Value m_input_tensor{ nullptr };

			//!
			std::array<int64_t, 4> m_input_shape{};

			//!
			std::array<float, ConstImageWidth * ConstImageHeight> m_input_image{};

			//!
			Ort::Value m_output_tensor{ nullptr };

			//!
			std::array<int64_t, 2> m_output_shape{};

			//!
			std::array<float, 10> m_results{};
		};
	}
}