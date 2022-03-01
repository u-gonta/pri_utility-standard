
// SamplePlcDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "SamplePlc.h"
#include "SamplePlcDlg.h"
#include "afxdialogex.h"

#include <regex>

#include "Common/Logging/Basis.h"
#include "Common/Transfer/Setting.h"
#include "Common/Tool/Text.h"
#include "Common/Tool/Variant.h"
#include "Common/Timeout/Basis.h"

#include "Transfer/Transfer.h"
#include "Io/Io.h"
#include "Axis/Axis.h"

#include "WrapperMotionApi/Status.h"
#include "PlcMotionApi/Register.h"
#include "PlcMotionApi/SettingController.h"
#include "PlcMotionApi/SettingRegister.h"
#include "PlcMotionApi/SettingAxis.h"
#include "PlcMotionApi/SettingDevice.h"
#include "PlcMotionApi/Parameter.h"

#pragma comment(lib, "Transfer.lib")
#pragma comment(lib, "Io.lib")
#pragma comment(lib, "Axis.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum EnumAxis
{
	AxisUnknown,
	AxisX,
	AxisY
};

enum EnumJogIo
{
	JogIoNegativeX = 0x100011,	// -ジョグ移動X軸
	JogIoPositiveX = 0x100012,	// +ジョグ移動X軸
	JogIoNegativeY = 0x100013,	// -ジョグ移動Y軸
	JogIoPositiveY = 0x100014	// +ジョグ移動Y軸
};

// CSamplePlcDlg ダイアログ
void CSamplePlcDlg::OnTransferException(const Exception::CObject& object)
{
	std::stringstream message;

	message << "TransferError" << Logging::ConstSeparator << "Code:" << object.ErrorCode << "\r\n" << object.Message;
	MessageBox(NULL, Text::Convert(message.str()).c_str());
}

void CSamplePlcDlg::OnIoException(const Exception::CObject& object)
{
	std::stringstream message;

	message << "IoError" << Logging::ConstSeparator << "Code:" << object.ErrorCode << "\r\n" << object.Message;
	MessageBox(NULL, Text::Convert(message.str()).c_str());
}

void CSamplePlcDlg::OnAxisException(const Exception::CObject& object)
{
	std::stringstream message;

	message << "AxisError" << Logging::ConstSeparator << "Code:" << object.ErrorCode << "\r\n" << object.Message;
	MessageBox(NULL, Text::Convert(message.str()).c_str());
}

void CSamplePlcDlg::OnIoInput(const Io::Status::CInput& object)
{
	// 入力をコピー
	m_inputIo += object;

	// 異常を確認
	Io::Specify::CAddress abnormal;

	abnormal.Category = Plc::Register::ConstCategoryM;
	abnormal.Category += Plc::Register::ConstTypeBit;
	abnormal.Index = 0x10;

	if (0 < m_inputIo.Values.count(abnormal))
	{
		if (Variant::Convert<ConstBool>(m_inputIo.Values.at(abnormal)))
		{
			for (auto entity : m_jogIo.Entitys)
			{
				if (entity.Value.type() == typeid(bool))
				{
					entity.Value = false;
				}
			}
		}
	}

	// 描画を更新
	this->Invalidate();
}

void CSamplePlcDlg::OnAxisInput(const Axis::Status::CInput& object)
{
	// 入力をコピー
	m_inputAxis += object;

	// 描画を更新
	this->Invalidate();
}

CSamplePlcDlg::CSamplePlcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAMPLEPLC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSamplePlcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_JOG_LEFT_TOP, m_buttonJogLeftTop);
	DDX_Control(pDX, IDC_BUTTON_JOG_TOP, m_buttonJogTop);
	DDX_Control(pDX, IDC_BUTTON_JOG_RIGHT_TOP, m_buttonJogRightTop);
	DDX_Control(pDX, IDC_BUTTON_JOG_LEFT, m_buttonJogLeft);
	DDX_Control(pDX, IDC_BUTTON_JOG_RIGHT, m_buttonJogRight);
	DDX_Control(pDX, IDC_BUTTON_JOG_LEFT_BOTTOM, m_buttonJogLeftBottom);
	DDX_Control(pDX, IDC_BUTTON_JOG_BOTTOM, m_buttonJogBottom);
	DDX_Control(pDX, IDC_BUTTON_JOG_RIGHT_BOTTOM, m_buttonJogRightBottom);
}

BEGIN_MESSAGE_MAP(CSamplePlcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CHECK_EMERGENCY, &CSamplePlcDlg::OnBnClickedCheckEmergency)
	ON_BN_CLICKED(IDC_BUTTON_READY, &CSamplePlcDlg::OnBnClickedButtonReady)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN, &CSamplePlcDlg::OnBnClickedButtonOrigin)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSamplePlcDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_RESET, &CSamplePlcDlg::OnBnClickedButtonAlarmReset)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_X, &CSamplePlcDlg::OnBnClickedButtonMoveX)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_Y, &CSamplePlcDlg::OnBnClickedButtonMoveY)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_XY, &CSamplePlcDlg::OnBnClickedButtonMoveXy)
