#pragma once

#include "Common/Io/Status.h"
#include "Common/Io/Setting.h"
#include "Execution.h"
#include "SettingDevice.h"
#include "Result.h"

#include "PlcMotionApi/ExecutionRegister.h"

namespace Standard
{
	namespace Plc
	{
		namespace Device
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デバイスのハンドルを作成
			/// @param[in]		logging ログの情報を保持するクラス
			/// @param[in]		object デバイスのハンドルを作成する設定クラス
			/// @return			デバイスのハンドルを作成した結果クラス
			////////////////////////////////////////////////////////////////////////////////
			MotionApi::Device::Result::CDeclare DeclareDevice(const Logging::CObject& logging, const MotionApi::Device::Setting::CDeclare& object)
			{
				MotionApi::Device::Result::CDeclare ret;
				std::stringstream postscript;

				// デバイスのハンドルを作成
				postscript << "MotionApi::DeclareDevice()";
				postscript << Logging::ConstSeparator << "軸ハンドル:";
				for (auto iterator = object.Handles.begin(); iterator != object.Handles.end(); iterator++)
				{
					// ハンドルの順番が先頭より後か確認
					if (0 < std::distance(object.Handles.begin(), iterator))
					{
						postscript << Logging::ConstSeparator;
					}
					postscript << FormatHandle(*iterator);
				}
				ret = MotionApi::DeclareDevice(object);
				// ログ出力
				postscript << Logging::ConstSeparator << "デバイスハンドル:" << FormatHandle(ret.Handle);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				return ret;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デバイスのハンドルをクリア
			/// @param[in]		logging ログの情報を保持するクラス
			/// @param[in]		object 軸のハンドルを作成した結果クラス
			////////////////////////////////////////////////////////////////////////////////
			void ClearDevice(const Logging::CObject& logging, const MotionApi::Device::Result::CDeclare& object)
			{
				std::stringstream postscript;

				MotionApi::Device::Setting::CClear setting;

				// ハンドルを更新
				setting.Handle = object.Handle;

				// デバイスのハンドルをクリア
				postscript << "MotionApi::ClearDevice()";
				postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(setting.Handle);
				MotionApi::ClearDevice(setting);
				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			制御を停止
			/// @param[in]		object 制御を停止する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			inline void Suspend(const Device::Setting::CSuspend& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Suspend()" << Logging::ConstSeparator;

				try
				{
					// 軸を走査
					for (const auto& axis : object.Axises)
					{
						MotionApi::Register::Setting::CDeclare declare;

						postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(axis.Handle);
						Io::Specify::CAddress address;
						address.Category = Plc::Register::ConstCategoryO;
						address.Category += Plc::Register::ConstTypeWord;
						address.Hex = true;
						address.Margin = 4;
						address.Index = axis.Address + 8;
						postscript << Logging::ConstSeparator << "アドレス:" << Io::Specify::Format(address);
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// レジスタのアドレスを更新
						Register::CSpecify specify(address.Category);
						specify.Index = address.Index;
						postscript << "Register::CSpecify::GetAddress()";
						declare.Address = specify.GetAddress();
						postscript.str("");

						// レジスタのハンドルを作成
						postscript << "MotionApi::DeclareRegister()";
						postscript << Logging::ConstSeparator << "アドレス:" << declare.Address;
						auto result = MotionApi::DeclareRegister(declare);
						// ログ出力
						postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(result.Handle);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						MotionApi::Register::Setting::CSetContent setting;

						// ハンドルを更新
						setting.Handle = result.Handle;

						// データを格納
						setting.Buffers.emplace_back(0);

						// データ数を更新
						setting.Size = 1;

						// レジスタの値を更新
						postscript << "MotionApi::SetRegisterData()" << Logging::ConstSeparator << "ハンドル:" << FormatHandle(setting.Handle);
						postscript << Logging::ConstSeparator << "更新サイズ:" << setting.Size;
						MotionApi::SetRegisterData(setting);
						// ログ出力
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
					}
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			サーボを制御
			/// @param[in]		object サーボの制御を行う設定クラス
			////////////////////////////////////////////////////////////////////////////////
			inline void Servo(const Device::Setting::CServo& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Servo()" << Logging::ConstSeparator;

				try
				{
					// デバイスのハンドルを作成
					postscript << "DeclareDevice()";
					auto result = DeclareDevice(logging, object);
					postscript.str("");

					try
					{
						// サーボの制御を行う設定クラス
						MotionApi::Device::Setting::CServo setting = object;

						// ハンドルを更新
						setting.Handle = result.Handle;

						// サーボを制御
						postscript << "MotionApi::ServoControl()";
						postscript << Logging::ConstSeparator << "ハンドル:" << Plc::FormatHandle(setting.Handle);
						postscript << Logging::ConstSeparator << "状態:" << (setting.Mode == Signal::StatusOn ? "ON" : "OFF");
						postscript << Logging::ConstSeparator << "タイムアウト:" << setting.Timeout;
						MotionApi::ServoControl(setting);
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// デバイスのハンドルをクリア
						postscript << "ClearDevie()";
						ClearDevice(logging, result);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ デバイスのハンドルをクリア
						ClearDevice(logging, result);
						throw;
					}
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			原点復帰を実行
			/// @param[in]		object 原点復帰を実行する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			inline void MoveOrigin(const Device::Setting::CMoveOrigin& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "MoveOrigin()" << Logging::ConstSeparator;

				try
				{
					MotionApi::Device::Setting::CMoveOrigin setting = object;

					// デバイスのハンドルを作成
					postscript << "DeclareDevice()";
					auto result = DeclareDevice(logging, setting.GetDeclare());
					postscript.str("");

					try
					{
						// ハンドルを更新
						setting.Handle = result.Handle;

						// 軸を走査
						for (const auto& axis : setting.Axises)
						{
							postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(axis.Handle);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert((MotionApi::Device::Setting::Motion::CTemplate)axis);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert((MotionApi::Device::Setting::Position::CTemplate)axis);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert(axis.Method);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert(axis.Direction);
							// ログ出力
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
						}

						// 原点復帰を実行
						postscript << "MotionApi::MoveOrigin()";
						postscript << Logging::ConstSeparator << "ハンドル:" << Plc::FormatHandle(setting.Handle);
						MotionApi::MoveOrigin(setting);
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// デバイスのハンドルをクリア
						postscript << "ClearDevie()";
						ClearDevice(logging, result);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ デバイスのハンドルをクリア
						ClearDevice(logging, result);
						throw;
					}
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ジョグを開始
			/// @param[in]		object ジョグを開始する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			inline void StartJog(const Device::Setting::CStartJog& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "StartJog()" << Logging::ConstSeparator;

				try
				{
					MotionApi::Device::Setting::CStartJog setting = object;

					// デバイスのハンドルを作成
					postscript << "DeclareDevice()";
					auto result = DeclareDevice(logging, setting.GetDeclare());
					postscript.str("");

					try
					{
						// ハンドルを更新
						setting.Handle = result.Handle;

						// 軸を走査
						for (const auto& axis : setting.Axises)
						{
							postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(axis.Handle);
							postscript << Logging::ConstSeparator << MotionApi::Message::Convert(axis);
							postscript << Logging::ConstSeparator << "方向:" << axis.Direction;
							postscript << Logging::ConstSeparator << "タイムアウト:" << axis.Timeout;
							// ログ出力
							postscript << Logging::ConstSeparator << stopWatch.Format(true);
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
						}

						// ジョグを実行
						postscript << "MotionApi::StartJog()";
						postscript << Logging::ConstSeparator << "ハンドル:" << Plc::FormatHandle(setting.Handle);
						MotionApi::StartJog(setting);
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// デバイスのハンドルをクリア
						postscript << "ClearDevie()";
						ClearDevice(logging, result);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ デバイスのハンドルをクリア
						ClearDevice(logging, result);
						throw;
					}
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			ジョグを停止
			/// @param[in]		object ジョグを停止する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			inline void StopJog(const Device::Setting::CStopJog& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodeAxisWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "StopJog()" << Logging::ConstSeparator;

				try
				{
					MotionApi::Device::Setting::CStopJog setting = object;

					// デバイスのハンドルを作成
					postscript << "DeclareDevice()";
					auto result = DeclareDevice(logging, setting.GetDeclare());
					postscript.str("");

					try
					{
						// ハンドルを更新
						setting.Handle = result.Handle;

						// 軸を走査
						for (const auto& axis : setting.Axises)
						{
							postscript << Logging::ConstSeparator << "軸:" << FormatHandle(axis.Handle);
							postscript << Logging::ConstSeparator << "完了:" << axis.Complete;
							// ログ出力
							postscript << Logging::ConstSeparator << stopWatch.Format(true);
							Transfer::Output(Logging::Join(logging, postscript.str()));
							postscript.str("");
						}

						// ジョグを停止
						postscript << "MotionApi::StopJog()";
						postscript << Logging::ConstSeparator << "ハンドル:" << Plc::FormatHandle(setting.Handle);
						MotionApi::StopJog(setting);
						// ログ出力
						postscript << Logging::ConstSeparator << stopWatch.Format(true);
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");

						// デバイスのハンドルをクリア
						postscript << "ClearDevie()";
						ClearDevice(logging, result);
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ デバイスのハンドルをクリア
						ClearDevice(logging, result);
						throw;
					}
				}
				catch (const MotionApi::CException& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, e.ErrorCode, logging, postscript.str(), e));
				}
				catch (const std::exception& e)
				{
					// 例外の処理 ⇒ 例外を発砲
					throw Exception::CObject(errorCode, Exception::Convert::Message(errorCode, deviceErrorCode, logging, postscript.str(), e));
				}
			}
		}
	}
}