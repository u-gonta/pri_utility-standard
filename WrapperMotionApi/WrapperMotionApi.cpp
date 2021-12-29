#include "pch.h"

#include <sstream>

#include "Common/Tool/Text.h"

#include "Exception.h"
#include "WrapperMotionApi.h"

#include "ymcPCAPI.h"

#if defined(_WIN64)
#pragma comment(lib, "ymcPCAPI_x64.lib")
#else
#pragma comment(lib, "ymcPCAPI.lib")
#endif

namespace Standard
{
	namespace MotionApi
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			コントローラに接続
		/// @param[in]		object	コントローラを開く設定のクラス
		/// @return			コントローラを開いた結果クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Controller::Result::COpen OpenController(const Controller::Setting::COpen& object)
		{
			Controller::Result::COpen ret;
			std::stringstream postscript;

			// COM_DEVICE構造体
			COM_DEVICE device;

			// 構造体を初期化
			::memset(&device, 0, sizeof(COM_DEVICE));

			// 通信種別
			device.ComDeviceType = (WORD)object.Device;

			// ポート番号
			device.PortNumber = 1;

			// CPU番号
			device.CpuNumber = 1;

			// 通信タイムアウト
			device.Timeout = 10000;

			HCONTROLLER result;

			// コントローラと接続
			postscript << "ymcOpenController()";
			result = ymcOpenController(&device, &ret.Handle);

			// 接続を確認
			if (result != MP_SUCCESS)
			{
				// 接続に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			Apiのタイムアウトを設定
		/// @param[in]		object	Apiのタイムアウト設定のクラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void SetApiTimeout(const Controller::Setting::CApiTimeout& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// モーションApiのタイムアウトを設定
			postscript << "ymcSetAPITimeoutValue()";
			result = ymcSetAPITimeoutValue((LONG)object.Value);

			// 設定を確認
			if (result != MP_SUCCESS)
			{
				// 設定に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			コントローラと切断
		/// @param[in]		object	コントローラを閉じる設定クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void CloseController(const Controller::Setting::CClose& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// コントローラと切断
			postscript << "ymcCloseController()";
			result = ymcCloseController(object.Handle);

			// 切断を確認
			if (result != MP_SUCCESS)
			{
				// 切断に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			レジスタのハンドルを作成
		/// @param[in]		object	レジスタのハンドルを作成する設定のクラス
		/// @return			レジスタのハンドルを作成した結果クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Register::Result::CDeclare DeclareRegister(const Register::Setting::CDeclare& object)
		{
			Register::Result::CDeclare ret;
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// レジスタのハンドルを取得
			postscript << "ymcGetRegisterDataHandle()";
			result = ymcGetRegisterDataHandle((unsigned char *)object.Address.c_str(), &ret.Handle);

			// 取得を確認
			if (result != MP_SUCCESS)
			{
				// 取得に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			レジスタの値を取得
		/// @param[in]		object	レジスタの値を取得する設定クラス
		/// @return			レジスタの値クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Register::Result::CContent GetRegisterData(const Register::Setting::CGetContent& object)
		{
			Register::Result::CContent ret;
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// バッファ数を更新
			ret.Buffers.resize(object.Size);

			// レジスタの値を取得
			postscript << "ymcGetRegisterData()";
			result = ymcGetRegisterData(object.Handle, object.Size, ret.Buffers.data(), &ret.Size);

			// 取得を確認
			if (result != MP_SUCCESS)
			{
				// 取得に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			レジスタの値を更新
		/// @param[in]		object	レジスタの値を更新する設定クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void SetRegisterData(const Register::Setting::CSetContent& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// レジスタの値を更新
			postscript << "ymcSetRegisterData()";
			result = ymcSetRegisterData(object.Handle, object.Size, (void*)object.Buffers.data());

			// 取得を確認
			if (result != MP_SUCCESS)
			{
				// 取得に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			軸のハンドルを消去
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void ClearAllAxis()
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// 軸のハンドルを消去
			postscript << "ymcClearAllAxes()";
			result = ymcClearAllAxes();

			// 消去を確認
			if (result != MP_SUCCESS)
			{
				// 消去に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			軸のハンドルを作成
		/// @param[in]		object	軸のハンドルを作成する設定のクラス
		/// @return			軸のハンドルを作成した結果クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Axis::Result::CDeclare DeclareAxis(const Axis::Setting::CDeclare& object)
		{
			Axis::Result::CDeclare ret;
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;
			std::stringstream name;

			name << "Axis" << Text::Format(2, object.LogicalNo);

			// 軸のハンドルを取得
			postscript << "ymcDeclareAxis()";
			result = ymcDeclareAxis(1, object.SlotNo, object.SubSlotNo, object.StationNo, object.LogicalNo, (WORD)object.Type, (LPBYTE)name.str().c_str(), &ret.Handle);

			// 取得を確認
			if (result != MP_SUCCESS)
			{
				// 取得に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			軸のハンドルをクリア
		/// @param[in]		object	軸のハンドルを破棄する設定のクラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void ClearAxis(const Axis::Setting::CClear& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// 軸のハンドルをクリア
			postscript << "ymcClearAxis()";
			result = ymcClearAxis(object.Handle);

			// 破棄を確認
			if (result != MP_SUCCESS)
			{
				// 破棄に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			デバイスのハンドルを作成
		/// @param[in]		object	デバイスのハンドルを作成する設定のクラス
		/// @return			デバイスのハンドルを作成した結果クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API Device::Result::CDeclare DeclareDevice(const Device::Setting::CDeclare& object)
		{
			Device::Result::CDeclare ret;
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			std::vector<HAXIS> axises;

			// 軸を走査
			for (const auto& handle : object.Handles)
			{
				// 軸のハンドルを追加
				axises.emplace_back(handle);
			}

			// デバイスのハンドルを取得
			postscript << "ymcDeclareDevice()";
			result = ymcDeclareDevice((WORD)axises.size(), axises.data(), &ret.Handle);

			// 取得を確認
			if (result != MP_SUCCESS)
			{
				// 取得に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			デバイスのハンドルをクリア
		/// @param[in]		object	デバイスのハンドルを破棄する設定のクラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void ClearDevice(const Device::Setting::CClear& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// デバイスのハンドルをクリア
			postscript << "ymcClearDevice()";
			result = ymcClearDevice(object.Handle);

			// 破棄を確認
			if (result != MP_SUCCESS)
			{
				// 破棄に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			サーボを制御
		/// @param[in]		object サーボの制御を行う設定クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void ServoControl(const Device::Setting::CServo& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			// サーボを制御
			postscript << "ymcServoControl()";
			result = ymcServoControl(object.Handle, object.Mode == Signal::StatusOn ? SERVO_ON : SERVO_OFF, object.Timeout);

			// 制御を確認
			if (result != MP_SUCCESS)
			{
				// 制御に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			軸の制御を行うテンプレートの設定クラスからライブラリへ渡す変数へ変換
		/// @param[in]		object 軸の制御を行うテンプレートの設定クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_DATA Convert(const Device::Setting::Motion::CTemplate& object)
		{
			MOTION_DATA ret;

			//! 座標系
			ret.CoordinateSystem = (WORD)object.Coordinate;

			//! 動作タイプ
			ret.MoveType = (SHORT)object.MoveType;

			//! 速度タイプ
			ret.VelocityType = (SHORT)object.VelocityType;

			//! 加減速タイプ
			ret.AccDecType = (SHORT)object.AccDecType;

			//! 加減速フィルタタイプ
			ret.FilterType = (SHORT)object.FilterType;

			// データフラグ
			ret.DataType = 0;

			// データフラグ
			for (const auto& iterator : Device::DataIterator())
			{
				// データタイプの登録を確認
				if (object.Datas.count(iterator) <= 0)
				{
					// データタイプが未登録 ⇒ 次のデータタイプへ
					continue;
				}

				// ビットをON
				ret.DataType |= 0x01 << iterator;
				switch (iterator)
				{
				case Device::DataMaxVelocity:
					// 速度の上限値、送り最高速度
					ret.MaxVelocity = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataAcceleration:
					// 加速度[指令単位/sec^2]／加速時定数[ms]
					ret.Acceleration = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataDeceleration:
					// 減速度[指令単位/sec^2]／減速時定数[ms]
					ret.Deceleration = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataFilterTime:
					// EnumFilterがFilterCurveまたはFilterExpを選択時に有効
					ret.FilterTime = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataVelocity:
					// 速度
					ret.Velocity = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataApproachVelocity:
					// 原点復帰のアプローチ速度
					ret.ApproachVelocity = (LONG)object.Datas.at(iterator);
					break;

				case Device::DataCreepVelocity:
					// 原点復帰のクリープ速度
					ret.CreepVelocity = (LONG)object.Datas.at(iterator);
					break;
				}
			}

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			軸のジョグ送りを実行
		/// @param[in]		object 軸のジョグを実行する設定クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void StartJog(const Device::Setting::CStartJog& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			std::vector<MOTION_DATA> datas;
			std::vector<WORD> directions;
			std::vector<WORD> timeouts;

			// 軸を走査
			for (const auto& motion : object.Motions)
			{
				// 軸の制御を行う状態を保持するクラスからライブラリへ渡す変数へ変換
				datas.emplace_back(Convert(motion));

				// 方向を追加
				directions.emplace_back((WORD)motion.Direction);

				// タイムアウトを追加
				timeouts.emplace_back((WORD)motion.Timeout);
			}

			// 軸のジョグ送りを実行
			postscript << "ymcMoveJOG()";
			result = ymcMoveJOG(object.Handle, datas.data(), directions.data(), timeouts.data(), 0, NULL, 0);

			// 制御を確認
			if (result != MP_SUCCESS)
			{
				// 制御に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			軸のジョグ送りを停止
		/// @param[in]		object 軸のジョグを停止する設定クラス
		////////////////////////////////////////////////////////////////////////////////
		MOTION_API void StopJog(const Device::Setting::CStopJog& object)
		{
			unsigned long result = MP_SUCCESS;
			std::stringstream postscript;

			std::vector<WORD> completeds;

			// 軸を走査
			for (const auto& motion : object.Motions)
			{
				// 完了タイプ
				completeds.emplace_back((WORD)motion.Completed);
			}

			// 軸のジョグ送りを停止
			postscript << "ymcStopJOG()";
			result = ymcStopJOG(object.Handle, 0, NULL, completeds.data(), 0);

			// 制御を確認
			if (result != MP_SUCCESS)
			{
				// 制御に失敗 ⇒ 例外を発砲
				throw CException(result, postscript.str());
			}
		}
	}
}