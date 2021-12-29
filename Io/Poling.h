#pragma once

#include "Common/Io/Status.h"
#include "Signal/WorkerExpand.h"
#include "PolingSetting.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
		{
			namespace Worker
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CTemplate
				/// @brief      IOの制御、IOの監視、状態を通知するテンプレートのクラス
				///				⇒ 状態を監視、書き込み、通知するテンプレートを拡張したクラスから派生
				////////////////////////////////////////////////////////////////////////////////
				class CTemplate
					: virtual public Signal::Worker::CTemplateExpand<CSetting, Status::CInput, Status::COutput>
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コンストラクタ
					////////////////////////////////////////////////////////////////////////////////
					CTemplate() : Signal::Worker::CTemplateExpand<CSetting, Status::CInput, Status::COutput>()
					{
						std::string name;

						// 名称を更新
						name = "Io::Poling::";
						SetName(name);
						SetNameException(name + "Exception::");
						SetNameArgument(name + "Notice::");
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			デストラクタ
					////////////////////////////////////////////////////////////////////////////////
					virtual ~CTemplate()
					{

					}
				};
			}
		}
	}
}