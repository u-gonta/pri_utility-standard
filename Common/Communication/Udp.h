#pragma once

#include <string>
#include <deque>
#include <functional>

#include "boost/function.hpp"

#include "Common/Exception/Basis.h"
#include "Common/Logging/Basis.h"
#include "Common/Tool/Text.h"
#include "Common/Tool/Exception.h"
#include "Common/Utility/SequentialAsync.h"

namespace Standard
{
	namespace Communication
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			Shift-JIS → UTF8へ変換 ※インライン関数
		/// @param[in]		value	元データ
		/// @return         UTF8に変換された文字列
		////////////////////////////////////////////////////////////////////////////////
		inline std::string ShiftJisToUtf8(const std::string& value)
		{
			std::string ret = "";

			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

			// Shift-JIS → UTF8へ変換
			ret = converter.to_bytes(Text::Convert(value));

			return ret;
		}

		namespace Udp
		{
			using namespace Standard;

			////////////////////////////////////////////////////////////////////////////////
			/// @class      CWorker
			/// @brief      UDP通信を行うクラス
			/// @detail		送信データの登録、非同期で送信を行う
			////////////////////////////////////////////////////////////////////////////////
			class CWorker
			{
			public:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			コンストラクタ
				////////////////////////////////////////////////////////////////////////////////
				CWorker() :
					  m_sequentialAsync(m_io_service, 1)
					, m_strand(m_io_service)
					, m_socket(m_io_service)
				{
					_FunctionException = nullptr;
					m_buffers.clear();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			デストラクタ
				////////////////////////////////////////////////////////////////////////////////
				~CWorker()
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << "Communication::Udp::~CWorker()" << Logging::ConstSeparator;

					try
					{
						// 破棄
						postscript << "Destroy()";
						Destroy();
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ デバッグ出力
						Exception::OutputDebug(logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			破棄
				////////////////////////////////////////////////////////////////////////////////
				void Destroy()
				{
					// 順次非同期処理を停止
					m_sequentialAsync.Stop();
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外を通知する関数の登録
				/// @detail			例外の発生時に呼び出されたい関数を指定する
				/// @param[in]		object	呼び出されたい関数
				////////////////////////////////////////////////////////////////////////////////
				void AttachFunctionException(std::function<void(const Exception::CObject&)> object)
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncException);

					// 例外の通知を登録
					_FunctionException = object;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外を通知する関数の解除
				/// @detail			例外の発生時に呼び出される関数を解除する
				////////////////////////////////////////////////////////////////////////////////
				void DetachFunctionException()
				{
					// 排他制御
					std::lock_guard<std::recursive_mutex> lock(m_asyncException);

					// 例外の通知を解除
					_FunctionException = nullptr;
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			送信先へ接続
				/// @detail			接続する関数を非同期で呼び出す
				/// @param[in]		address	IPアドレス
				/// @param[in]		port	ポート(接続先)
				////////////////////////////////////////////////////////////////////////////////
				void AsyncConnect(const std::string& address, unsigned short port)
				{
					// 接続
					m_strand.post(boost::bind(&CWorker::Connect, this, address, port));
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			送信データを追加
				/// @detail			追加する関数を非同期で呼び出す
				/// @param[in]		value	メッセージ
				////////////////////////////////////////////////////////////////////////////////
				void AsyncMessagePush(const std::string& value)
				{
					// 送信データを追加
					m_strand.post(boost::bind(&CWorker::MessagePush, this, value));
				}

			private:
				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外の通知を転送
				/// @detail			通知の転送は非同期の関数を使用する事
				/// @param[in]		object	例外のクラス
				////////////////////////////////////////////////////////////////////////////////
				void TransferException(const Exception::CObject& object)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << "Communication::CUdp::TransferException()" << Logging::ConstSeparator;

					try
					{
						// 排他制御
						std::lock_guard<std::recursive_mutex> lock(m_asyncException);

						postscript << "_FunctionException()";
						if (_FunctionException != nullptr)
						{
							// 例外を通知する関数が有効 ⇒ 例外を通知
							_FunctionException(object);
						}
						postscript.str("");
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ デバッグ出力
						Exception::OutputDebug(logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			例外の通知を転送
				/// @param[in]		errorCode	エラーコード
				/// @param[in]		logging	ログのクラス
				/// @param[in]		postscript	追加の文字列
				/// @param[in]		e	例外
				////////////////////////////////////////////////////////////////////////////////
				void TransferException(Exception::EnumCode errorCode, const Logging::CObject& logging, const std::string& postscript, const std::exception& e)
				{
					Exception::CObject object;

					std::stringstream message;

					object.ErrorCode = errorCode;
					message << logging.Message.str() << postscript << Logging::ConstSeparator << e.what();
					object.Message = message.str();

					// 例外の通知を転送
					TransferException(object);
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			送信先へ接続
				/// @detail			送信先への接続は非同期の関数を使用する事
				/// @param[in]		address		IPアドレス
				/// @param[in]		port	ポート(接続先)
				////////////////////////////////////////////////////////////////////////////////
				void Connect(const std::string& address, unsigned short port)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << "Communication::CUdp::AsyncConnect()" << Logging::ConstSeparator;

					try
					{
						// 接続を確認
						postscript << "m_socket.is_open()";
						if (m_socket.is_open())
						{
							postscript.str("");
							postscript << "m_socket.close()";
							m_socket.close();
						}
						postscript.str("");

						boost::system::error_code error;

						// 接続
						postscript << "m_socket.connet()";
						m_socket.connect(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(address), port), error);

						// エラーを確認
						if (error)
						{
							std::stringstream message;
							message << "Error:" << error;

							// 例外を発砲
							throw std::runtime_error(message.str());
						}
						postscript.str("");
					}
					catch (const std::exception & e)
					{
						// 例外の処理 ⇒ 例外を転送
						TransferException(Exception::CodeUdpConnect, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			送信データを追加
				/// @detail			送信データの追加は非同期の関数を使用する事
				/// @param[in]		value	メッセージ
				////////////////////////////////////////////////////////////////////////////////
				void MessagePush(const std::string& value)
				{
					Logging::CObject logging;
					std::stringstream postscript;

					logging.Message << "Communication::CUdp::AsyncPush()" << Logging::ConstSeparator;

					try
					{
						// 送信データを分割する数を計算
						postscript << "size()";
						size_t size = m_buffers.size();
						size_t block = 10000;
						size_t split = value.size() / block;
						size_t remainder = value.size() % block;
						size_t last = 0;
						postscript.str("");

						// 送信データを分割して走査
						postscript << "m_buffers.push_back()";
						for (size_t index = 0; index < split; index++)
						{
							// 送信データを分割してバッファへ追加
							m_buffers.push_back(value.substr(last, block));
							last += block;
						}

						// 送信データの余りをバッファに格納
						if (remainder)
						{
							// バッファーへ追加
							m_buffers.push_back(value.substr(last, remainder));
						}
						postscript.str("");

						do
						{
							// 未送信のバッファーがあるか確認
							if (0 < size)
							{
								break;
							}

							// 送信の手続き
							postscript << "AsyncWriteProdure()";
							AsyncWriteProcedure();
							postscript.str("");
						} while (false);
					}
					catch (const std::exception& e)
					{
						// 例外の処理 ⇒ 例外を転送
						TransferException(Exception::CodeUdpWrite, logging, postscript.str(), e);
					}
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			送信の手続き
				/// @detail			送信の手続きを非同期で呼び出す
				////////////////////////////////////////////////////////////////////////////////
				void AsyncWriteProcedure()
				{
					// 先頭のバッファを取得
					const std::string& value = ShiftJisToUtf8(m_buffers.front());

					// 非同期で送信
					m_socket.async_send(boost::asio::buffer(value.c_str(), value.size()), m_strand.wrap(boost::bind(&CWorker::OnWriteCompleted, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
				}

				////////////////////////////////////////////////////////////////////////////////
				/// @brief			送信後のイベント
				/// @detail			送信が完了したタイミングで発生する
				/// @param[in]		error	エラー
				/// @param[in]		size	送信サイズ
				////////////////////////////////////////////////////////////////////////////////
				void OnWriteCompleted(const boost::system::error_code& error, const size_t size)
				{
					Logging::CObject logging;

					logging.Message << "Communication::CUdp::OnWriteCompleted()" << Logging::ConstSeparator;

					// バッファーの先頭を削除
					m_buffers.pop_front();

					do
					{
						// エラーを確認
						if (error)
						{
							logging.Level = Logging::LevelError;
							logging.Message << "Error:" << error;

							// 例外を転送
							TransferException(Exception::CObject(Exception::CodeUdpWrite, logging.Message.str()));
							break;
						}

						// 未送信のバッファーがあるか確認
						if (m_buffers.empty())
						{
							break;
						}

						// 送信の手続き
						AsyncWriteProcedure();
					} while (false);
				}

			private:
				//! 排他制御クラス(例外を通知する関数用)
				std::recursive_mutex m_asyncException;

				//! 例外を通知する関数
				std::function<void(const Exception::CObject&)> _FunctionException;

				//! IOサービス
				boost::asio::io_service m_io_service;

				//! 順次非同期処理のクラス
				Utility::CSequentialAsync m_sequentialAsync;

				//! 順次排他制御のクラス
				boost::asio::io_service::strand m_strand;

				//! ソケットのクラス
				boost::asio::ip::udp::socket m_socket;

				//! 送信先のIPアドレス
				std::string m_address;

				//! 送信文字列のバッファ
				std::deque<std::string> m_buffers;
			};
		}
	}
}