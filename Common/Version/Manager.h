#pragma once

#include <minwindef.h>
#include <mutex>

#include "Common/Exception/Basis.h"
#include "Common/Exception/Code.h"
#include "Common/Logging/Basis.h"
#include "Common/Singleton/Worker.h"
#include "Common/Tool/Text.h"
#include "Basis.h"

#pragma comment(lib,"version.lib")

namespace Standard
{
	namespace Version
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @class      CManager
		/// @brief      バージョンの情報を取得するクラス
		///				⇒ シングルトン化するテンプレートのクラスから継承
		////////////////////////////////////////////////////////////////////////////////
		class CManager
			: virtual public Singleton::Worker::CTemplate<CManager>
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			コンストラクタ
			////////////////////////////////////////////////////////////////////////////////
			CManager()
			{
				m_hModule = nullptr;
			}

		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			デストラクタ
			////////////////////////////////////////////////////////////////////////////////
			virtual ~CManager()
			{
				// 破棄
				Destroy();
			}

		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			初期化
			/// @param[in]		hModule	モジュールID
			////////////////////////////////////////////////////////////////////////////////
			void Initialize(HMODULE hModule)
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncModule);

				m_hModule = hModule;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			破棄
			////////////////////////////////////////////////////////////////////////////////
			void Destroy()
			{
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncModule);

				m_hModule = nullptr;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			バージョンのクラスを取得
			/// @return			バージョンのクラス
			////////////////////////////////////////////////////////////////////////////////
			Version::CObject GetVersion()
			{
				Version::CObject ret;

				std::stringstream message;
					
				// 排他制御
				std::lock_guard<std::recursive_mutex> lock(m_asyncModule);

				TCHAR filePath[MAX_PATH] = {};

				// モジュールIDを確認
				message << "HMODULE";
				if (m_hModule == nullptr)
				{
					// 例外を発砲
					message << Logging::ConstSeparator << "未設定";
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				// インスタンスのパスを取得
				message.str("");
				message << "GetModuleFileName()";
				if (::GetModuleFileName(m_hModule, filePath, sizeof(filePath)) == 0)
				{
					// 例外を発砲
					message << Logging::ConstSeparator << "エラー:" << Text::FormatX(8, ::GetLastError());
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				DWORD dwZero = 0;

				// ファイルのバージョン情報サイズを取得
				message.str("");
				message << "GetFileVersionInfoSize()";
				DWORD infoSize = ::GetFileVersionInfoSize(filePath, &dwZero);
				if (infoSize == 0)
				{
					// 例外を発砲
					message << Logging::ConstSeparator << "サイズなし";
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				std::unique_ptr<char[]> buffer = std::make_unique<char[]>(infoSize);

				// ファイルのバージョン情報を取得
				message.str("");
				message << "GetFileVersionInfo()";
				::GetFileVersionInfo(filePath, dwZero, infoSize, buffer.get());

				VS_FIXEDFILEINFO* versionInfo;
				UINT length;

				// バージョン情報を取得
				message.str("");
				message << "VerQueryValue()";
				if (::VerQueryValue(buffer.get(), _T("\\"), (void**)&versionInfo, &length) == 0)
				{
					// 例外を発砲
					message << Logging::ConstSeparator << "エラー:" << Text::FormatX(8, ::GetLastError());
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}
				ret.Major = (int)(HIWORD(versionInfo->dwFileVersionMS));
				ret.Minor = (int)(LOWORD(versionInfo->dwFileVersionMS));
				ret.Build = (int)(HIWORD(versionInfo->dwFileVersionLS));
				ret.Revision = (int)(LOWORD(versionInfo->dwFileVersionLS));

				WORD* languageCode;

				// 言語識別子とコードページ識別子を取得
				message.str("");
				message << "VerQueryValue(\\VarFileInfo\\Translation)";
				if (::VerQueryValue(buffer.get(), _T("\\VarFileInfo\\Translation"), (void**)&languageCode, &length) == 0)
				{
					// 例外を発砲
					message << Logging::ConstSeparator << "エラー:" << Text::FormatX(8, ::GetLastError());
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				// ファイル名を取得
				ret.FileName = GetResourceInfomation((void**)buffer.get(), languageCode, "OriginalFilename");

				// ファイルのバージョン情報を取得
				ret.FileDescription = GetResourceInfomation((void**)buffer.get(), languageCode, "FileDescription");

				return ret;
			}

		private:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			バージョンの情報を取得
			/// @detail			リソースに記載してある定義済みの文字列を取得
			///					例:)FileDescription、OriginalFilenameなど
			/// @param[in]		buffer	バージョン情報のポインタ
			/// @param[in]		languageCode	言語識別子とコードページ識別子
			/// @param[in]		target	定義済みの文字列
			/// @return			リソースに記載してある定義済みの文字列
			////////////////////////////////////////////////////////////////////////////////
			std::string GetResourceInfomation(void** buffer, WORD* languageCode, std::string target)
			{
				std::string ret = "";

				std::stringstream message;
				std::stringstream block;
				TCHAR* result;
				UINT length;

				block << "\\StringFileInfo\\" << Text::FormatX(4, languageCode[0], "") << Text::FormatX(4, languageCode[1], "") << "\\" << target;

				// ファイルのバージョン情報を取得
				message << "VerQueryValue(" << block.str() << ")";
				if (::VerQueryValue(buffer, Text::Convert(block.str()).c_str(), (void**)&result, &length) == 0)
				{
					// 例外を発砲
					message << Logging::ConstSeparator << "エラー:" << Text::FormatX(8, ::GetLastError());
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				if (0 < length)
				{
					ret = Text::Convert(result);
				}

				return ret;
			}

		private:
			//! 排他制御クラス(モジュール用)
			std::recursive_mutex m_asyncModule;

			//! モジュールID
			HMODULE m_hModule;
		};
	}
}