END_MESSAGE_MAP()


BOOL CSamplePlcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	CWnd* wnd = this->GetDlgItem(IDC_EDIT_X);
	if (wnd && wnd->m_hWnd)
	{
		wnd->SetWindowText(L"0");
	}
	wnd = this->GetDlgItem(IDC_EDIT_Y);
	if (wnd && wnd->m_hWnd)
	{
		wnd->SetWindowText(L"0");
	}

	try
	{
		// 例外を通知する関数を登録
		Transfer::AttachFunctionException(std::bind(&CSamplePlcDlg::OnTransferException, this, std::placeholders::_1));

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
		MessageBox(Text::Convert(message.str()).c_str());
	}

#ifdef IoUse
	try
	{
		// 例外を通知する関数を登録
		Io::AttachFunctionException(std::bind(&CSamplePlcDlg::OnIoException, this, std::placeholders::_1));

		// 入力の変化を通知する関数を登録
		Io::AttachFunctionInput(std::bind(&CSamplePlcDlg::OnIoInput, this, std::placeholders::_1));

		Io::Setting::CDefault settingDefault;

		// ポーリングの周期[ms]
		settingDefault.Cycle = 100;

		// IOを初期化
		Io::Initialize(settingDefault);

		// IOに接続
		Io::Setting::CConnect connectSetting;
		Io::Setting::CAddressRange addressRange;

		// 安川PLCのレジスタ
		addressRange.Start.Category = Plc::Register::ConstCategoryM;
		addressRange.Start.Category += Plc::Register::ConstTypeBit;
		addressRange.Start.Hex = true;
		addressRange.Start.Margin = 6;
		addressRange.Start.Index = 0x000001;
		addressRange.Size = 4;
		connectSetting.AddressRanges.emplace_back(addressRange);
		addressRange.Start.Index = 0x000010;
		addressRange.Size = 3;
		connectSetting.AddressRanges.emplace_back(addressRange);
		addressRange.Start.Index = 0x000020;
		addressRange.Size = 3;
		connectSetting.AddressRanges.emplace_back(addressRange);
		addressRange.Start.Index = 0x000100;
		addressRange.Size = 1;
		connectSetting.AddressRanges.emplace_back(addressRange);

		addressRange.Start.Category = Plc::Register::ConstCategoryM;
		addressRange.Start.Category += Plc::Register::ConstTypeWord;
		addressRange.Start.Margin = 5;
		addressRange.Start.Index = 0x1000;
		addressRange.Size = 1;
		connectSetting.AddressRanges.emplace_back(addressRange);

		addressRange.Start.Category = Plc::Register::ConstCategoryM;
		addressRange.Start.Category += Plc::Register::ConstTypeLong;
		addressRange.Start.Margin = 5;
		addressRange.Start.Index = 0x2000;
		addressRange.Size = 4;
		connectSetting.AddressRanges.emplace_back(addressRange);
		addressRange.Start.Index = 0x2100;
		addressRange.Size = 2;
		connectSetting.AddressRanges.emplace_back(addressRange);

		Io::Status::CEntity bit;

		bit.Category = Plc::Register::ConstCategoryM;
		bit.Category += Plc::Register::ConstTypeBit;
		bit.Value = false;
		bit.Hex = true;

		// -ジョグ移動X軸
		bit.Index = JogIoNegativeX;
		m_jogIo.Entitys.emplace_back(bit);

		// +ジョグ移動X軸
		bit.Index = JogIoPositiveX;
		m_jogIo.Entitys.emplace_back(bit);

		// -ジョグ移動Y軸
		bit.Index = JogIoNegativeY;
		m_jogIo.Entitys.emplace_back(bit);

		// +ジョグ移動Y軸
		bit.Index = JogIoPositiveY;
		m_jogIo.Entitys.emplace_back(bit);

		//↓MotionApi用
		// PCIe接続
		MotionApi::Controller::Setting::COpen open;
		open.Device = MotionApi::Controller::DevicePciE;
		connectSetting.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterOpenController, open.Output()));

		// Apiのタイムアウト
		MotionApi::Controller::Setting::CApiTimeout apiTimeout;
		apiTimeout.Value = Timeout::ConstMotionApi;
		connectSetting.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterApiTimeout, apiTimeout.Output()));
		//↑

		// IOに接続
		Io::Connect(connectSetting);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
