
// SampleMfcDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "SampleMfc.h"
#include "SampleMfcDlg.h"
#include "afxdialogex.h"

#include <regex>

#include "boost/function.hpp"

#include "Common/Logging/Basis.h"
#include "Common/Communication/Udp.h"
#include "Common/Image/Basis.h"
#include "Common/Tool/Variant.h"
#include "Common/Version/Basis.h"
#include "Common/Timeout/Basis.h"

#include "Common/Transfer/Setting.h"
#include "Common/Axis/Setting.h"

#include "Transfer/Transfer.h"
#include "Grabber/Grabber.h"
#include "Plc/Plc.h"
#include "Io/Io.h"
#include "Axis/Axis.h"

#include "PlcMotionApi/Parameter.h"

#pragma comment(lib, "Transfer.lib")
#pragma comment(lib, "Grabber.lib")
#pragma comment(lib, "Plc.lib")
#pragma comment(lib, "Io.lib")
#pragma comment(lib, "Axis.lib")

#include "Onnx/Onnx.h"

#pragma comment(lib, "Onnx.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSampleMfcDlg ダイアログ

void CSampleMfcDlg::OnTransferException(const Exception::CObject& object)
{
	std::stringstream message;

	message << "ErrorCode:" << object.ErrorCode << "\r\n" << object.Message;
	MessageBox(NULL, Text::Convert(message.str()).c_str());
}

void CSampleMfcDlg::OnGrabberException(const Exception::CObject& object)
{
	std::stringstream message;

	message << "ErrorCode:" << object.ErrorCode << "\r\n" << object.Message;
	MessageBox(NULL, Text::Convert(message.str()).c_str());
}

void CSampleMfcDlg::OnPlcException(const Exception::CObject& object)
{
	std::stringstream message;

	message << "ErrorCode:" << object.ErrorCode << "\r\n" << object.Message;
	MessageBox(NULL, Text::Convert(message.str()).c_str());
}

void CSampleMfcDlg::OnIoException(const Exception::CObject& object)
{
	std::stringstream message;

	message << "ErrorCode:" << object.ErrorCode << "\r\n" << object.Message;
	MessageBox(NULL, Text::Convert(message.str()).c_str());
}

void CSampleMfcDlg::OnAxisException(const Exception::CObject& object)
{
	std::stringstream message;

	message << "ErrorCode:" << object.ErrorCode << "\r\n" << object.Message;
	MessageBox(NULL, Text::Convert(message.str()).c_str());
}

void CSampleMfcDlg::OnIoInput(const Io::Status::CInput& object)
{

}

CSampleMfcDlg::CSampleMfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SampleMfc_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleMfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_WRITE_ANALOG_GAIN, &CSampleMfcDlg::OnBnClickedButtonWriteAnalogGain)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_DIGITAL_GAIN, &CSampleMfcDlg::OnBnClickedButtonWriteDigitalGain)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_EXPOSURE_TIME, &CSampleMfcDlg::OnBnClickedButtonWriteExposureTime)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CSampleMfcDlg::OnBnClickedButtonCapture)
	ON_BN_CLICKED(IDC_BUTTON_IO_READ_BIT, &CSampleMfcDlg::OnBnClickedButtonIoReadBit)
	ON_BN_CLICKED(IDC_BUTTON_IO_READ_INT_MIDDLE, &CSampleMfcDlg::OnBnClickedButtonIoReadIntMiddle)
	ON_BN_CLICKED(IDC_BUTTON_IO_READ_PENDING, &CSampleMfcDlg::OnBnClickedButtonIoReadPending)
	ON_BN_CLICKED(IDC_BUTTON_IO_WRITE_BIT, &CSampleMfcDlg::OnBnClickedButtonIoWriteBit)
	ON_BN_CLICKED(IDC_BUTTON_IO_WRITE_INT_MIDDLE, &CSampleMfcDlg::OnBnClickedButtonIoWriteIntMiddle)
END_MESSAGE_MAP()


// CSampleMfcDlg メッセージ ハンドラー

