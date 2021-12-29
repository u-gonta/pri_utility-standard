#pragma once

#include "Axis/Poling.h"

namespace Standard
{
	namespace Axis
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      軸の制御、軸の監視、状態を通知するクラス
			///				⇒ 軸の制御、軸の監視、状態を通知するテンプレートのクラスから継承
			/// @detail		軸の制御、軸の監視、状態の通知を行う
			////////////////////////////////////////////////////////////////////////////////
			class CWorker
				: virtual public Worker::CTemplate
			{
			public:
				// コンストラクタ
				CWorker();

				// デストラクタ
				~CWorker() override;

			protected:
				// 初期化
				void InitializeDerived() override;

				// 破棄
				void DestroyDerived() override;

				// 読み込み
				void ReadDerived() override;

				// 書き込み
				void WriteDerived() override;

				// 完了を通知
				template <typename CWrite, typename CArgument>
				void NotifyComplete(std::shared_ptr<CArgument> object, bool remove = true);
			};
		}
	}
}