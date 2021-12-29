#include "Onnx.h"

#include "Common/Tool/Text.h"
#include "Worker.h"

namespace Standard
{
	namespace Onnx
	{
		static CWorker _worker;

		ONNX_API void Initialize()
		{
			_worker.LoadModel("D:\\model.onnx");
		}

		ONNX_API void Test(const std::string& path)
		{
			//cv::Mat image;

			//image = cv::imread(path);
			//
			//_worker.Search(image);
		}
	}
}