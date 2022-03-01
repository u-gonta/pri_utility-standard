#pragma once

#include "Execution.h"
#include "SettingRegister.h"
#include "Result.h"
#include "Register.h"

namespace Standard
{
	namespace Plc
	{
		namespace Register
		{
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			レジスタの値を取得
			/// @param[in]		object レジスタの値を取得する設定クラス
			////////////////////////////////////////////////////////////////////////////////
			inline Result::CContent Get(const Setting::CGetContent& object)
			{
				Result::CContent ret;

				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodePlcRead;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Get()" << Logging::ConstSeparator;

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
					//@@@Transfer::Output(Logging::Join(logging, postscript.str()));
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
					//@@@Transfer::Output(Logging::Join(logging, postscript.str()));
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
			inline void Set(const Setting::CSetContent& object)
			{
				Logging::CObject logging;
				std::stringstream postscript;
				Exception::EnumCode errorCode = Exception::CodePlcWrite;
				int deviceErrorCode = Exception::DeviceCodeSuccess;
				Utility::CStopWatch stopWatch;

				logging.Message << object.Name << "Set()" << Logging::ConstSeparator;

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
		}
	}
}