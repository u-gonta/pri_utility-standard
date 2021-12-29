#pragma once

#include "Axis/Worker.h"
#include "Poling.h"

namespace Standard
{
	namespace Axis
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      軸の制御を行うクラス
		///				⇒ 軸の監視、軸の制御、状態を通知するテンプレートのクラスから派生
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Axis::Worker::CTemplate<Poling::CWorker>
		{
		public:
			// コンストラクタ
			CWorker();

			// デストラクタ
			~CWorker() override;
		};
	}
}

