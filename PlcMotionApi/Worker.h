#pragma once

#include "Plc/Worker.h"

namespace Standard
{
	namespace Plc
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      PLCの制御を行うクラス
		///				⇒ PLCの制御を行うテンプレートのクラスから継承
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

			// 接続
			void ConnectDerived() override;

			// 切断
			void DisconnectDerived() override;

			// 接続の状態を取得
			bool GetConnected() override;

		private:
			//! 排他制御クラス(ライブラリ用)
			std::recursive_mutex m_asyncLibrary;
		};
	}
}