BOOL CSampleMfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	Onnx::Initialize();


	CWnd *wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_INDEX_BIT);
	CString buffer;

	buffer.Format(_T("%d"), 0);
	wnd->SetWindowText(buffer);

	wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_INDEX_INT_MIDDLE);
	buffer.Format(_T("%d"), 100);
	wnd->SetWindowText(buffer);

	wnd = this->GetDlgItem(IDC_EDIT_IO_OUTPUT_INDEX_BIT);
	buffer.Format(_T("%d"), 50);
	wnd->SetWindowText(buffer);

	wnd = this->GetDlgItem(IDC_EDIT_IO_OUTPUT_INDEX_INT_MIDDLE);
	buffer.Format(_T("%d"), 200);
	wnd->SetWindowText(buffer);

	wnd = this->GetDlgItem(IDC_EDIT_IO_OUTPUT_INT_MIDDLE);
	buffer.Format(_T("%d"), 1);
	wnd->SetWindowText(buffer);

	try
	{
		// 例外を通知する関数を登録
		Transfer::AttachFunctionException(std::bind(&CSampleMfcDlg::OnTransferException, this, std::placeholders::_1));

		// ログを初期化
		Transfer::Initialize(Transfer::Setting::CObject());

		// ログを出力
		Logging::CObject logging;
		logging.Message << "OnInitDialog()";
		Transfer::Output(logging);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(message.str()).c_str());
	}

	try
	{
		// 例外を通知する関数を登録
		Grabber::AttachFunctionException(std::bind(&CSampleMfcDlg::OnGrabberException, this, std::placeholders::_1));

		// カメラを初期化
		Grabber::Setting::CDefault defaultSetting;
		defaultSetting.HeartbeatTimeout = Timeout::ConstGrabberHeatBeat;
		defaultSetting.CaptureTimeout = Timeout::ConstGrabberCapture;
		Grabber::Initialize(defaultSetting);

		// カメラに接続
		Grabber::Setting::CConnect connectSetting;
		connectSetting.Identifier = "192.168.9.232";
		Grabber::Connect(connectSetting);

		CWnd* wnd = this->GetDlgItem(IDC_EDIT_ANALOG_GAIN);

		if (wnd && wnd->m_hWnd)
		{
			std::stringstream buffer;

			// アナログゲインを読み込み
			buffer << Grabber::ReadAnalogGain();
			wnd->SetWindowText(Text::Convert(buffer.str()).c_str());
		}

		wnd = this->GetDlgItem(IDC_EDIT_DIGITAL_GAIN);

		if (wnd && wnd->m_hWnd)
		{
			std::stringstream buffer;

			// デジタルゲインを読み込み
			buffer << Grabber::ReadDigitalGain();
			wnd->SetWindowText(Text::Convert(buffer.str()).c_str());
		}

		wnd = this->GetDlgItem(IDC_EDIT_EXPOSURE_TIME);

		if (wnd && wnd->m_hWnd)
		{
			std::stringstream buffer;

			// 露光時間を読み込み
			buffer << Grabber::ReadExposureTime();
			wnd->SetWindowText(Text::Convert(buffer.str()).c_str());
		}
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	try
	{
		// 例外を通知する関数を登録
		Plc::AttachFunctionException(std::bind(&CSampleMfcDlg::OnPlcException, this, std::placeholders::_1));

		// PLCを初期化
		Plc::Setting::CDefault defaultSetting;
		//defaultSetting.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterMotionApiTimeout, Timeout::ConstMotionApi));
		Plc::Initialize(defaultSetting);

		// PLCへ接続
		Plc::Connect(Plc::Setting::CConnect());
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	try
	{
		// 例外を通知する関数を登録
		Io::AttachFunctionException(std::bind(&CSampleMfcDlg::OnIoException, this, std::placeholders::_1));

		// 入力の変化を通知する関数を登録
		Io::AttachFunctionInput(std::bind(&CSampleMfcDlg::OnIoInput, this, std::placeholders::_1));

		Io::Setting::CDefault settingDefault;

		settingDefault.Cycle = 5000;

		// IOを初期化
		Io::Initialize(settingDefault);

		// IOに接続
		Io::Setting::CConnect connectSetting;
		Io::Setting::CAddressRange addressRange;

		//↓デフォルト
		addressRange.Start.Index = 0;
		addressRange.Size = 10;
		connectSetting.AddressRanges.emplace_back(addressRange);
		//↑

		Io::Connect(connectSetting);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	try
	{
		//// 例外を通知する関数を登録
		//Axis::AttachFunctionException(std::bind(&CSampleMfcDlg::OnAxisException, this, std::placeholders::_1));

		//// 軸を初期化
		//Axis::Initialize(Axis::Setting::CDefault());

		//// 軸に接続
		//Axis::Setting::CConnect connectSetting;
		//connectSetting.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterDeviceType, MotionApi::DevicePciE));
		//connectSetting.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterMotionApiTimeout, Timeout::ConstMotionApi));
		//for (int index = 0; index < 2; index++)
		//{
		//	Axis::Setting::CDriver buffer;

		//	switch (index)
		//	{
		//	case 0:
		//		buffer.Index = (int)Plc::StationNo01;
		//		break;

		//	case 1:
		//		buffer.Index = (int)Plc::StationNo05;
		//		break;
		//	}
		//	buffer.Other.Objects.emplace_back(Variant::CObject((Plc::EnumParameter)(Plc::ParameterAxisType01 + buffer.Index - Plc::StationNo01), MotionApi::AxisReal));

		//	connectSetting.Drivers[index] = buffer;
		//}
		//Axis::Connect(connectSetting);

		//// サーボをON
		//Axis::Setting::CServo servoSetting;
		//servoSetting.Mode = Signal::StatusOn;
		//servoSetting.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterServeTimeout, Timeout::ConstMotionServo));
		//for (const auto& driver : connectSetting.Drivers)
		//{
		//	servoSetting.Drivers[driver.first] = Axis::Setting::CDriver();
		//}
		//Axis::Servo(servoSetting);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CSampleMfcDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	try
	{
		//// 軸を破棄
		//Axis::Destroy();

		//// 例外を通知する関数を解除
		//Axis::DetachFunctionException();
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	try
	{
		// IOを破棄
		Io::Destroy();

		// 例外を通知する関数を解除
		Io::DetachFunctionException();
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	try
	{
		// PLCを破棄
		Plc::Destroy();

		// 例外を通知する関数を解除
		Plc::DetachFunctionException();
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	try
	{
		// カメラを破棄
		Grabber::Destroy();

		// 例外を通知する関数を解除
		Grabber::DetachFunctionException();
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	// ログを出力
	Logging::CObject logging;
	logging.Message << "OnDestroy()";
	Transfer::Output(logging);
}

void CSampleMfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CSampleMfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CSampleMfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSampleMfcDlg::OnBnClickedButtonWriteAnalogGain()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		CWnd* wnd = this->GetDlgItem(IDC_EDIT_ANALOG_GAIN);

		if (wnd && wnd->m_hWnd)
		{
			CString buffer;

			wnd->GetWindowText(buffer);

			// アナログゲインを書き込み
			Grabber::WriteAnalogGain(::atof(Text::Convert(buffer.GetBuffer(0)).c_str()));
		}
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}


void CSampleMfcDlg::OnBnClickedButtonWriteDigitalGain()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		CWnd* wnd = this->GetDlgItem(IDC_EDIT_DIGITAL_GAIN);

		if (wnd && wnd->m_hWnd)
		{
			CString buffer;

			wnd->GetWindowText(buffer);

			// デジタルゲインを書き込み
			Grabber::WriteAnalogGain(::atof(Text::Convert(buffer.GetBuffer(0)).c_str()));
		}
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}


void CSampleMfcDlg::OnBnClickedButtonWriteExposureTime()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		CWnd* wnd = this->GetDlgItem(IDC_EDIT_EXPOSURE_TIME);

		if (wnd && wnd->m_hWnd)
		{
			CString buffer;

			wnd->GetWindowText(buffer);

			// 露光時間を書き込み
			Grabber::WriteExposureTime(::atof(Text::Convert(buffer.GetBuffer(0)).c_str()));
		}
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}


void CSampleMfcDlg::OnBnClickedButtonCapture()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		Image::CObject image;

		// 取り込み
		image = Grabber::Capture();
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}


void CSampleMfcDlg::OnBnClickedButtonIoReadBit()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		Io::Setting::CInput setting;
		Io::Specify::CAddress address;

		CWnd *wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_CATEGORY_BIT);
		CString buffer;

		wnd->GetWindowText(buffer);
		address.Category = Text::Convert(buffer.GetBuffer(0)).c_str();

		wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_INDEX_BIT);
		wnd->GetWindowText(buffer);
		address.Index = ::_ttoi(buffer);

		setting.Addresses.emplace_back(address);

		Io::Status::CInput input = Io::ReadInput(setting);

		CButton *button = (CButton*)this->GetDlgItem(IDC_CHECK_IO_INPUT_BIT);
		auto status = BST_UNCHECKED;

		if (Variant::Convert<bool>(input.Values.at(address)))
		{
			status = BST_CHECKED;
		}

		button->SetCheck(status);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}


