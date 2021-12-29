#pragma once

#ifdef ONNX_EXPORTS
#define ONNX_API __declspec(dllexport)
#else
#define ONNX_API __declspec(dllimport)
#endif

#include <string>

namespace Standard
{
	namespace Onnx
	{
		ONNX_API void Initialize();

		ONNX_API void Test(const std::string& path);
	}
}