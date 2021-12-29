#include "Worker.h"

namespace Standard
{
	namespace Axis
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			コンストラクタ
		////////////////////////////////////////////////////////////////////////////////
		CWorker::CWorker() : CTemplate()
		{

		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			デストラクタ
		////////////////////////////////////////////////////////////////////////////////
		CWorker::~CWorker()
		{

		}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			軸の制御、軸の監視、状態を通知する基礎クラスの例外を受け取る関数
		///// @param[in]		object	例外のクラス
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::OnPolingException(const Exception::CObject& object)
		//{

		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			デバイス名を取得 ※派生先で記述
		///// @return			デバイス名
		//////////////////////////////////////////////////////////////////////////////////
		//std::string CWorker::GetName()
		//{
		//	std::string ret = "Axis::";

		//	return ret;
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			接続の状態を取得
		///// @return			true:接続済み / false:未接続
		//////////////////////////////////////////////////////////////////////////////////
		//bool CWorker::GetConnected()
		//{
		//	bool ret = false;

		//	// 接続の状態を取得
		//	ret = m_poling.GetConnected();

		//	return ret;
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			初期化
		///// @detail			初期化のタイミングで呼び出される ※軸の制御、軸の監視、状態を通知するクラスの例外を通知する関数を登録、周期の更新など行う
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::InitializeDerived()
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "InitializeDerived()" << Logging::ConstSeparator;

		//	try
		//	{
		//		// デフォルトの設定クラスを取得
		//		Setting::CDefault defaultSetting = GetDefaultSetting();

		//		// 例外を通知する関数の登録
		//		postscript << "Poling::AttachFunctionException()";
		//		m_poling.AttachFunctionException(std::bind(&CWorker::OnPolingException, this, std::placeholders::_1));
		//		postscript.str("");

		//		// ポーリングの周期を更新
		//		postscript << "Poling::SetCycle(" << defaultSetting.Cycle << ")";
		//		m_poling.SetCycle(defaultSetting.Cycle);
		//		// ログ出力
		//		postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//		Transfer::Output(Logging::Join(logging, postscript.str()));
		//		postscript.str("");

		//		// 軸をポーリングする為の設定を取得
		//		postscript << "Poling::GetSetting()";
		//		Setting::CPoling polingSetting = m_poling.GetSetting();
		//		postscript.str("");

		//		// リトライ回数を更新
		//		postscript << "Poling::Setting::Retry(" << defaultSetting.Retry << ")";
		//		polingSetting.Retry = defaultSetting.Retry;
		//		// ログ出力
		//		postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//		Transfer::Output(Logging::Join(logging, postscript.str()));
		//		postscript.str("");

		//		// 軸をポーリングする為の設定を更新
		//		postscript << "Poling::SetSetting()";
		//		m_poling.SetSetting(polingSetting);
		//		postscript.str("");

		//		// 軸のポーリングを開始
		//		postscript << "Poling::Start()";
		//		m_poling.Start();
		//		postscript.str("");
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// 例外の処理 ⇒ 例外を通知＆発砲
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			破棄
		///// @detail			破棄のタイミングで呼び出される ※軸の制御、軸の監視、状態を通知するクラスの例外を通知する関数の解除など行う
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::DestroyDerived()
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "DestroyDerived()" << Logging::ConstSeparator;

		//	// 破棄
		//	try
		//	{
		//		// 例外を通知する関数の解除
		//		postscript << "Poling::DetachFunctionException()";
		//		m_poling.DetachFunctionException();
		//		// ログ出力
		//		postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//		Transfer::Output(Logging::Join(logging, postscript.str()));
		//		postscript.str("");
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// 例外の処理 ⇒ 例外を通知＆発砲
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			接続
		///// @detail			接続のタイミングで呼び出される ※軸の制御、軸の監視、状態を通知するクラスで接続が完了するのを待機など行う
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::ConnectDerived()
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisConnect;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "ConnectDerived()" << Logging::ConstSeparator;

		//	try
		//	{
		//		do
		//		{
		//			// 軸の制御、軸の監視、状態を通知する設定を取得
		//			postscript << "Poling::GetSetting()";
		//			Setting::CPoling polingSetting = m_poling.GetSetting();
		//			postscript.str("");

		//			// 軸へ接続を行う設定クラスを更新
		//			postscript << "Poling::Setting::Connect()";
		//			polingSetting.Connect = GetConnectSetting();
		//			postscript.str("");

		//			// 軸の制御、軸の監視、状態を通知する設定を更新
		//			postscript << "Poling::SetSetting()";
		//			m_poling.SetSetting(polingSetting);
		//			postscript.str("");

		//			// 軸の初期化を制御する情報クラス
		//			std::shared_ptr<Write::CInitialize> initializeWrite = std::make_shared<Write::CInitialize>();

		//			// 接続を待機するクラス
		//			Signal::CObserver<Plc::Result::CConnect> observer;

		//			// 接続を通知をする関数を登録
		//			postscript << "Subject::RegistrationFunctionConnect()";
		//			std::shared_ptr<std::function<Plc::OnConnect>> onCalled = std::make_shared<std::function<Plc::OnConnect>>(std::bind(&Signal::CObserver<Plc::Result::CConnect>::OnCalled, &observer, std::placeholders::_1));
		//			m_poling.RegistrationFunctionConnect(onCalled);
		//			postscript.str("");

		//			// 初期化を要求
		//			postscript << "Poling::RequestInitialize()";
		//			m_poling.RequestInitialize(initializeWrite);
		//			postscript.str("");

		//			// 接続を待つ
		//			postscript << "Observer::Wait()";
		//			observer.Wait();
		//			postscript.str("");

		//			// 接続の結果を取得
		//			postscript << "Observer::GetArgument()";
		//			std::shared_ptr<Plc::Result::CConnect> result = observer.GetArgument();
		//			postscript.str("");

		//			// 中断を確認
		//			if (result->Suspension)
		//			{
		//				postscript << "Observer::Initialize()" << Logging::ConstSeparator << "中断";
		//				// ログ出力
		//				postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//				Transfer::Output(Logging::Join(logging, postscript.str()));
		//				postscript.str("");
		//				break;
		//			}
		//		} while (false);
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// 例外の処理 ⇒ 例外を通知＆発砲
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			切断
		///// @detail			切断のタイミングで呼び出される ※軸の制御、軸の監視、状態を通知するクラスで切断が完了するのを待機など行う
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::DisconnectDerived()
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisDisconnect;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "DisconnectDerived()" << Logging::ConstSeparator;

		//	try
		//	{
		//		do
		//		{
		//			// 切断を待機するクラス
		//			Signal::CObserver<Plc::Result::CDisconnect> observer;

		//			// 切断を通知をする関数を登録
		//			postscript << "Subject::RegistrationFunctionDisconnect()";
		//			std::shared_ptr<std::function<Plc::OnDisconnect>> onCalled = std::make_shared<std::function<Plc::OnDisconnect>>(std::bind(&Signal::CObserver<Plc::Result::CDisconnect>::OnCalled, &observer, std::placeholders::_1));
		//			m_poling.RegistrationFunctionDisconnect(onCalled);
		//			postscript.str("");

		//			// 軸の破棄を制御する情報クラス
		//			std::shared_ptr<Write::CDestroy> destroyWrite = std::make_shared<Write::CDestroy>();

		//			// 破棄を要求
		//			postscript << "Poling::RequestDestroy()";
		//			m_poling.RequestDestroy(destroyWrite);
		//			postscript.str("");

		//			// 破棄を待つ
		//			postscript << "Observer::Wait()";
		//			observer.Wait();
		//			postscript.str("");

		//			// 接続の結果を取得
		//			postscript << "Observer::GetArgument()";
		//			std::shared_ptr<Plc::Result::CDisconnect> result = observer.GetArgument();
		//			postscript.str("");

		//			// 中断を確認
		//			if (result->Suspension)
		//			{
		//				postscript << "Observer::Destroy()" << Logging::ConstSeparator << "中断";
		//				// ログ出力
		//				postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//				Transfer::Output(Logging::Join(logging, postscript.str()));
		//				postscript.str("");
		//				break;
		//			}
		//		} while (false);
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// 例外の処理 ⇒ 例外を通知＆発砲
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////////////
		///// @brief			サーボを制御
		///// @param[in]		object	サーボを制御する設定クラス
		//////////////////////////////////////////////////////////////////////////////////
		//void CWorker::Servo(const Setting::CServo& setting)
		//{
		//	Logging::CObject logging;
		//	std::stringstream postscript;
		//	Exception::EnumCode errorCode = Exception::CodeAxisWrite;
		//	int deviceErrorCode = Exception::DeviceCodeSuccess;
		//	Utility::CStopWatch stopWatch;

		//	logging.Message << GetName() << "Servo()" << Logging::ConstSeparator;

		//	try
		//	{
		//		do
		//		{
		//			// サーボの制御を待機するクラス
		//			Signal::CObserver<Result::CSuper> observer;

		//			// サーボを制御する情報クラス
		//			std::shared_ptr<Write::CServo> servoWrite = std::make_shared<Write::CServo>();

		//			// サーボの状態を更新
		//			servoWrite->Mode = setting.Mode;

		//			// 軸の機器を走査
		//			for (const auto& driver : setting.Drivers)
		//			{
		//				// 軸の番号を更新
		//				servoWrite->Indexs.emplace_back(driver.first);
		//			}

		//			// サーボの制御を通知をする関数を登録
		//			postscript << "Write::_Funtion";
		//			//std::shared_ptr<std::function<Axis::OnServo>> onCalled = std::make_shared<std::function<Axis::OnServo>>(std::bind(&Signal::CObserver<Axis::Result::CServo>::OnCalled, &observer, std::placeholders::_1));
		//			//servoWrite._Function_ = onCalled;
		//			servoWrite->Subject.RegistrationFunction(std::make_shared<std::function<OnReceive>>(std::bind(&Signal::CObserver<Result::CSuper>::OnCalled, &observer, std::placeholders::_1)));
		//			postscript.str("");

		//			// 書き込みを要求
		//			postscript << "Poling::RequestWrite()";
		//			m_poling.RequestWrite(servoWrite);
		//			postscript.str("");

		//			// 制御を待つ
		//			postscript << "Observer::Wait()";
		//			observer.Wait();
		//			postscript.str("");

		//			// 制御の結果を取得
		//			postscript << "Observer::GetArgument()";
		//			auto result = std::dynamic_pointer_cast<Result::CSuper>(observer.GetArgument());
		//			postscript.str("");

		//			// 中断を確認
		//			if (result->Suspension)
		//			{
		//				postscript << "Observer::Write()" << Logging::ConstSeparator << "中断";
		//				// ログ出力
		//				postscript << Logging::ConstSeparator << stopWatch.Format(true);
		//				Transfer::Output(Logging::Join(logging, postscript.str()));
		//				postscript.str("");
		//				break;
		//			}
		//		} while (false);
		//	}
		//	catch (const std::exception& e)
		//	{
		//		// 例外の処理 ⇒ 例外を通知＆発砲
		//		throw OutputException(errorCode, deviceErrorCode, logging, postscript.str(), e);
		//	}
		//}
	}
}