#else
	try
	{
		// 例外を通知する関数を登録
		Axis::AttachFunctionException(std::bind(&CSamplePlcDlg::OnAxisException, this, std::placeholders::_1));

		// 入力の変化を通知する関数を登録
		Axis::AttachFunctionInput(std::bind(&CSamplePlcDlg::OnAxisInput, this, std::placeholders::_1));

		Axis::Setting::CDefault settingDefault;

		// ポーリングの周期[ms]
		settingDefault.Cycle = 100;

		// 軸を初期化
		Axis::Initialize(settingDefault);

		// 軸に接続
		Axis::Setting::CConnect connectSetting;

		//↓MotionApi用
		// PCIe接続
		MotionApi::Controller::Setting::COpen open;
		open.Device = MotionApi::Controller::DevicePciE;
		connectSetting.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterOpenController, open.Output()));
		for (int index = AxisX; index <= AxisY; index++)
		{
			Axis::Setting::CDriver driver;
			MotionApi::Axis::Setting::CDeclare declare;

			// SVR32(仮想軸)
			//declare.SlotNo = 0;

			switch (index)
			{
			case AxisX:
				driver.Name = "X軸";
				// 仮想軸
				//declare.SubSlotNo = 4;
				declare.Address = 0x8000;
				break;

			case AxisY:
				driver.Name = "Y軸";
				// 仮想軸
				//declare.SubSlotNo = 4;
				declare.Address = 0x8080;
				break;
			}

			declare.StationNo = index;
			declare.LogicalNo = index;
			//declare.Type = MotionApi::Axis::AxisReal;

			driver.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterDeclareAxis, declare));
			connectSetting.Drivers[index] = driver;
			m_driverAxises[index] = driver;
		}

		// Apiのタイムアウト
		MotionApi::Controller::Setting::CApiTimeout apiTimeout;
		apiTimeout.Value = Timeout::ConstMotionApi;
		connectSetting.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterApiTimeout, apiTimeout.Output()));
		//↑

		// 軸に接続
		Axis::Connect(connectSetting);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
