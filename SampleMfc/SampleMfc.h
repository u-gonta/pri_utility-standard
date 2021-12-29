
// SampleMfc.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// メイン シンボル


// CSampleMfcApp:
// このクラスの実装については、SampleMfc.cpp を参照してください
//

class CSampleMfcApp : public CWinApp
{
public:
	CSampleMfcApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSampleMfcApp theApp;
