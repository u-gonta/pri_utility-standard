
// SamplePlc.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// メイン シンボル


// CSamplePlcApp:
// このクラスの実装については、SamplePlc.cpp を参照してください
//

class CSamplePlcApp : public CWinApp
{
public:
	CSamplePlcApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CSamplePlcApp theApp;
