#pragma once

#include "Grabber/Worker.h"
#include "Parameter.h"

namespace Standard
{
	namespace Grabber
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CWorker
		/// @brief      カメラの制御を行うクラス
		///				⇒ カメラの制御を行うテンプレートのクラスから継承
		////////////////////////////////////////////////////////////////////////////////
		class CWorker
			: virtual public Worker::CTemplate
		{
		public:
			// コンストラクタ
			CWorker();

			// デストラクタ
			~CWorker() override;

		public:
			// コールバックから呼び出し関数
			void ReceiveNode(GenApi::INode* node);

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

		private:
			// デバイスロストを確認
			bool IsDeviceLost();

			// デバイスを確認
			bool IsDevice(const Setting::CConnect& setting, const GenApi::INodeMap& nodeMap, const uint32_t index);

			// パラメータをデフォルトに戻す
			void DefaultParameter(const GenApi::INodeMap& nodeMap, const Setting::CDefault& setting) throw(...);

			// HeartBeatを更新
			void UpdateHeartbeat(const GenApi::INodeMap& nodeMap, double timeout) throw(...);

			// DeviceLostのコールバックを登録
			void RegisterDeviceLost(const GenApi::INodeMap& nodeMap) throw(...);

			// 接続済みを確認
			void IsConnected();

		private:
			//! 排他制御クラス(ライブラリ用)
			std::recursive_mutex m_asyncLibrary;

			//! システム
			StApi::CIStSystemPtr m_stSystem;

			//! インターフェース
			StApi::CIStDevicePtr m_stDevice;

			//! データストリーム
			StApi::CIStDataStreamPtr m_stDataStream;
		};
	}
}