#pragma once

#include "Exception/Convert.h"
#include "WrapperMotionApi/include/ErrorCode.h"
#include "WrapperMotionApi/Exception.h"
#include "WrapperMotionApi/WrapperMotionApi.h"
#include "WrapperMotionApi/Result.h"
#include "Setting.h"
#include "SettingController.h"
#include "SettingRegister.h"
#include "SettingAxis.h"
#include "SettingDevice.h"
#include "Register.h"
#include "Result.h"

#pragma comment(lib, "WrapperMotionApi.lib")

namespace Standard
{
	namespace Plc
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			ハンドルを文字列へ変換
		/// @param[in]		value ハンドルの値
		/// @return			文字列に変換したハンドル
		////////////////////////////////////////////////////////////////////////////////
		inline std::string FormatHandle(unsigned long value)
		{
			std::string ret = "";

			// ハンドルを文字列へ変換
			ret = Text::FormatX(8, value);

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			コントローラを初期化
		/// @param[in]		object コントローラを初期化する設定クラス
		/// @return			コントローラを初期化した結果を保持するクラス
		////////////////////////////////////////////////////////////////////////////////
		inline Controller::Result::CInitialize InitializeController(const Controller::Setting::CInitialize& object)
		{
			Controller::Result::CInitialize ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::InitializeController()" << Logging::ConstSeparator;

			try
			{
				// コントローラを開く設定クラス
				MotionApi::Controller::Setting::COpen setting = object.Open;

				// コントローラと接続
				postscript << "MotionApi::OpenController()";
				ret.Controller = MotionApi::OpenController(setting);
				// ログ出力
				postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(ret.Controller.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// Apiタイムアウトの設定クラス
				MotionApi::Controller::Setting::CApiTimeout apiTimeout = object.ApiTimeout;

				// Apiのタイムアウトを設定
				postscript << "MotionApi::SetApiTimeout()" << Logging::ConstSeparator << "タイムアウト:" << apiTimeout.Value;
				MotionApi::SetApiTimeout(apiTimeout);
				// ログ出力
				postscript << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
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

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			コントローラを破棄
		/// @param[in]		object コントローラを破棄する設定クラス
		////////////////////////////////////////////////////////////////////////////////
		inline void DestroyController(const Controller::Setting::CDestroy& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::DestroyController()" << Logging::ConstSeparator;

			try
			{
				do
				{
					// コントローラを閉じる設定クラス
					MotionApi::Controller::Setting::CClose setting = object.Close;

					// ハンドルを確認
					if (setting.Handle == MotionApi::ConstDisconnected)
					{
						// ログ出力
						postscript << Logging::ConstSeparator << "未初期化";
						Transfer::Output(Logging::Join(logging, postscript.str()));
						postscript.str("");
						break;
					}

					// コントローラと切断
					postscript << "MotionApi::CloseController()";
					MotionApi::CloseController(setting);
					// ログ出力
					postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(setting.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
					Transfer::Output(Logging::Join(logging, postscript.str()));
					postscript.str("");
				} while (false);
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
		/// @brief			レジスタの値を取得
		/// @param[in]		object レジスタの値を取得する設定クラス
		////////////////////////////////////////////////////////////////////////////////
		inline Register::Result::CContent GetRegister(const Register::Setting::CGetContent& object)
		{
			Register::Result::CContent ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcRead;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::GetRegister()" << Logging::ConstSeparator;

			try
			{
				Register::CSpecify specify(object.Category);

				// 番号を更新
				specify.Index = object.Index;

				// データのタイプ
				auto type = specify.GetType();
				// 取得する数
				unsigned long size = object.Size;
				// アドレス
				std::string address = specify.GetAddress();

				// レジスタのデータタイプを確認
				if (Register::ConstTypeBit == type)
				{
					// ビット型 ⇒ レジスタを取得するサイズをワード型のサイズで更新
					size /= 16;
					// ※先頭が0ビット以外を考慮して2つ余分に確保
					size += 2;
				}
				else if (Register::ConstTypeLong == type)
				{
					// ロング型 ⇒ レジスタを取得するサイズをワード型のサイズで更新
					size *= 2;
				}

				// データのタイプをワード型に更新
				postscript << "Register::CSpecify::ChangeType(Register::ConstTypeWord)";
				specify.ChangeType(Register::ConstTypeWord);
				postscript.str("");

				MotionApi::Register::Setting::CDeclare declare;

				// レジスタのアドレスを更新
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

				MotionApi::Register::Setting::CGetContent setting;

				// ハンドルを更新
				setting.Handle = result.Handle;

				// データ数を更新
				setting.Size = size;

				// レジスタの値を取得
				postscript << "MotionApi::GetRegisterData()" << Logging::ConstSeparator << "ハンドル:" << FormatHandle(setting.Handle);
				postscript << Logging::ConstSeparator << "要求サイズ:" << setting.Size;
				auto content = MotionApi::GetRegisterData(setting);
				// ログ出力
				postscript << Logging::ConstSeparator << "取得サイズ:" << content.Size;
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");

				// データを走査
				for (unsigned long index = 0; index < content.Size; index++)
				{
					unsigned long target = index;

					// レジスタのデータタイプを確認
					if (Register::ConstTypeBit == type)
					{
						// ビット型 ⇒ レジスタの先頭ビット番号を取得
						auto first = std::stoi(std::string(&address.back()), nullptr, 16);
						// ※先頭のレジスタ以外は0ビット目が先頭
						auto bit = index == 0 ? first : 0;

						// 1ワード内を走査
						while (bit < 16)
						{
							// データの格納先を確定
							target = (index * 16) + bit - first;

							// 格納先を確認
							if (object.Size <= target)
							{
								// サイズを超えた ⇒ 処理を抜ける
								break;
							}

							// データを格納
							ret.Bits[target] = content.Buffers.at(index) >> bit & 0x1;

							// 次のビットへ
							bit++;
						}
					}
					else if (Register::ConstTypeWord == type)
					{
						// ワード型 ⇒ データを格納
						ret.Words[target] = content.Buffers.at(index);
					}
					else if (Register::ConstTypeLong == type)
					{
						// ロング型 ⇒ データの格納先を確定
						target = index / 2 * 2;
						// データを確認
						if (ret.Longs.count(target) <= 0)
						{
							// 未登録
							ret.Longs[target];
						}

						// 格納先を確認
						if (index % 2)
						{
							// 上位ワード ⇒ 1ワード分シフトして更新
							ret.Longs[target] |= ((unsigned long)content.Buffers.at(index)) << 16;
						}
						else
						{
							// 下位ワード
							ret.Longs[target] |= (unsigned long)content.Buffers.at(index);
						}
					}
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

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			レジスタの値を更新
		/// @param[in]		object	レジスタの値を更新する設定クラス
		////////////////////////////////////////////////////////////////////////////////
		inline void SetRegister(const Register::Setting::CSetContent& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodePlcWrite;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::SetRegister()" << Logging::ConstSeparator;

			try
			{
				Register::CSpecify specify(object.Category);

				// 番号を更新
				specify.Index = object.Index;

				// データのタイプ
				auto type = specify.GetType();
				// 更新する数
				unsigned long size = (unsigned long)object.Values.size();
				// 書き込む回数
				unsigned long loop = 1;

				// レジスタのデータタイプを確認
				if (Register::ConstTypeBit == type)
				{
					// ビット型 ⇒ 1データ毎のサイズで更新
					size = 1;
					// 繰り返し回数を確定
					loop = size;
				}
				else if (Register::ConstTypeLong == type)
				{
					// ロング型 ⇒ レジスタを更新するサイズをワード型のサイズで更新
					size *= 2;
					// データのタイプをワード型に更新
					specify.ChangeType(Register::ConstTypeWord);
				}

				for (unsigned long index = 0; index < loop; index++)
				{
					MotionApi::Register::Setting::CDeclare declare;

					// レジスタのアドレスを更新
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

					// レジスタのデータタイプを確認
					if (Register::ConstTypeBit == type)
					{
						// ビット型	⇒ レジスタのビット番号を取得
						auto first = std::stoi(std::string(&specify.GetAddress().back()), nullptr, 16);
						// ※先頭のレジスタ以外は0ビット目が先頭
						auto bit = index == 0 ? first : 0;

						// データを格納
						setting.Buffers.emplace_back(object.Values[index]);
					}
					else if (Register::ConstTypeWord == type || Register::ConstTypeLong == type)
					{
						// ワード型 or ロング型 ⇒ データを格納
						std::copy(object.Values.begin(), object.Values.end(), std::back_inserter(setting.Buffers));
					}

					// データ数を更新
					setting.Size = size;

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
		/// @brief			軸を初期化
		/// @param[in]		object 軸を初期化する設定クラス
		/// @return			軸を初期化した結果クラス
		////////////////////////////////////////////////////////////////////////////////
		inline Axis::Result::CInitialize InitializeAxis(const Axis::Setting::CInitialize& object)
		{
			Axis::Result::CInitialize ret;

			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisInitialize;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::InitializeAxis()" << Logging::ConstSeparator;

			try
			{
				// 軸のハンドルを作成する設定クラス
				MotionApi::Axis::Setting::CDeclare setting = object.Declare;

				// 軸のハンドルを作成
				postscript << "MotionApi::DeclareAxis()";
				postscript << Logging::ConstSeparator << "スロットNo:" << object.Declare.SlotNo;
				postscript << Logging::ConstSeparator << "サブスロットNo:" << object.Declare.SubSlotNo;
				postscript << Logging::ConstSeparator << "ステーションNo:" << object.Declare.StationNo;
				postscript << Logging::ConstSeparator << "論理軸No:" << object.Declare.LogicalNo;
				postscript << Logging::ConstSeparator << "軸タイプ:" << object.Declare.Type;
				ret.Axis = MotionApi::DeclareAxis(setting);
				// ログ出力
				postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(ret.Axis.Handle) << Logging::ConstSeparator << stopWatch.Format(true);
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
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

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			軸を破棄
		/// @param[in]		object 軸を破棄する設定クラス
		////////////////////////////////////////////////////////////////////////////////
		inline void DestroyAxis(const Axis::Setting::CDestroy& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisDestroy;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::DestroyAxis()" << Logging::ConstSeparator;

			try
			{
				// 軸のハンドルを破棄
				postscript << "MotionApi::ClearAllAxis()";
				MotionApi::ClearAllAxis();
				// ログ出力
				Transfer::Output(Logging::Join(logging, postscript.str()));
				postscript.str("");
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
			for (const auto& handle : object.Handles)
			{
				// ハンドルを取得
				postscript << Logging::ConstSeparator << FormatHandle(handle);
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
		/// @brief			サーボを制御
		/// @param[in]		object サーボの制御を行う設定クラス
		////////////////////////////////////////////////////////////////////////////////
		inline void AxisServo(const Device::Setting::CServoControl& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisWrite;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::AxisServo()" << Logging::ConstSeparator;

			try
			{
				// デバイスのハンドルを作成
				postscript << "DeclareDevice()";
				auto result = DeclareDevice(logging, object.Device);
				postscript.str("");

				try
				{
					// サーボの制御を行う設定クラス
					MotionApi::Device::Setting::CServo setting = object.Status;

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
		/// @brief			ジョグを実行
		/// @param[in]		object ジョグを実行する設定クラス
		////////////////////////////////////////////////////////////////////////////////
		inline void AxisStartJog(const Device::Setting::CStartJog& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisWrite;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::AxisStartJog()" << Logging::ConstSeparator;

			try
			{
				MotionApi::Device::Setting::CStartJog setting = object.Device;

				// デバイスのハンドルを作成
				postscript << "DeclareDevice()";
				auto result = DeclareDevice(logging, setting.GetDeclare());
				postscript.str("");

				try
				{
					// ハンドルを更新
					setting.Handle = result.Handle;

					// 軸を走査
					for (const auto& motion : setting.Motions)
					{
						postscript << Logging::ConstSeparator << "ハンドル:" << FormatHandle(motion.Handle);
						postscript << Logging::ConstSeparator << "座標系:" << motion.Coordinate;
						postscript << Logging::ConstSeparator << "動作タイプ:" << motion.MoveType;
						postscript << Logging::ConstSeparator << "速度タイプ:" << motion.VelocityType;
						postscript << Logging::ConstSeparator << "加減速タイプ:" << motion.AccDecType;
						postscript << Logging::ConstSeparator << "加減速フィルタタイプ:" << motion.FilterType;

						// データタイプを走査
						for (const auto& iterator : MotionApi::Device::DataIterator())
						{
							// データタイプの登録を確認
							if (motion.Datas.count(iterator) <= 0)
							{
								// データタイプが未登録 ⇒ 次のデータタイプへ
								continue;
							}

							postscript << Logging::ConstSeparator;

							switch (iterator)
							{
							case MotionApi::Device::DataMaxVelocity:
								postscript << "速度の上限値:";
								break;

							case MotionApi::Device::DataAcceleration:
								postscript << "加速度:";
								break;

							case MotionApi::Device::DataDeceleration:
								postscript << "減速度:";
								break;

							case MotionApi::Device::DataFilterTime:
								postscript << "フィルタ時間:";
								break;

							case MotionApi::Device::DataVelocity:
								postscript << "速度:";
								break;

							case MotionApi::Device::DataApproachVelocity:
								postscript << "原点復帰のアプローチ速度:";
								break;

							case MotionApi::Device::DataCreepVelocity:
								postscript << "原点復帰のクリープ速度:";
								break;
							}
							postscript << motion.Datas.at(iterator);
						}
						postscript << Logging::ConstSeparator << "方向:" << motion.Direction;
						postscript << Logging::ConstSeparator << "タイムアウト:" << motion.Timeout;
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
		inline void AxisStopJog(const Device::Setting::CStopJog& object)
		{
			Logging::CObject logging;
			std::stringstream postscript;
			Exception::EnumCode errorCode = Exception::CodeAxisWrite;
			int deviceErrorCode = Exception::DeviceCodeSuccess;
			Utility::CStopWatch stopWatch;

			logging.Message << object.Name << "Controller::AxisStopJog()" << Logging::ConstSeparator;

			try
			{
				MotionApi::Device::Setting::CStopJog setting = object.Device;

				// デバイスのハンドルを作成
				postscript << "DeclareDevice()";
				auto result = DeclareDevice(logging, setting.GetDeclare());
				postscript.str("");

				try
				{
					// ハンドルを更新
					setting.Handle = result.Handle;

					// 軸を走査
					for (const auto& motion : setting.Motions)
					{
						postscript << Logging::ConstSeparator << "軸:" << FormatHandle(motion.Handle);
						postscript << Logging::ConstSeparator << "完了:" << motion.Completed;
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