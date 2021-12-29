#pragma once

#include "Common/Plc/Setting.h"
#include "Device/Worker.h"

namespace Standard
{
	namespace Plc
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      Plcの制御を行うテンプレートのクラス
			///				⇒ デバイスの制御を行うテンプレートのクラスから継承
			/// @param[in]	CDefault	デフォルトの設定クラスを指定
			/// @param[in]	CConnect	接続の設定クラスを指定
			////////////////////////////////////////////////////////////////////////////////
			class CTemplate
				: virtual public Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>()
				{
					// 名称を更新
					SetName("Plc::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化済みの例外を発砲
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionInitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionInitialized()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodePlcInitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄済みの例外を発砲
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionDiscarded() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionDiscarded()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodePlcDiscarded, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			未初期化の例外を発砲
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionUninitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionUninitialized()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodePlcUninitialized, logging, postscript.str());
				}
			};
		}
	}
}