void CSampleMfcDlg::OnBnClickedButtonIoReadIntMiddle()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		Io::Setting::CInput setting;
		Io::Specify::CAddress address;

		CString buffer;

		CWnd *wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_CATEGORY_INT_MIDDLE);
		wnd->GetWindowText(buffer);
		address.Category = Text::Convert(buffer.GetBuffer(0)).c_str();

		wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_INDEX_INT_MIDDLE);
		wnd->GetWindowText(buffer);
		address.Index = ::_ttoi(buffer);

		setting.Addresses.emplace_back(address);

		Io::Status::CInput input = Io::ReadInput(setting);

		buffer.Format(_T("%d"), Variant::Convert<__int16>(input.Values[address]));

		wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_INT_MIDDLE);
		wnd->SetWindowText(buffer);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}


void CSampleMfcDlg::OnBnClickedButtonIoReadPending()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		Io::Setting::CInputPending setting;

		CString buffer;

		CWnd *wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_CATEGORY_BIT);
		wnd->GetWindowText(buffer);
		setting.Trigger.Address.Category = Text::Convert(buffer.GetBuffer(0)).c_str();

		wnd = this->GetDlgItem(IDC_EDIT_IO_INPUT_INDEX_BIT);
		wnd->GetWindowText(buffer);
		setting.Trigger.Address.Index = ::_ttoi(buffer);

		CButton *button = (CButton*)this->GetDlgItem(IDC_CHECK_IO_INPUT_TRIGGER);
		if (button->GetCheck() == BST_CHECKED)
		{
			setting.Trigger.Direction = Io::Status::DirectionRise;
		}
		else
		{
			setting.Trigger.Direction = Io::Status::DirectionFall;
		}
		setting.Timeout = 6000;

		Io::TriggerInputPending(setting);

		std::stringstream message;

		MessageBox(NULL, Text::Convert("トリガー発生").c_str());
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}


