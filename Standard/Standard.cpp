#include "Standard.h"

#include "Common/Dll/Basis.h"
#include "Common/Version/Manager.h"

namespace Standard
{
	////////////////////////////////////////////////////////////////////////////////
	/// @brief			�A�v���P�[�V�����̃G���g���|�C���g������
	/// @param[in]		hModule	�n���h��
	/// @param[in]		ul_reason_for_call	�Ăяo��
	/// @param[in]		lpReserved	�߂�l
	////////////////////////////////////////////////////////////////////////////////
	STANDARD_API void EntryPoint(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
	{
		// �A�v���P�[�V�����̃G���g���|�C���g������
		Dll::Main(hModule, ul_reason_for_call, lpReserved);
	}

	////////////////////////////////////////////////////////////////////////////////
	/// @brief			�o�[�W�����̃N���X���擾
	////////////////////////////////////////////////////////////////////////////////
	STANDARD_API Version::CObject GetVersion()
	{
		Version::CObject ret;

		Version::CManager& versionManager = Version::CManager::Instance();

		// �o�[�W�����̃N���X���擾
		ret = versionManager.GetVersion();

		return ret;
	}
}