#endif

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CSamplePlcDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
#ifdef IoUse
	try
	{
		// IOを破棄
		Io::Destroy();

		// 入力を通知する関数を解除
		Io::DetachFunctionInput();

		// 例外を通知する関数を解除
		Io::DetachFunctionException();
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
#else
	try
	{
		// 軸を破棄
		Axis::Destroy();

		// 入力を通知する関数を解除
		Axis::DetachFunctionInput();

		// 例外を通知する関数を解除
		Axis::DetachFunctionException();
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
#endif
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CSamplePlcDlg::OnPaint()
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

		CPaintDC dc(this); // 描画のデバイス コンテキスト

		Logging::CObject logging;

		try
		{
#ifdef IoUse
			Io::Specify::CAddress address;

			// 運転準備の状態
			address.Category = Plc::Register::ConstCategoryM;
			address.Category += Plc::Register::ConstTypeBit;
			address.Index = 0x11;
			DrawSignal(address, IDC_STATIC_READY, RGB(0, 255, 0));

			// 原点復帰の状態
			address.Index = 0x12;
			DrawSignal(address, IDC_STATIC_ORIGIN, RGB(0, 255, 0));

			// 異常発生中の状態
			address.Index = 0x10;
			DrawSignal(address, IDC_STATIC_EMERGENCY, RGB(255, 0, 0));

			// 処理中の状態
			address.Index = 0x20;
			DrawSignal(address, IDC_STATIC_RUNNING, RGB(0, 255, 0));

			// 動作中X軸の状態
			address.Index = 0x21;
			DrawSignal(address, IDC_STATIC_RUNNING_X, RGB(0, 255, 0));

			// 動作中Y軸の状態
			address.Index = 0x22;
			DrawSignal(address, IDC_STATIC_RUNNING_Y, RGB(0, 255, 0));

			// -オーバーランX軸の状態
			address.Index = 0x1;
			DrawSignal(address, IDC_STATIC_LOWER_OVERRUN_X, RGB(255, 0, 0));
	
			// +オーバーランX軸の状態
			address.Index = 0x2;
			DrawSignal(address, IDC_STATIC_UPPER_OVERRUN_X, RGB(255, 0, 0));

			// -オーバーランY軸の状態
			address.Index = 0x3;
			DrawSignal(address, IDC_STATIC_LOWER_OVERRUN_Y, RGB(255, 0, 0));

			// +オーバーランY軸の状態
			address.Index = 0x4;
			DrawSignal(address, IDC_STATIC_UPPER_OVERRUN_Y, RGB(255, 0, 0));
#else
			COLORREF offColor = RGB(64, 64, 64);
			COLORREF color = offColor;

			// 運転準備の状態
			color = RGB(0, 255, 0);
			for (const auto& driver : m_inputAxis.Drivers)
			{
				if (driver.second.Motion.count(Axis::Status::Input::MotionServo))
				{
					if (driver.second.Motion.at(Axis::Status::Input::MotionServo) == false)
					{
						color = offColor;
					}
				}
			}
			DrawCircle(IDC_STATIC_READY, color);

			// 原点復帰の状態
			color = RGB(0, 255, 0);
			for (const auto& driver : m_inputAxis.Drivers)
			{
				if (driver.second.Motion.count(Axis::Status::Input::MotionOrigin))
				{
					if (driver.second.Motion.at(Axis::Status::Input::MotionOrigin) == false)
					{
						color = offColor;
					}
				}
			}
			DrawCircle(IDC_STATIC_ORIGIN, color);

			// 異常発生中の状態
			color = offColor;
			for (const auto& driver : m_inputAxis.Drivers)
			{
				for (const auto& warning : driver.second.Warning)
				{
					if (warning.second)
					{
						color = RGB(255, 0, 0);
					}
				}

				for (const auto& alarm : driver.second.Alarm)
				{
					if (alarm.second)
					{
						color = RGB(255, 0, 0);
					}
				}
			}
			DrawCircle(IDC_STATIC_EMERGENCY, color);
#endif
			// 現在値
			DrawCurrent(IDC_STATIC_AREA);
		}
		catch (const std::exception& e)
		{
			logging.Level = Logging::LevelError;
			logging.Message << e.what();
			Transfer::Output(logging);
		}
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CSamplePlcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSamplePlcDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	try
	{
		Logging::CObject logging;

		logging.Message << "OnLButtonDown()";

#ifdef IoUse
		for (auto iterator = m_jogIo.Entitys.begin(); iterator != m_jogIo.Entitys.end(); iterator++)
		{
			switch (iterator->Index)
			{
			case JogIoNegativeX:
				// -ジョグ移動X軸
				iterator->Value = IsJogNegativeX(point);
				logging.Message << Logging::ConstSeparator << "-X" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
				break;

			case JogIoPositiveX:
				// +ジョグ移動X軸
				iterator->Value = IsJogPositiveX(point);
				logging.Message << Logging::ConstSeparator << "+X" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
				break;

			case JogIoNegativeY:
				// -ジョグ移動Y軸
				iterator->Value = IsJogNegativeY(point);
				logging.Message << Logging::ConstSeparator << "-Y" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
				break;

			case JogIoPositiveY:
				// +ジョグ移動Y軸
				iterator->Value = IsJogPositiveY(point);
				logging.Message << Logging::ConstSeparator << "+Y" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
				break;
			}
		}

		Io::WriteOutput(m_jogIo);
#else
		m_jogAxis.clear();

		// -ジョグ移動X軸
		if (IsJogNegativeX(point))
		{
			m_jogAxis[AxisX] = MotionApi::Device::DirectionNegative;
			logging.Message << Logging::ConstSeparator << "-X";
		}

		// +ジョグ移動X軸
		if (IsJogPositiveX(point))
		{
			m_jogAxis[AxisX] = MotionApi::Device::DirectionPositive;
			logging.Message << Logging::ConstSeparator << "+X";
		}

		// -ジョグ移動Y軸
		if (IsJogNegativeY(point))
		{
			m_jogAxis[AxisY] = MotionApi::Device::DirectionNegative;
			logging.Message << Logging::ConstSeparator << "-Y";
		}

		// +ジョグ移動Y軸
		if (IsJogPositiveY(point))
		{
			m_jogAxis[AxisY] = MotionApi::Device::DirectionPositive;
			logging.Message << Logging::ConstSeparator << "+Y";
		}

		Axis::Status::Output::CStartJog status;
		bool valid = false;

		for (const auto& axis : m_jogAxis)
		{
			status.Drivers[axis.first];

			MotionApi::Device::Setting::Jog::CStart setting;

			//setting.Coordinate = MotionApi::Device::CoodinateMachine;
			//setting.MoveType = MotionApi::Device::MoveAbsolute;
			//setting.VelocityType = MotionApi::Device::VelocityUnit;
			//setting.AccDecType = MotionApi::Device::AccDecKeep;
			//setting.FilterType = MotionApi::Device::FilterKeep;

			//setting.MaxVelocity.Indirect = false;
			//setting.MaxVelocity.Value = 3000;
			//setting.Acceleration.Indirect = false;
			//setting.Acceleration.Value = 100;
			//setting.Deceleration.Indirect = false;
			//setting.Deceleration.Value = 100;
			//setting.FilterTime.Indirect = false;
			//setting.FilterTime.Value = 100;
			//setting.Velocity.Indirect = false;
			//setting.Velocity.Value = 3000;
			//setting.ApproachVelocity.Indirect = false;
			//setting.ApproachVelocity.Value = 1000;
			//setting.CreepVelocity.Indirect = false;
			//setting.CreepVelocity.Value = 500;

			setting.Direction = axis.second;
			setting.Timeout = 100;
			status.Drivers[axis.first].Other.Objects.emplace_back(Variant::CObject(Plc::ParameterStartJog, setting));
			valid = true;
		}

		if (valid)
		{
			// ジョグを開始
			Axis::StartJog(status);
		}
#endif
		Transfer::Output(logging);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSamplePlcDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	try
	{
		Logging::CObject logging;

		logging.Message << "OnLButtonUp()";

#ifdef IoUse
		for (auto iterator = m_jogIo.Entitys.begin(); iterator != m_jogIo.Entitys.end(); iterator++)
		{
			iterator->Value = false;
			switch (iterator->Index)
			{
			case JogIoNegativeX:
				// -ジョグ移動X軸
				logging.Message << Logging::ConstSeparator << "-X" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
				break;

			case JogIoPositiveX:
				// +ジョグ移動X軸
				logging.Message << Logging::ConstSeparator << "+X" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
				break;

			case JogIoNegativeY:
				// -ジョグ移動Y軸
				logging.Message << Logging::ConstSeparator << "-Y" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
				break;

			case JogIoPositiveY:
				// +ジョグ移動Y軸
				logging.Message << Logging::ConstSeparator << "+Y" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
				break;
			}
		}

		Io::WriteOutput(m_jogIo);
#else
		if (0 < m_jogAxis.size())
		{
			Axis::Status::Output::CStopJog status;

			for (const auto& axis : m_jogAxis)
			{
				status.Drivers[axis.first];

				MotionApi::Device::Setting::Jog::CStop setting;

				setting.Complete = MotionApi::Device::CompletePositioning;
				status.Drivers[axis.first].Other.Objects.emplace_back(Variant::CObject(Plc::ParameterStopJog, setting));
			}

			// ジョグを停止
			Axis::StopJog(status);
		}
		m_jogAxis.clear();
#endif
		Transfer::Output(logging);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CSamplePlcDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	try
	{
		bool change = false;

		Logging::CObject logging;

		logging.Message << "OnMouseMove()";

#ifdef IoUse
		for (auto iterator = m_jogIo.Entitys.begin(); iterator != m_jogIo.Entitys.end(); iterator++)
		{
			if (Variant::Convert<bool>(iterator->Value))
			{
				switch (iterator->Index)
				{
				case JogIoNegativeX:
					// -ジョグ移動X軸
					if (IsJogNegativeX(point))
					{
						continue;
					}
					logging.Message << Logging::ConstSeparator << "-X" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
					break;

				case JogIoPositiveX:
					// +ジョグ移動X軸
					if (IsJogPositiveX(point))
					{
						continue;
					}
					logging.Message << Logging::ConstSeparator << "+X" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
					break;

				case JogIoNegativeY:
					// -ジョグ移動Y軸
					if (IsJogNegativeY(point))
					{
						continue;
					}
					logging.Message << Logging::ConstSeparator << "-Y" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
					break;

				case JogIoPositiveY:
					// +ジョグ移動Y軸
					if (IsJogPositiveY(point))
					{
						continue;
					}
					logging.Message << Logging::ConstSeparator << "+Y" << Logging::ConstSeparator << Variant::FormatValue(iterator->Value);
					break;
				}
				iterator->Value = false;
				change = true;
			}
		}

		if (change)
		{
			Io::WriteOutput(m_jogIo);

			Transfer::Output(logging);
		}
#else
		if (0 < m_jogAxis.size())
		{
			Axis::Status::Output::CStopJog status;

			for (const auto& axis : m_jogAxis)
			{
				switch (axis.first)
				{
				case AxisX:
					switch (axis.second)
					{
					case MotionApi::Device::DirectionNegative:
						// -ジョグ移動X軸
						if (IsJogNegativeX(point))
						{
							continue;
						}
						logging.Message << Logging::ConstSeparator << "-X";
						break;

					case MotionApi::Device::DirectionPositive:
						// +ジョグ移動X軸
						if (IsJogPositiveX(point))
						{
							continue;
						}
						logging.Message << Logging::ConstSeparator << "+X";
						break;
					}
					break;

				case AxisY:
					switch (axis.second)
					{
					case MotionApi::Device::DirectionNegative:
						// -ジョグ移動Y軸
						if (IsJogNegativeY(point))
						{
							continue;
						}
						logging.Message << Logging::ConstSeparator << "-Y";
						break;

					case MotionApi::Device::DirectionPositive:
						// +ジョグ移動Y軸
						if (IsJogPositiveY(point))
						{
							continue;
						}
						logging.Message << Logging::ConstSeparator << "+X";
						break;
					}
					break;
				}

				status.Drivers[axis.first] = m_driverAxises[axis.first];
				MotionApi::Device::Setting::Jog::CStop setting;
				setting.Complete = MotionApi::Device::CompletePositioning;
				status.Drivers[axis.first].Other.Objects.emplace_back(Variant::CObject(Plc::ParameterStopJog, setting));
				change = true;
			}

			if (change)
			{
				// ジョグを停止
				Axis::StopJog(status);
			}
		}
#endif
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CSamplePlcDlg::OnBnClickedButtonReady()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		// 運転準備
#ifdef IoUse
		HandShake(3);
#else
		Axis::Status::Output::CServo status;

		status.Mode = Signal::StatusOn;
		status.Drivers = m_driverAxises;
		status.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterServoTimeout, 3000));

		// サーボをON
		Axis::Servo(status);
#endif
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}

void CSamplePlcDlg::OnBnClickedButtonOrigin()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		// 原点復帰
#ifdef IoUse
		HandShake(11);
#else
		Axis::Status::Output::CMoveOrigin status;

		for (const auto& driver : m_driverAxises)
		{
			status.Drivers[driver.first];

			MotionApi::Device::Setting::Origin::CAxis setting;

			//setting.Coordinate = MotionApi::Device::CoodinateMachine;
			//setting.MoveType = MotionApi::Device::MoveAbsolute;
			//setting.VelocityType = MotionApi::Device::VelocityUnit;
			//setting.AccDecType = MotionApi::Device::AccDecKeep;
			//setting.FilterType = MotionApi::Device::FilterKeep;

			//setting.MaxVelocity.Indirect = false;
			//setting.MaxVelocity.Value = 3000;
			//setting.Acceleration.Indirect = false;
			//setting.Acceleration.Value = 100;
			setting.Deceleration.Indirect = false;
			setting.Deceleration.Value = 100;
			setting.FilterTime.Indirect = false;
			setting.FilterTime.Value = 100;
			setting.Velocity.Indirect = false;
			setting.Velocity.Value = 3000;
			setting.ApproachVelocity.Indirect = false;
			setting.ApproachVelocity.Value = 1000;
			setting.CreepVelocity.Indirect = false;
			setting.CreepVelocity.Value = 500;

			setting.PositionType = MotionApi::Device::PositionImmediate;
			setting.PositionData = 0;

			setting.Method = MotionApi::Device::OriginC;
			//setting.Direction = MotionApi::Device::DirectionNegative;
			//setting.Complete = MotionApi::Device::CompleteStart;

			status.Drivers[driver.first].Other.Objects.emplace_back(Variant::CObject(Plc::ParameterOriginAxis, setting));
		}
		status.Other.Objects.emplace_back(Variant::CObject(Plc::ParameterMoveOriginTimeout, 10000));

		// 原点復帰を実行
		Axis::MoveOrigin(status);
#endif
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}

void CSamplePlcDlg::OnBnClickedButtonMoveX()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		// 軸の目標位置を書き込み
#ifdef IoUse
		WriteAxis();

		// X軸を移動
		HandShake(21);
#else

#endif
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}

