
// SamplePlcDlg.h : ヘッダー ファイル
//

#pragma once

#include "Common/Exception/Basis.h"
#include "Common/Axis/Setting.h"
#include "Common/Io/Status.h"
#include "Common/Axis/Status.h"

#include "WrapperMotionApi/SettingDevice.h"

using namespace Standard;

#include "CButtonExpand.h"

#define ConstBool bool						// Bのデータ型
#define ConstWord __int16					// Wのデータ型
#define ConstLong __int32					// Lのデータ型

//@@@#define IoUse

// CSamplePlcDlg ダイアログ
class CSamplePlcDlg : public CDialogEx
{
// コンストラクション
public:
	CSamplePlcDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMPLEPLC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

public:
	void OnTransferException(const Exception::CObject& object);
	void OnIoException(const Exception::CObject& object);
	void OnIoInput(const Io::Status::CInput& object);
	void OnAxisException(const Exception::CObject& object);
	void OnAxisInput(const Axis::Status::CInput& object);

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonReady();
	afx_msg void OnBnClickedButtonOrigin();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonAlarmReset();
	afx_msg void OnBnClickedCheckEmergency();
	afx_msg void OnBnClickedButtonMoveX();
	afx_msg void OnBnClickedButtonMoveY();
	afx_msg void OnBnClickedButtonMoveXy();
	DECLARE_MESSAGE_MAP()

private:
	CRect AreaControl(int id);
	bool IsJogNegativeX(POINT target);
	bool IsJogPositiveX(POINT target);
	bool IsJogNegativeY(POINT target);
	bool IsJogPositiveY(POINT target);
	void WriteAxis();
	void HandShake(ConstWord request);
	void DrawSignal(const Io::Specify::CAddress& address, int id, COLORREF color);
	void DrawCircle(int id, COLORREF color);
	void DrawCurrent(int id);

private:
	Io::Status::CInput m_inputIo;
	Io::Status::COutput m_jogIo;
	std::map<int, Axis::Setting::CDriver> m_driverAxises;
	Axis::Status::CInput m_inputAxis;
	std::map<int, MotionApi::Device::EnumDirection> m_jogAxis;

	CButtonExpand m_buttonJogLeftTop;
	CButtonExpand m_buttonJogTop;
	CButtonExpand m_buttonJogRightTop;
	CButtonExpand m_buttonJogLeft;
	CButtonExpand m_buttonJogRight;
	CButtonExpand m_buttonJogLeftBottom;
	CButtonExpand m_buttonJogBottom;
	CButtonExpand m_buttonJogRightBottom;
};
