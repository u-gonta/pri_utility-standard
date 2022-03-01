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
			/// @class      CSurveillance
			/// @brief      軸を監視する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			class CSurveillance
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @enum			監視の種類
				////////////////////////////////////////////////////////////////////////////////
				enum EnumStatus
				{
					StatusFinish		// 終了を監視
				};

			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CSurveillance()
				{
					Status = StatusFinish;
					Wakeup.reset();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @return			自クラス
				////////////////////////////////////////////////////////////////////////////////
				CSurveillance& operator = (const CSurveillance& object) noexcept
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CSurveillance()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CSurveillance& object)
				{
					this->Status = object.Status;
					this->Ids.clear();
					std::copy(object.Ids.begin(), object.Ids.end(), std::back_inserter(this->Ids));
					this->Wakeup = object.Wakeup;
				}

			public:
				//! 監視の種類
				EnumStatus Status;

				//! 軸の識別子
				std::vector<int> Ids;

				//! 制御した結果の通知を呼び出す関数
				std::weak_ptr<std::function<Execution::OnWakeup>> Wakeup;
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

				// 軸の状態を追加
				void AdditionStatus(int id, const CStatus& object);

				// 軸の状態を取得
				CStatus GetStatus(int id);

				// 全軸の状態を取得
				std::map<int, CStatus> GetStatus();

				// 軸を監視する数を取得
				size_t GetSurveillanceSize();

				// 軸の監視を取得
				CSurveillance GetSurveillance(size_t index);

				// 軸の監視を追加
				void AdditionSurveillance(const CSurveillance& object);

				// 軸の監視を削除
				void EraseSurveillance(const std::vector<size_t>& indexs);

				// 入力レジスタの値を機器の状態を保持するクラスへ変換
				Axis::Status::Input::CDriver ConvertInput(const Plc::Register::Result::CContent& object);

				// 出力レジスタの値を機器の状態を保持するクラスへ変換
				Axis::Status::Input::CDriver ConvertOutput(const Plc::Register::Result::CContent& object);

				// 制御を停止
				bool Suspend(const Execution::CSetting& object);

				// サーボを制御
				bool Servo(const Execution::CSetting& object);

				// 原点復帰を実行
				bool MoveOrigin(const Execution::CSetting& object);

				// ジョグを開始
				bool StartJog(const Execution::CSetting& object);

				// ジョグを停止
				bool StopJog(const Execution::CSetting& object);

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
				std::vector<CSurveillance> m_surveillances;
			};
		}
	}
}
