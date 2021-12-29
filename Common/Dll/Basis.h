#pragma once

#include "Common/Version/Manager.h"

namespace Standard
{
	namespace Dll
	{
		////////////////////////////////////////////////////////////////////////////////
		/// @brief			�A�v���P�[�V�����̃G���g���|�C���g������
		/// @param[in]		hModule	�n���h��
		/// @param[in]		ul_reason_for_call	�Ăяo��
		/// @param[in]		lpReserved	�߂�l
		////////////////////////////////////////////////////////////////////////////////
		inline void Main(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
		{
			Version::CManager& versionManager = Version::CManager::Instance();

			switch (ul_reason_for_call)
			{
			case DLL_PROCESS_ATTACH:
				// DLL���v���Z�X�̃A�h���X��ԂɃ}�b�s���O���ꂽ�B �� ���W���[��ID�ŏ�����
				versionManager.Initialize(hModule);
				break;

			case DLL_THREAD_ATTACH:
				// �X���b�h���쐬����悤�Ƃ��Ă���B
				break;

			case DLL_THREAD_DETACH:
				// �X���b�h���j������悤�Ƃ��Ă���B �� �j��
				versionManager.Destroy();
				break;

			case DLL_PROCESS_DETACH:
				// DLL�̃}�b�s���O����������悤�Ƃ��Ă���B
				break;
			}
		}
	}
}