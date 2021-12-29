#include "pch.h"

#include <mutex>

#include "Transfer.h"
#include "Common/Version/Manager.h"
#include "Common/Communication/Udp.h"

namespace Standard
{
	namespace Transfer
	{
		//! 終端コード ※ETX(テキスト終了)
		const char ConstTerminum = 0x03;

		//! 排他制御クラス(フラグ用)
		std::recursive_mutex _asyncFlag;

		//! 初期フラグ
		static bool _initialize = false;

		//! 破棄フラグ
		static bool _destroy = false;

		//! 転送を行う設定クラス
		static Setting::CObject _setting;

		//! Udp通信のクラス
		static Communication::Udp::CWorker _udp;

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			バージョンのクラスを取得
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API Version::CObject GetVersion()
		{
			Version::CObject ret;

			Version::CManager& versionManager = Version::CManager::Instance();

			// バージョンのクラスを取得
			ret = versionManager.GetVersion();

			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			例外の通知する関数を登録
		/// @detail			例外の発生時に呼び出されたい関数を指定する
		/// @param[in]		object	呼び出されたい関数
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void AttachFunctionException(std::function<void(const Exception::CObject&)> object)
		{
			// 例外の通知する関数を登録
			_udp.AttachFunctionException(object);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			例外の通知する関数を解除
		/// @detail			例外の発生時に呼び出される関数を解除する
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void DetachFunctionException()
		{
			// 例外の通知する関数を解除
			_udp.DetachFunctionException();
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			初期化
		/// @detail			転送先への接続まで行う
		/// @param[in]		object	転送を行う設定クラス
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void Initialize(const Setting::CObject& object)
		{
			// 排他制御
			std::lock_guard<std::recursive_mutex> lock(_asyncFlag);

			// 破棄フラグを確認
			if (_destroy)
			{
				// 例外を発砲
				throw Exception::CObject(Exception::CodeTransferDiscarded);
			}

			// 初期フラグを確認
			if (_initialize)
			{
				// 例外を発砲
				throw Exception::CObject(Exception::CodeTransferInitialized);
			}

			// 転送の設定クラスをコピー
			_setting = object;

			// 送信先へ接続
			_udp.AsyncConnect(_setting.IpAddress, _setting.Port);

			// 初期フラグを設定
			_initialize = true;
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			破棄
		/// @detail			転送先と切断まで行う
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void Destroy()
		{
			// 排他制御
			std::lock_guard<std::recursive_mutex> lock(_asyncFlag);

			do
			{
				// 初期フラグを確認
				if (_initialize == false)
				{
					break;
				}

				// 破棄フラグを確認
				if (_destroy)
				{
					break;
				}

				// 破棄
				_udp.Destroy();

				// 初期フラグを解除
				_initialize = false;

				// 破棄フラグを設定
				_destroy = true;
			} while (false);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			ログを転送
		/// @detail			出力する下限レベル以上のみ転送する
		///					先頭に年月日 時分秒を付加する
		/// @param[in]		object	ログのクラス
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void Output(const Logging::CObject& object)
		{
			// 排他制御
			std::lock_guard<std::recursive_mutex> lock(_asyncFlag);

			// 破棄フラグを確認
			if (_destroy)
			{
				// 例外を発砲
				throw Exception::CObject(Exception::CodeTransferDiscarded);
			}

			do
			{
				// 出力するレベルを確認
				if (object.Level < _setting.Lower)
				{
					// 下限値より低い ⇒ 転送をしない
					break;
				}

				std::stringstream message;
				SYSTEMTIME time;

				// 現在時刻を取得
				::GetLocalTime(&time);

				// 年
				message << Text::Format(4, time.wYear);
				// 月
				message << "-" << Text::Format(2, time.wMonth);
				// 日
				message << "-" << Text::Format(2, time.wDay);
				// 時
				message << "/" << Text::Format(2, time.wHour);
				// 分
				message << ":" << Text::Format(2, time.wMinute);
				// 秒
				message << ":" << Text::Format(2, time.wSecond);
				// ミリ秒
				message << "." << Text::Format(3, time.wMilliseconds);

				// レベル
				message << Logging::ConstSeparator << object.Level;

				// メッセージ
				message << Logging::ConstSeparator << object.Message.str();

				// 終端コード ※ETX(テキスト終了)
				message << (char)0x03;

				// 出力
				_udp.AsyncMessagePush(message.str());
				// デバッグ出力
				message << "\n";
				::OutputDebugString(Text::Convert(message.str()).c_str());
			} while (false);
		}

		////////////////////////////////////////////////////////////////////////////////
		/// @brief			ログを転送
		/// @param[in]		object	ログのクラス
		////////////////////////////////////////////////////////////////////////////////
		TRANSFER_API void Output(Logging::EnumLevel level, const std::string& message)
		{
			// ログを転送
			Output(Logging::CObject(level, message));
		}
	}
}