#pragma once

#include "Io/Worker.h"
#include "Poling.h"

namespace Standard
{
	namespace Io
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      IOの監視、IOの制御、状態を通知するクラス
		///				⇒ IOの監視、IOの制御、状態を通知するテンプレートのクラスから派生
		/// @detail		IOの監視、IOの制御、状態の通知を行う
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Worker::CTemplate<Poling::CWorker>
		{
		public:
			// コンストラクタ
			CWorker();

			// デストラクタ
			~CWorker() override;
		};
	}
}