void CSamplePlcDlg::OnBnClickedButtonMoveY()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		// 軸の目標位置を書き込み
#ifdef IoUse
		WriteAxis();

		// Y軸を移動
		HandShake(22);
#else

#endif
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}

void CSamplePlcDlg::OnBnClickedButtonMoveXy()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		// 軸の目標位置を書き込み
#ifdef IoUse
		WriteAxis();

		// XY軸を移動
		HandShake(23);
#else

#endif
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}

void CSamplePlcDlg::OnBnClickedButtonStop()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		// 停止
#ifdef IoUse
		HandShake(2);
#else

#endif
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}

void CSamplePlcDlg::OnBnClickedButtonAlarmReset()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	try
	{
		// アラーム解除
#ifdef IoUse
		HandShake(1);
#else

#endif
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}

void CSamplePlcDlg::OnBnClickedCheckEmergency()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* button = (CButton*)GetDlgItem(IDC_CHECK_EMERGENCY);

	try
	{
		do
		{
			if (button == nullptr || button->m_hWnd == nullptr)
			{
				break;
			}

#ifdef IoUse
			Io::Status::COutput output;
			Io::Status::CEntity bit;

			// 非常停止
			bit.Category = Plc::Register::ConstCategoryM;
			bit.Category += Plc::Register::ConstTypeBit;
			bit.Index = 0x100000;
			bit.Value = button->GetCheck() == BST_CHECKED ? true : false;
			bit.Hex = true;

			output.Entitys.emplace_back(bit);

			Io::WriteOutput(output);
#else

#endif
		} while (false);
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}
}

