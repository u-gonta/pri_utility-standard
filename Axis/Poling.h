#pragma once

#include <algorithm>

#include "Common/Axis/Status.h"
#include "Signal/WorkerExpand.h"
#include "PolingSetting.h"

namespace Standard
{
	namespace Axis
	{
		namespace Execution
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CFinish
			/// @brief      制御の結果を保持するクラス
			///				⇒ 状態を監視した結果を保持するテンプレートのクラスから継承
			////////////////////////////////////////////////////////////////////////////////
			class CFinish
				: virtual public Signal::Result::CTemplate
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CFinish() : Signal::Result::CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @return			自クラス
				////////////////////////////////////////////////////////////////////////////////
				CFinish& operator = (const CFinish& object) noexcept
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CFinish()
				{

				}
			};

			//! 制御した結果を通知する関数の定義
			typedef void(OnWakeup)(const Execution::CFinish&);

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CSetting
			/// @brief      制御を実行する設定のクラス
			////////////////////////////////////////////////////////////////////////////////
			class CSetting
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CSetting()
				{
					Wakeup.reset();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コピー代入演算子
				/// @return			自クラス
				////////////////////////////////////////////////////////////////////////////////
				CSetting& operator = (const CSetting& object) noexcept
				{
					// 更新
					Update(object);

					return *this;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CSetting()
				{

				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			自クラスを更新
				/// @param[in]		object	自クラス
				////////////////////////////////////////////////////////////////////////////////
				void Update(const CSetting& object)
				{
					this->Output = object.Output;
					this->Wakeup = object.Wakeup;
				}

			public:
				//! 軸の状態(出力)を指定するクラス
				Status::COutput Output;

				//! 制御した結果の通知を呼び出す関数
				std::weak_ptr<std::function<OnWakeup>> Wakeup;
			};
		}

		namespace Poling
		{
			namespace Worker
			{
				////////////////////////////////////////////////////////////////////////////////
				/// @class      CTemplate
				/// @brief      軸の制御、軸の監視、状態を通知するテンプレートのクラス
				///				⇒ 状態を監視、書き込み、通知するテンプレートのクラスから派生
				/// @detail		軸の制御、軸の監視、状態の通知を行う
				////////////////////////////////////////////////////////////////////////////////
				class CTemplate
					: virtual public Signal::Worker::CTemplateExpand<Poling::CSetting, Status::CInput, Execution::CSetting>
				{
				public:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			コンストラクタ
					////////////////////////////////////////////////////////////////////////////////
					CTemplate() : Signal::Worker::CTemplateExpand<Poling::CSetting, Status::CInput, Execution::CSetting>()
					{
						std::string name;

						// 名称を更新
						name = "Axis::Poling::";
						SetName(name);
						SetNameException(name + "Exception::");
						SetNameArgument(name + "Notice::");
					}

					////////////////////////////////////////////////////////////////////////////////
					/// @brief			デストラクタ
					////////////////////////////////////////////////////////////////////////////////
					virtual ~CTemplate()
					{

					}

				protected:
					////////////////////////////////////////////////////////////////////////////////
					/// @brief			処理の完了を起床
					/// @param[in]		object	制御を実行した結果のクラス
					/// @param[in]		function 制御を実行した結果の通知を呼び出す関数
					////////////////////////////////////////////////////////////////////////////////
					void WakeupFinish(const Execution::CFinish& object, const std::weak_ptr<std::function<Execution::OnWakeup>>& function) noexcept
					{
						Logging::CObject logging;

						logging.Message << GetName() << "WakeupFinish()" << Logging::ConstSeparator;

						try
						{
							Signal::Subject::CTemplate<Execution::CFinish> subject;

							// 関数を登録
							subject.RegistrationFunction(function);

							// 関数を通知
							subject.CallingFunction(object);
						}
						catch (const std::exception& e)
						{
							// 例外の処理 ⇒ ログへ追記
							logging.Message << e.what();
							// ログを保存
							Transfer::Output(logging);
						}
					}

					//////////////////////////////////////////////////////////////////////////////////
					///// @brief			書き込むデータのクラスを確認 ※制御したい軸の有無を取得
					///// @detail			制御したい軸が監視中の軸に含まれている場合にはデータなしを返す
					///// @return			true:書き込むデータあり / false:書き込むデータなし
					//////////////////////////////////////////////////////////////////////////////////
					//bool IsWrite()
					//{
					//	bool ret = false;

					//	std::stringstream postscript;
					//	Exception::EnumCode errorCode = Exception::CodeUnknown;
					//	int deviceErrorCode = Exception::DeviceCodeSuccess;

					//	postscript << GetName() << "IsWrite()";

					//	try
					//	{
					//		// 排他制御
					//		std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

					//		// 書き込み可能な軸の情報クラスを取得
					//		if (GetWrite() != nullptr)
					//		{
					//			// 書き込み可能な軸の情報クラスあり
					//			ret = true;
					//		}
					//	}
					//	catch (const std::exception& e)
					//	{
					//		// 例外の処理
					//		postscript << Logging::ConstSeparator << e.what();

					//		// 例外を発砲
					//		throw Exception::CObject(errorCode, deviceErrorCode, postscript.str());
					//	}

					//	return ret;
					//}

					//////////////////////////////////////////////////////////////////////////////////
					///// @brief			書き込み中を確認 ※軸の実施中を取得
					///// @return			true:書き込み中 / false:書き込みなし
					//////////////////////////////////////////////////////////////////////////////////
					//bool IsWriting() override
					//{
					//	bool ret = false;

					//	// 排他制御
					//	std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//	// 監視している軸の情報を確認
					//	if (0 < m_surveillances.size())
					//	{
					//		// 監視中の軸あり
					//		ret = true;
					//	}

					//	return ret;
					//}

					//////////////////////////////////////////////////////////////////////////////////
					///// @brief			書き込み可能な軸のスーパークラスを取得 ※制御する事が可能な軸の情報を取得
					///// @detail			制御したい軸あっても監視中の軸に含まれている場合にはnullptrを返す
					///// @param[in]		remove	ture:戻り値に該当する書き込み可能な軸の情報クラスを削除 / false:戻り値に該当する書き込み可能な軸の情報クラスは削除しない
					///// @return			軸の情報クラス
					//////////////////////////////////////////////////////////////////////////////////
					//std::shared_ptr<Write::CSuper> GetWrite(bool remove = false)
					//{
					//	std::shared_ptr<Write::CSuper> ret = nullptr;

					//	// 排他制御
					//	std::lock_guard<std::recursive_mutex> lock(m_asyncWrite);

					//	// 書き込み可能な軸のイテレータ
					//	std::deque<std::shared_ptr<Write::CSuper>>::iterator extraction = m_writes.end();

					//	// 書き込み可能な軸を後方から走査
					//	for (auto write = m_writes.begin(); write != m_writes.end(); write++)
					//	{
					//		// 軸の番号が一致フラグ
					//		bool valid = false;

					//		// 排他制御
					//		std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//		// 監視している軸の情報を走査
					//		std::for_each(m_surveillances.begin(), m_surveillances.end(), [this, write, &valid](auto surveillance)
					//		{
					//			// 書き込み可能な軸の番号を走査
					//			for (const auto& source : (*write)->Indexs)
					//			{
					//				// 監視している軸の番号を走査
					//				for (const auto& target : surveillance->Indexs)
					//				{
					//					// 書き込み可能な軸の番号と監視している軸の番号を比較
					//					if (source == target)
					//					{
					//						// 番号が一致
					//						valid = true;
					//						break;
					//					}
					//				}
					//			}
					//		});

					//		// 軸の番号が一致を確認
					//		if (valid == false)
					//		{
					//			// 一致する番号なし
					//			extraction = write;
					//		}
					//	}

					//	// 書き込み可能な軸のイテレータを確認
					//	if (extraction != m_writes.end())
					//	{
					//		// 書き込み可能な軸の情報クラスを確定
					//		ret = *extraction;

					//		// 削除を確認
					//		if (remove)
					//		{
					//			// 書き込み可能な軸を削除
					//			m_writes.erase(extraction);
					//		}
					//	}

					//	return ret;
					//}

				protected:
					////! 排他制御クラス(監視データのクラス用)
					//std::recursive_mutex m_asyncSurveillances;

					////! 監視している軸の指定クラス
					//std::vector<std::shared_ptr<Status::Output::CTemplate>> m_surveillances;
				};
			}
		}
	}
}
