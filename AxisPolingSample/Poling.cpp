#include "Poling.h"

namespace Standard
{
	namespace Axis
	{
		namespace Poling
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CWorker::CWorker() : Worker::CTemplate()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CWorker::~CWorker()
			{

			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			完了を通知
			/// @param[in]		object	通知する情報のクラス(テンプレートで指定)
			/// @param[in]		remove	true:通知後に削除 / false:通知後に削除しない
			////////////////////////////////////////////////////////////////////////////////
			template <typename CWrite, typename CArgument>
			void CWorker::NotifyComplete(std::shared_ptr<CArgument> object, bool remove/*=true*/)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeUnknown;//@@@
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "NotifyComplete(" << typeid(CWrite).name() << ")" << Logging::ConstSeparator;

				try
				{
					//// 排他制御
					//std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//// 通知した監視している軸の情報クラス
					//std::vector<std::vector<std::shared_ptr<Write::CSuper>>::iterator> notifieds;

					//// 監視している軸を走査
					//for (std::vector<std::shared_ptr<Write::CSuper>>::iterator surveillance = m_surveillances.begin(); surveillance != m_surveillances.end(); surveillance++)
					//{
					//	// 制御する情報クラスを確認
					//	if (typeid(CWrite) != typeid(surveillance))
					//	{
					//		// 制御する情報クラスが不一致
					//		continue;
					//	}

					//	// 通知を呼び出し
					//	if ((*surveillance)->Subject.CallingFunction(object))
					//	{
					//		// 呼び出しを成功
					//		notifieds.emplace_back(surveillance);
					//	}
					//}

					//// 削除フラグを確認
					//if (remove)
					//{
					//	for (const auto& notified : notifieds)
					//	{
					//		// 監視している軸を削除
					//		m_surveillances.erase(notified);
					//	}
					//}

					//// ログ出力
					//postscript << "Count:" << notifieds.size() << Logging::ConstSeparator << stopWatch.Format(true);
					//Transfer::Output(Logging::Join(logging, postscript.str()));
					//postscript.str("");
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を通知＆発砲
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			初期化
			/// @detail			メインスレッドから初期化のタイミングで呼び出される
			///					★継承した関数
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::InitializeDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CInitialize trigger;

				logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

				try
				{
					//@@@ 疑似的な待ち時間
					std::this_thread::sleep_for(std::chrono::milliseconds(100));

					// ★必須
					//@@@ 接続する処理なんかを記述する

					// ★必須
					// 初期化の完了を起床
					postscript << "WakeupInitializeComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupInitializeComplete(trigger);
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 初期化の完了を起床
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupInitializeComplete(trigger);

					// 例外を通知＆発砲
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			破棄
			/// @detail			メインスレッドから破棄のタイミングで呼び出される
			///					★継承した関数
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::DestroyDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;
				Signal::Trigger::CDestroy trigger;

				logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

				try
				{
					//@@@ 疑似的な待ち時間
					std::this_thread::sleep_for(std::chrono::milliseconds(700));

					// ★必須
					//@@@ 切断する処理なんかを記述する

					// ★必須
					// 破棄の完了を起床
					postscript << "WakeupDestroyComplete()" << Logging::ConstSeparator << stopWatch.Format(true);
					WakeupDestroyComplete(trigger);
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 破棄の完了を起床
					trigger.Suspension = true;
					trigger.Message = e.what();
					WakeupDestroyComplete(trigger);

					// 例外を通知＆発砲
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			読み込み
			/// @detail			軸の状態を確認する
			///					★継承した関数
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::ReadDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisRead;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "ReadDerived()" << Logging::ConstSeparator;

				try
				{
					size_t index = 0;

					//// 排他制御
					//std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//// 監視している軸の情報を走査
					//while (index < m_surveillances.size())
					//{
					//	// 完了フラグ
					//	bool complete = false;

					//	// 軸を制御する情報のスーパークラス
					//	std::shared_ptr<Write::CSuper> super = m_surveillances.at(index);

					//	// 軸のサーボを制御する基礎クラスに変換
					//	auto basis = std::dynamic_pointer_cast<Write::CBasis>(super);

					//	try
					//	{
					//		// 軸制御する設定クラスを確認
					//		if (typeid(Write::CServo) == typeid(*basis))
					//		{
					//			// サーボを制御する設定
					//			MotionApi::Setting::CServoControl servoSetting;

					//			// デバイスのハンドルを設定
					//			servoSetting.DeviceHandle = basis->DeviceHandle;

					//			// サーボを制御
					//			postscript << "Plc::Servo()" << Logging::ConstSeparator << "デバイスのハンドル:" << basis->DeviceHandle;
					//			Plc::Servo(GetName(), servoSetting);
					//			postscript.str("");

					//			// 完了フラグを設定
					//			complete = true;
					//		}
					//	}
					//	catch (const Exception::CObject& e)
					//	{
					//		// 例外の処理 ⇒ デバイスのハンドルを破棄
					//		postscript << "GetDeviceHandle()";
					//		Plc::DestroyDevice(GetName(), basis->DeviceHandle);
					//		postscript.str("");

					//		deviceErrorCode = e.DeviceErrorCode;

					//		// 例外を発砲
					//		throw std::exception(e.what());
					//	}

					//	// 完了フラグを確認
					//	if (complete)
					//	{
					//		// 監視している軸の情報を削除 ※先頭
					//		m_surveillances.erase(m_surveillances.begin());
					//		continue;
					//	}

					//	// 次のデバイスへ
					//	index += 1;
					//}
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を通知＆発砲
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			書き込み
			/// @detail			軸の制御を開始する
			///					★継承した関数
			////////////////////////////////////////////////////////////////////////////////
			void CWorker::WriteDerived()
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << GetName() << "WriteDerived()" << Logging::ConstSeparator;

				try
				{
					//bool run = true;

					//// 書き込み可能な軸だけ制御
					//while (run)
					//{
					//	// 書き込み可能な軸のスーパークラスを取得
					//	auto super = GetWrite(true);

					//	// 書き込み可能な軸のスーパークラスを確認
					//	if (super == nullptr)
					//	{
					//		// 該当なし
					//		run = false;
					//		continue;
					//	}

					//	//// 軸のサーボを制御する基礎クラスに変換
					//	//auto basis = std::dynamic_pointer_cast<Write::CBasis>(super);

					//	//// デバイスのハンドルを取得
					//	//postscript << "GetDeviceHandle()";
					//	//basis->DeviceHandle = GetDeviceHandle(basis->Indexs);
					//	//postscript.str("");

					//	//// 排他制御
					//	//std::lock_guard<std::recursive_mutex> lock(m_asyncSurveillances);

					//	//// 監視している軸の情報クラスに追加
					//	//m_surveillances.push_back(basis);
					//}
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を通知＆発砲
					throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
				}
			}
		}
	}
}