CRect CSamplePlcDlg::AreaControl(int id)
{
	CRect ret = CRect();

	auto wnd = this->GetDlgItem(id);
	if (wnd != nullptr && wnd->m_hWnd != nullptr)
	{
		wnd->GetWindowRect(ret);
	}

	return ret;
}

bool CSamplePlcDlg::IsJogNegativeX(POINT target)
{
	bool ret = false;

	auto areaControl = AreaControl(IDC_BUTTON_JOG_LEFT_TOP);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}
	areaControl = AreaControl(IDC_BUTTON_JOG_LEFT);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}
	areaControl = AreaControl(IDC_BUTTON_JOG_LEFT_BOTTOM);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}

	return ret;
}

bool CSamplePlcDlg::IsJogPositiveX(POINT target)
{
	bool ret = false;

	auto areaControl = AreaControl(IDC_BUTTON_JOG_RIGHT_TOP);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}
	areaControl = AreaControl(IDC_BUTTON_JOG_RIGHT);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}
	areaControl = AreaControl(IDC_BUTTON_JOG_RIGHT_BOTTOM);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}

	return ret;
}

bool CSamplePlcDlg::IsJogNegativeY(POINT target)
{
	bool ret = false;

	auto areaControl = AreaControl(IDC_BUTTON_JOG_LEFT_BOTTOM);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}
	areaControl = AreaControl(IDC_BUTTON_JOG_BOTTOM);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}
	areaControl = AreaControl(IDC_BUTTON_JOG_RIGHT_BOTTOM);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}

	return ret;
}

