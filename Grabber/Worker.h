#pragma once

#include "Common/Image/Basis.h"
#include "Common/Grabber/Condition.h"
#include "Common/Grabber/Setting.h"
#include "Device/Worker.h"

namespace Standard
{
	namespace Grabber
	{
		namespace Worker
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @class      CTemplate
			/// @brief      カメラの制御を行うテンプレートのクラス
			///				⇒ デバイスの制御を行うテンプレートのクラスから派生
			/// @param[in]	CDefault	デフォルトな設定クラスを指定
			/// @param[in]	CConnect	接続の設定クラスを指定
			////////////////////////////////////////////////////////////////////////////////
			class CTemplate
				: virtual public Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CTemplate() : Device::Worker::CTemplate<Setting::CDefault, Setting::CConnect>()
				{
					// 名称を更新
					SetName("Grabber::");
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				virtual ~CTemplate()
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			取り込みの条件クラスを取得
				/// @return			取り込みの条件クラス
				////////////////////////////////////////////////////////////////////////////////
				Condition::CCapture GetCaptureCondition()
				{
					Condition::CCapture ret;

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncCaptureCondition);

					// 取り込みの条件クラスを取得
					ret = m_captureCondition;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			アナログゲインを読み込み
				/// @return			アナログゲイン値
				////////////////////////////////////////////////////////////////////////////////
				double ReadAnalogGain() throw(...)
				{
					double ret = 0;

					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ReadAnalogGain()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 制御が可能か確認
					IsControllable();

					// アナログゲインを読み込み
					ret = ReadAnalogGainDerived();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			アナログゲインを書き込み
				/// @param[in]		value	アナログゲイン値
				////////////////////////////////////////////////////////////////////////////////
				void WriteAnalogGain(double value) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WriteAnalogGain()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 制御が可能か確認
					IsControllable();

					// アナログゲインを書き込み
					WriteAnalogGainDerived(value);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デジタルゲインを読み込み
				/// @return			デジタルゲイン値
				////////////////////////////////////////////////////////////////////////////////
				double ReadDigitalGain() throw(...)
				{
					double ret = 0;

					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ReadDigitalGain()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 制御が可能か確認
					IsControllable();

					// デジタルゲインを読み込み
					ret = ReadDigitalGainDerived();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デジタルゲインを書き込み
				/// @param[in]		value	デジタルゲイン値
				////////////////////////////////////////////////////////////////////////////////
				void WriteDigitalGain(double value) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WriteDigitalGain()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 制御が可能か確認
					IsControllable();

					// デジタルゲインを書き込み
					WriteDigitalGainDerived(value);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			露光時間を読み込み
				/// @return			露光時間
				////////////////////////////////////////////////////////////////////////////////
				double ReadExposureTime() throw(...)
				{
					double ret = 0;

					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ReadExposureTime()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 制御が可能か確認
					IsControllable();

					// 露光時間を読み込み
					ret = ReadExposureTimeDerived();

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			露光時間を書き込み
				/// @param[in]		value	露光時間
				////////////////////////////////////////////////////////////////////////////////
				void WriteExposureTime(double value) throw(...)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "WriteExposureTime()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 制御が可能か確認
					IsControllable();

					// 露光時間を書き込み
					WriteExposureTimeDerived(value);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			取り込み
				/// @return			画像のクラス
				////////////////////////////////////////////////////////////////////////////////
				Image::CObject Capture() throw(...)
				{
					Image::CObject ret;

					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "Capture()" << Logging::ConstSeparator;

					// ログ出力
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");

					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncFlag);

					// 制御が可能か確認
					IsControllable();

					// 取り込み
					ret = CaptureDerived();

					return ret;
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			取り込みの条件クラスを更新
				/// @param[in]		object	取り込みの条件クラス
				////////////////////////////////////////////////////////////////////////////////
				void SetCaptureCondition(const Condition::CCapture& object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncCaptureCondition);

					// 取り込みの条件クラスを更新
					m_captureCondition = object;
				}

			protected:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			初期化済みの例外を発砲
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionInitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionInitialized()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeGrabberInitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄済みの例外を発砲
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionDiscarded() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionDiscarded()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeGrabberDiscarded, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			未初期化の例外を発砲
				///					★継承した関数
				////////////////////////////////////////////////////////////////////////////////
				void ExceptionUninitialized() override
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << GetName() << "ExceptionUninitialized()" << Logging::ConstSeparator;

					// 例外を発砲
					OutputException(Exception::CodeGrabberUninitialized, logging, postscript.str());
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			アナログゲインを読み込み
				///	@detail			★派生先で記述する
				/// @return			アナログゲイン値
				////////////////////////////////////////////////////////////////////////////////
				virtual double ReadAnalogGainDerived() throw(...)
				{
					double ret = 0;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			アナログゲインを書き込み
				///	@detail			★派生先で記述する
				/// @param[in]		value	アナログゲイン値
				////////////////////////////////////////////////////////////////////////////////
				virtual void WriteAnalogGainDerived(double value) throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デジタルゲインを読み込み
				///	@detail			★派生先で記述する
				/// @return			デジタルゲイン値
				////////////////////////////////////////////////////////////////////////////////
				virtual double ReadDigitalGainDerived() throw(...)
				{
					double ret = 0;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デジタルゲインを書き込み
				///	@detail			★派生先で記述する
				/// @param[in]		value	デジタルゲイン値
				////////////////////////////////////////////////////////////////////////////////
				virtual void WriteDigitalGainDerived(double value) throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			露光時間を読み込み
				///	@detail			★派生先で記述する
				/// @return			露光時間
				////////////////////////////////////////////////////////////////////////////////
				virtual double ReadExposureTimeDerived() throw(...)
				{
					double ret = 0;

					return ret;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			露光時間を書き込み
				///	@detail			★派生先で記述する
				/// @param[in]		value	露光時間
				////////////////////////////////////////////////////////////////////////////////
				virtual void WriteExposureTimeDerived(double value) throw(...)
				{

				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			取り込み
				///	@detail			★派生先で記述する
				/// @return			画像のクラス
				////////////////////////////////////////////////////////////////////////////////
				virtual Image::CObject CaptureDerived() throw(...)
				{
					Image::CObject ret;

					return ret;
				}

			private:
				//! 排他制御クラス(取り込みの条件クラス用)
				std::recursive_mutex m_asyncCaptureCondition;

				//! 取り込みの条件クラス
				Condition::CCapture m_captureCondition;
			};
		}
	}
}

