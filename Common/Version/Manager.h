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
		/// @brief      �o�[�W�����̏����擾����N���X
		///				�� �V���O���g��������e���v���[�g�̃N���X����p��
		////////////////////////////////////////////////////////////////////////////////
		class CManager
			: virtual public Singleton::Worker::CTemplate<CManager>
		{
		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�R���X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			CManager()
			{
				m_hModule = nullptr;
			}

		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�f�X�g���N�^
			////////////////////////////////////////////////////////////////////////////////
			virtual ~CManager()
			{
				// �j��
				Destroy();
			}

		public:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			������
			/// @param[in]		hModule	���W���[��ID
			////////////////////////////////////////////////////////////////////////////////
			void Initialize(HMODULE hModule)
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncModule);

				m_hModule = hModule;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�j��
			////////////////////////////////////////////////////////////////////////////////
			void Destroy()
			{
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncModule);

				m_hModule = nullptr;
			}

			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�o�[�W�����̃N���X���擾
			/// @return			�o�[�W�����̃N���X
			////////////////////////////////////////////////////////////////////////////////
			Version::CObject GetVersion()
			{
				Version::CObject ret;

				std::stringstream message;
					
				// �r������
				std::lock_guard<std::recursive_mutex> lock(m_asyncModule);

				TCHAR filePath[MAX_PATH] = {};

				// ���W���[��ID���m�F
				message << "HMODULE";
				if (m_hModule == nullptr)
				{
					// ��O�𔭖C
					message << Logging::ConstSeparator << "���ݒ�";
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				// �C���X�^���X�̃p�X���擾
				message.str("");
				message << "GetModuleFileName()";
				if (::GetModuleFileName(m_hModule, filePath, sizeof(filePath)) == 0)
				{
					// ��O�𔭖C
					message << Logging::ConstSeparator << "�G���[:" << Text::FormatX(8, ::GetLastError());
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				DWORD dwZero = 0;

				// �t�@�C���̃o�[�W�������T�C�Y���擾
				message.str("");
				message << "GetFileVersionInfoSize()";
				DWORD infoSize = ::GetFileVersionInfoSize(filePath, &dwZero);
				if (infoSize == 0)
				{
					// ��O�𔭖C
					message << Logging::ConstSeparator << "�T�C�Y�Ȃ�";
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				std::unique_ptr<char[]> buffer = std::make_unique<char[]>(infoSize);

				// �t�@�C���̃o�[�W���������擾
				message.str("");
				message << "GetFileVersionInfo()";
				::GetFileVersionInfo(filePath, dwZero, infoSize, buffer.get());

				VS_FIXEDFILEINFO* versionInfo;
				UINT length;

				// �o�[�W���������擾
				message.str("");
				message << "VerQueryValue()";
				if (::VerQueryValue(buffer.get(), _T("\\"), (void**)&versionInfo, &length) == 0)
				{
					// ��O�𔭖C
					message << Logging::ConstSeparator << "�G���[:" << Text::FormatX(8, ::GetLastError());
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}
				ret.Major = (int)(HIWORD(versionInfo->dwFileVersionMS));
				ret.Minor = (int)(LOWORD(versionInfo->dwFileVersionMS));
				ret.Build = (int)(HIWORD(versionInfo->dwFileVersionLS));
				ret.Revision = (int)(LOWORD(versionInfo->dwFileVersionLS));

				WORD* languageCode;

				// ���ꎯ�ʎq�ƃR�[�h�y�[�W���ʎq���擾
				message.str("");
				message << "VerQueryValue(\\VarFileInfo\\Translation)";
				if (::VerQueryValue(buffer.get(), _T("\\VarFileInfo\\Translation"), (void**)&languageCode, &length) == 0)
				{
					// ��O�𔭖C
					message << Logging::ConstSeparator << "�G���[:" << Text::FormatX(8, ::GetLastError());
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				// �t�@�C�������擾
				ret.FileName = GetResourceInfomation((void**)buffer.get(), languageCode, "OriginalFilename");

				// �t�@�C���̃o�[�W���������擾
				ret.FileDescription = GetResourceInfomation((void**)buffer.get(), languageCode, "FileDescription");

				return ret;
			}

		private:
			////////////////////////////////////////////////////////////////////////////////
			/// @brief			�o�[�W�����̏����擾
			/// @detail			���\�[�X�ɋL�ڂ��Ă����`�ς݂̕�������擾
			///					��:)FileDescription�AOriginalFilename�Ȃ�
			/// @param[in]		buffer	�o�[�W�������̃|�C���^
			/// @param[in]		languageCode	���ꎯ�ʎq�ƃR�[�h�y�[�W���ʎq
			/// @param[in]		target	��`�ς݂̕�����
			/// @return			���\�[�X�ɋL�ڂ��Ă����`�ς݂̕�����
			////////////////////////////////////////////////////////////////////////////////
			std::string GetResourceInfomation(void** buffer, WORD* languageCode, std::string target)
			{
				std::string ret = "";

				std::stringstream message;
				std::stringstream block;
				TCHAR* result;
				UINT length;

				block << "\\StringFileInfo\\" << Text::FormatX(4, languageCode[0], "") << Text::FormatX(4, languageCode[1], "") << "\\" << target;

				// �t�@�C���̃o�[�W���������擾
				message << "VerQueryValue(" << block.str() << ")";
				if (::VerQueryValue(buffer, Text::Convert(block.str()).c_str(), (void**)&result, &length) == 0)
				{
					// ��O�𔭖C
					message << Logging::ConstSeparator << "�G���[:" << Text::FormatX(8, ::GetLastError());
					throw Exception::CObject(Exception::CodeVersion, message.str());
				}

				if (0 < length)
				{
					ret = Text::Convert(result);
				}

				return ret;
			}

		private:
			//! �r������N���X(���W���[���p)
			std::recursive_mutex m_asyncModule;

			//! ���W���[��ID
			HMODULE m_hModule;
		};
	}
}