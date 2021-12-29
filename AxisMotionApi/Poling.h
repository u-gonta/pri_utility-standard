#pragma once

#include "Axis/Poling.h"

#include "PlcMotionApi/Result.h"

namespace Standard
{
	namespace Axis
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CStatus
			/// @brief      軸の状態クラス
			////////////////////////////////////////////////////////////////////////////////
			class CStatus
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CStatus()
				{
					Name = "";
					Handle = 0;
					Address = 0x8000;
					Resolution = 1;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @return			自クラス
				////////////////////////////////////////////////////////////////////////////////
				CStatus& operator = (const CStatus& object) noexcept
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CStatus()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CStatus& object)
				{
					this->Name = object.Name;
					this->Handle = object.Handle;
					this->Address = object.Address;
					this->Resolution = object.Resolution;
				}

			public:
				//! 軸の名称
				std::string Name;

				//! 軸のハンドル
				unsigned long Handle;

				//! 軸のレジスタ先頭アドレス
				unsigned short Address;

				//! 位置の分解能
				double Resolution;
			};

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

				// リトライを確認
				bool IsRetry() override;

				// リトライの遅延時間を取得
				int GetDelayRetry() override;

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

				// 軸を開放
				void Clear();

				// 軸を追加
				void Addition(int id, const CStatus& status);

				// 軸を取得
				CStatus Get(int id);

				// 全軸を取得
				std::map<int, CStatus> Get();

				// サーボを制御
				void Servo(const Surveillance::CSetting& object);

				// ジョグを実行
				void StartJog(const Surveillance::CSetting& object);

				// ジョグを停止
				void StopJog(const Surveillance::CSetting& object);

			private:
				//! 排他制御クラス(コントローラの情報クラス用)
				std::recursive_mutex m_asyncController;

				//! コントローラを開いた結果クラス
				MotionApi::Controller::Result::COpen m_controller;

				//! 排他制御クラス(軸の状態用)
				std::recursive_mutex m_asyncAxisStatus;

				//! 軸の状態クラス
				std::map<int, CStatus> m_axisStatuses;

				//! 排他制御クラス(制御を監視する設定のクラス用)
				std::recursive_mutex m_asyncSurveillance;

				//! 制御を監視する設定のクラス
				std::vector<Surveillance::CSetting> m_surveillances;
			};
		}
	}
}
