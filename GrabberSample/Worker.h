#pragma once

#include "Grabber/Worker.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      ゲインや露光時間の制御、撮像を行うクラス
		///				⇒ カメラの制御を行うテンプレートのクラスから派生
		/// @detail		ゲインや露光時間の制御、撮像を行う
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Worker::CTemplate
		{
		public:
			// コンストラクタ
			CWorker();

			// デストラクタ
			~CWorker();

		public:
			// アナログゲインを読み込み
			double ReadAnalogGainDerived() override;

			// アナログゲインを書き込み
			void WriteAnalogGainDerived(double value) override;

			// デジタルゲインを読み込み
			double ReadDigitalGainDerived() override;

			// デジタルゲインを書き込み
			void WriteDigitalGainDerived(double value) override;

			// 露光時間を読み込み
			double ReadExposureTimeDerived() override;

			// 露光時間を書き込み
			void WriteExposureTimeDerived(double value) override;

			// 取り込み
			Image::CObject CaptureDerived() override;

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
			// 接続の状態を更新
			void SetConnected(bool value);

		private:
			//! 排他制御クラス(ダミーのデータ用)
			std::recursive_mutex m_asyncDmy;

			//! 接続の状態フラグ
			bool m_connected;

			//! アナログゲインの値
			double m_analogGain;

			//! デジタルゲインの値
			double m_digitalGain;

			//! 露光時間の値
			double m_exposureTime;

			//! 画像
			Image::CObject m_image;
		};
	}
}
