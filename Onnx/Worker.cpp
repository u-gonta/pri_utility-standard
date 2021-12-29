#include "Worker.h"

#include "Common/Tool/Text.h"

namespace Standard
{
	namespace Onnx
	{
////////////////////////////////////////////////////////////////////////////////
		/// @brief			�R���X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CWorker::CWorker()
		{
			m_session.release();
			m_input_shape.at(0) = 1;
			m_input_shape.at(1) = 1;
			m_input_shape.at(2) = ConstImageWidth;
			m_input_shape.at(3) = ConstImageHeight;
			m_output_shape.at(0) = 1;
			m_output_shape.at(1) = 10;

			auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

			m_input_tensor = Ort::Value::CreateTensor<float>(memory_info, m_input_image.data(), m_input_image.size(), m_input_shape.data(), m_input_shape.size());
			m_output_tensor = Ort::Value::CreateTensor<float>(memory_info, m_results.data(), m_results.size(), m_output_shape.data(), m_output_shape.size());
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�f�X�g���N�^
		////////////////////////////////////////////////////////////////////////////////
		CWorker::~CWorker()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���f���ǂݍ���
		/// @param[in]		���f���̃t�@�C��
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::LoadModel(const std::string& path)
		{
			// �r������
			std::unique_lock<std::mutex> lock(m_asyncOrt);

			m_session.release();

			m_session = std::make_unique<Ort::Session>(m_env, Text::Convert(path).c_str(), Ort::SessionOptions{nullptr});


		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			���f����T��
		/// @param[in]		�摜�̏���ێ�����N���X
		////////////////////////////////////////////////////////////////////////////////
		void CWorker::Search(const cv::Mat& object)
		{
			// �r������
			std::unique_lock<std::mutex> lock(m_asyncOrt);

			if (m_session == nullptr)
			{
				throw std::exception("Not loaded");
			}

			const char* input_names[] = { "Input3" };
			const char* output_names[] = { "Plus214_Output_0" };

			m_session->Run(Ort::RunOptions{ nullptr }, input_names, &m_input_tensor, 1, output_names, &m_output_tensor, 1);

			int64_t result = std::distance(m_results.begin(), std::max_element(m_results.begin(), m_results.end()));
		}
	}
}