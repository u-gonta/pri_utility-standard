#pragma once

#include "Io/Poling.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      IOの監視、IOの制御、状態を通知するクラス
			///				⇒ IOの制御、IOの監視、状態を通知するテンプレートのクラスから派生
			/// @detail		IOの監視、IOの制御、状態の通知を行う
			////////////////////////////////////////////////////////////////////////////////
			class CWorker
				: virtual public Worker::CTemplate
			{
			public:
				// コンストラクタ
				CWorker();

				// デストラクタ
				~CWorker();

			protected:
				// 初期化
				void InitializeDerived() override;

				// 破棄
				void DestroyDerived() override;

				// 読み込み
				void ReadDerived() override;

				// 書き込み
				void WriteDerived() override;
			};
		}
	}
}