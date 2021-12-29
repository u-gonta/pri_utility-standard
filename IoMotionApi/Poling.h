#pragma once

#include "Io/Poling.h"

#include "WrapperMotionApi/Result.h"

namespace Standard
{
	namespace Io
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      IOの監視、IOの制御、状態を通知するクラス
			///				⇒ IOの制御、IOの監視、状態を通知するテンプレートのクラスから継承
			/// @detail		IOの監視、IOの制御、状態の通知を行う
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

				// リトライを確認
				bool IsRetry() override;

				// 読み込み
				void ReadDerived() override;

				// 書き込み
				void WriteDerived() override;

			public:
				// 接続の状態を取得
				bool GetConnected();

			private:
				// 接続済みを確認
				void IsConnected();

			private:
				//! 排他制御クラス(コントローラを開いた結果クラス用)
				std::recursive_mutex m_asyncController;

				//! コントローラを開いた結果クラス
				MotionApi::Controller::Result::COpen m_controller;
			};
		}
	}
}
