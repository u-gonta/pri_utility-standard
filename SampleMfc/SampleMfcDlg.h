
// SampleMfcDlg.h : ヘッダー ファイル
//

#pragma once

#include "Common/Exception/Basis.h"
#include "Common/Io/Status.h"

using namespace Standard;

// CSampleMfcDlg ダイアログ
class CSampleMfcDlg : public CDialogEx
{
// コンストラクション
public:
	CSampleMfcDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SampleMfc_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

public:
	void OnTransferException(const Exception::CObject& object);
	void OnGrabberException(const Exception::CObject& object);
	void OnPlcException(const Exception::CObject& object);
	void OnIoException(const Exception::CObject& object);
	void OnAxisException(const Exception::CObject& object);

	void OnIoInput(const Io::Status::CInput& object);

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonWriteAnalogGain();
	afx_msg void OnBnClickedButtonWriteDigitalGain();
	afx_msg void OnBnClickedButtonWriteExposureTime();
	afx_msg void OnBnClickedButtonCapture();
	afx_msg void OnBnClickedButtonIoReadBit();
	afx_msg void OnBnClickedButtonIoReadIntMiddle();
	afx_msg void OnBnClickedButtonIoReadPending();
	afx_msg void OnBnClickedButtonIoWriteBit();
	afx_msg void OnBnClickedButtonIoWriteIntMiddle();
};