bool CSamplePlcDlg::IsJogPositiveY(POINT target)
{
	bool ret = false;

	auto areaControl = AreaControl(IDC_BUTTON_JOG_LEFT_TOP);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}
	areaControl = AreaControl(IDC_BUTTON_JOG_TOP);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}
	areaControl = AreaControl(IDC_BUTTON_JOG_RIGHT_TOP);
	if (areaControl.PtInRect(target))
	{
		ret = true;
	}

	return ret;
}

void CSamplePlcDlg::WriteAxis()
{
	Io::Status::COutput output;

	Io::Status::CEntity axis;

	axis.Category = Plc::Register::ConstCategoryM;
	axis.Category += Plc::Register::ConstTypeLong;

	auto wnd = this->GetDlgItem(IDC_EDIT_X);
	if (wnd != nullptr && wnd->m_hWnd != nullptr)
	{
		CString buffer;

		wnd->GetWindowText(buffer);
		axis.Index = (ConstLong)0x12000;
		axis.Value = std::atoi(Text::Convert(buffer.GetString()).c_str());
		output.Entitys.emplace_back(axis);
	}

	wnd = this->GetDlgItem(IDC_EDIT_Y);
	if (wnd != nullptr && wnd->m_hWnd != nullptr)
	{
		CString buffer;

		wnd->GetWindowText(buffer);
		axis.Index = (ConstLong)0x12002;
		axis.Value = std::atoi(Text::Convert(buffer.GetString()).c_str());
		output.Entitys.emplace_back(axis);
	}

	Io::WriteOutput(output);
}

void CSamplePlcDlg::HandShake(ConstWord request)
{
	Io::Status::COutput output;
	Io::Status::CEntity kind;
	Io::Status::CEntity bit;

	// 要求の種類
	kind.Category = Plc::Register::ConstCategoryM;
	kind.Category += Plc::Register::ConstTypeWord;
	kind.Index = 0x11000;
	kind.Value = request;
	kind.Hex = true;
	output.Entitys.emplace_back(kind);

	// 動作の要求
	bit.Category = Plc::Register::ConstCategoryM;
	bit.Category += Plc::Register::ConstTypeBit;
	bit.Index = 0x100100;
	bit.Value = true;
	bit.Hex = true;
	output.Entitys.emplace_back(bit);

	Io::WriteOutput(output);

	try
	{
		Io::Setting::CInputPending pendingSetting;

		// 要求の応答
		pendingSetting.Trigger.Address.Category = Plc::Register::ConstCategoryM;
		pendingSetting.Trigger.Address.Category += Plc::Register::ConstTypeBit;
		pendingSetting.Trigger.Address.Index = 0x100;
		pendingSetting.Trigger.Direction = Io::Status::DirectionRise;
		pendingSetting.Timeout = 1000;
		Io::TriggerInputPending(pendingSetting);

		Io::Setting::CInput readSetting;
		Io::Specify::CAddress response;

		// 応答の種類
		response.Category = Plc::Register::ConstCategoryM;
		response.Category += Plc::Register::ConstTypeWord;
		response.Index = 0x01000;
		response.Hex = true;
		readSetting.Addresses.emplace_back(response);
		auto input = Io::ReadInput(readSetting);

		if (Variant::Convert<ConstWord>(input.Values[response]) != 1)
		{
			std::stringstream message;

			switch (Variant::Convert<ConstWord>(input.Values[response]))
			{
			case 0:
				message << "不明";
				break;

			case 2:
				message << "種類が未対応";
				break;

			case 3:
				message << "異常が発生中";
				break;

			case 4:
				message << "動作中";
				break;

			case 5:
				message << "停止中";
				break;

			case 6:
				message << "運転準備が未完了";
				break;

			case 7:
				message << "原点復帰が未完了";
				break;
			}
			MessageBox(Text::Convert(message.str()).c_str());
		}
	}
	catch (const std::exception& e)
	{
		std::stringstream message;

		message << e.what();
		MessageBox(Text::Convert(std::regex_replace(message.str(), std::regex("\t"), "\r\n")).c_str());
	}

	// 動作の要求を解除
	bit.Value = false;

	output.Entitys.clear();
	output.Entitys.emplace_back(bit);

	Io::WriteOutput(output);
}

void CSamplePlcDlg::DrawSignal(const Io::Specify::CAddress& address, int id, COLORREF color)
{
	do
	{
		if (0 < m_inputIo.Values.count(address))
		{
			if (Variant::Convert<ConstBool>(m_inputIo.Values[address]))
			{
				break;
			}
		}

		color = RGB(64, 64, 64);
	} while (false);

	DrawCircle(id, color);
}