void CSampleMfcDlg::OnBnClickedButtonIoWriteBit()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		Io::Status::COutput output;
		Io::Status::CEntity bit;

		CString buffer;

		CWnd *wnd = this->GetDlgItem(IDC_EDIT_IO_OUTPUT_CATEGORY_BIT);
		wnd->GetWindowText(buffer);
		bit.Category = Text::Convert(buffer.GetBuffer(0)).c_str();

		wnd = this->GetDlgItem(IDC_EDIT_IO_OUTPUT_INDEX_BIT);
		wnd->GetWindowText(buffer);
		bit.Index = ::_ttoi(buffer);

		CButton *button = (CButton*)this->GetDlgItem(IDC_CHECK_IO_OUTPUT_BIT);
		bit.Value = true;
		if (button->GetCheck() == BST_UNCHECKED)
		{
			bit.Value = false;
		}

		output.Entitys.emplace_back(bit);

		Io::WriteOutput(output);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}


void CSampleMfcDlg::OnBnClickedButtonIoWriteIntMiddle()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		Io::Status::COutput output;
		Io::Status::CEntity intMiddle;

		CString buffer;

		CWnd *wnd = this->GetDlgItem(IDC_EDIT_IO_OUTPUT_CATEGORY_INT_MIDDLE);
		wnd->GetWindowText(buffer);
		intMiddle.Category = Text::Convert(buffer.GetBuffer(0)).c_str();

		wnd = this->GetDlgItem(IDC_EDIT_IO_OUTPUT_INDEX_INT_MIDDLE);
		wnd->GetWindowText(buffer);
		intMiddle.Index = ::_ttoi(buffer);

		wnd = this->GetDlgItem(IDC_EDIT_IO_OUTPUT_INT_MIDDLE);
		wnd->GetWindowText(buffer);
		intMiddle.Value = (__int32)::_ttoi(buffer);

		output.Entitys.emplace_back(intMiddle);

		Io::WriteOutput(output);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(NULL, Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}