void CSamplePlcDlg::DrawCircle(int id, COLORREF color)
{
	auto wnd = this->GetDlgItem(id);

	do
	{
		if (wnd == nullptr || wnd->m_hWnd == nullptr)
		{
			break;
		}

		CClientDC dc(wnd);
		CBrush brush(color);
		CRect rect;

		wnd->GetClientRect(&rect);

		dc.SelectObject(brush);
		dc.Ellipse(&rect);
	} while (false);
}

void CSamplePlcDlg::DrawCurrent(int id)
{
	auto wnd = this->GetDlgItem(id);

	do
	{
		if (wnd == nullptr || wnd->m_hWnd == nullptr)
		{
			break;
		}

		CClientDC dc(wnd);
		CBrush brushOrigin(RGB(255, 0, 0));
		CBrush brushMark(RGB(0, 0, 255));
		CRect control;
		CRect rect(-5, -5, 5, 5);

		wnd->GetClientRect(&control);

		CRect area;

#ifdef IoUse
		Io::Specify::CAddress address;

		address.Category = Plc::Register::ConstCategoryM;
		address.Category += Plc::Register::ConstTypeLong;

		// -リミットX軸
		address.Index = 0x2000;
		if (0 < m_inputIo.Values.count(address))
		{
			area.left = Variant::Convert<ConstLong>(m_inputIo.Values.at(address));
		}
		// +リミットX軸
		address.Index = 0x2002;
		if (0 < m_inputIo.Values.count(address))
		{
			area.right = Variant::Convert<ConstLong>(m_inputIo.Values.at(address));
		}
		// -リミットY軸
		address.Index = 0x2004;
		if (0 < m_inputIo.Values.count(address))
		{
			area.top = Variant::Convert<ConstLong>(m_inputIo.Values.at(address));
		}
		// +リミットY軸
		address.Index = 0x2006;
		if (0 < m_inputIo.Values.count(address))
		{
			area.bottom = Variant::Convert<ConstLong>(m_inputIo.Values.at(address));
		}

		float ratioX = (float)control.Width() / (float)area.Width();
		float ratioY = (float)control.Height() / (float)area.Height();

		long currentX = 0;
		long currentY = 0;

		// 現在のX軸
		address.Index = 0x2100;
		if (0 < m_inputIo.Values.count(address))
		{
			currentX = Variant::Convert<ConstLong>(m_inputIo.Values.at(address));
		}
		// 現在のY軸
		address.Index = 0x2102;
		if (0 < m_inputIo.Values.count(address))
		{
			currentY = Variant::Convert<ConstLong>(m_inputIo.Values.at(address));
		}

		float originX;
		float originY;

		// 原点を算出
		originX = 0;// (float)area.left * ratioX;
		originX -= area.left;
		originY = 0;// (float)control.Height() - (float)area.top * ratioY;
		originY -= area.top;
#else
		// -リミットX軸
		area.left = m_inputAxis.Drivers.at(AxisX).Coordinate.at(Axis::Status::Input::CoordinateNegativeLimit);
		// +リミットX軸
		area.right = m_inputAxis.Drivers.at(AxisX).Coordinate.at(Axis::Status::Input::CoordinatePositiveLimit);
		// -リミットY軸
		area.top = m_inputAxis.Drivers.at(AxisY).Coordinate.at(Axis::Status::Input::CoordinateNegativeLimit);
		// +リミットY軸
		area.bottom = m_inputAxis.Drivers.at(AxisY).Coordinate.at(Axis::Status::Input::CoordinatePositiveLimit);

		float ratioX = (float)control.Width() / (float)area.Width();
		float ratioY = (float)control.Height() / (float)area.Height();

		// 現在のX軸
		long currentX = m_inputAxis.Drivers.at(AxisX).Coordinate.at(Axis::Status::Input::CoordinateCurrent);

		// 現在のY軸
		long currentY = m_inputAxis.Drivers.at(AxisY).Coordinate.at(Axis::Status::Input::CoordinateCurrent);

		float originX;
		float originY;

		// 原点を算出
		originX = 0;// (float)area.left * ratioX;
		originX -= area.left;
		originY = 0;// (float)control.Height() - (float)area.top * ratioY;
		originY -= area.top;
#endif
		// 原点を描画
		dc.SelectObject(brushOrigin);
		dc.MoveTo((int)(originX * ratioX), 0);
		dc.LineTo((int)(originX * ratioX), control.Height());
		dc.MoveTo(0, control.Height() - (int)(originY * ratioY));
		dc.LineTo(control.Width(), control.Height() - (int)(originY * ratioY));

		// 位置をシフト
		currentX -= area.left;
		currentY -= area.top;
		rect.MoveToX((int)((float)currentX * ratioX) - (float)rect.Width() * 0.5);
		rect.MoveToY(control.Height() - (int)((float)currentY * ratioY) - (float)rect.Height() * 0.5);

		dc.SelectObject(brushMark);
		dc.Ellipse(&rect);
	} while (false);
}