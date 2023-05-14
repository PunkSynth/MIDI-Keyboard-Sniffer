// MIDI_KeyboardDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MIDI_Keyboard.h"
#include "MIDI_KeyboardDlg.h"
#include "afxdialogex.h"
#include "WndMsg.h"
#include "ComPort.h"
#include "ThreadSerialCom.h"
#include "MidiMsgFormat.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx {
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMIDIKeyboardDlg dialog

CMIDIKeyboardDlg::CMIDIKeyboardDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MIDI_KEYBOARD_DIALOG, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMIDIKeyboardDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPORT, mCtrlComPort);
	DDX_Control(pDX, IDC_SELECT_TEXT, mCtrlSelectText);
	DDX_Control(pDX, IDC_CONNECT, mCtrlConnect);
	DDX_Control(pDX, IDC_UPDATE, mCtrlUpdate);
	DDX_Control(pDX, IDC_MIDIMSG, mCtrlMidiMsg);
	DDX_Control(pDX, IDC_KILLSWITCH, mCtrlKillSwitch);
	DDX_Control(pDX, IDC_MIDIMSG, mCtrlMidiMsg);
}

BEGIN_MESSAGE_MAP(CMIDIKeyboardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MIDIMSG, &CMIDIKeyboardDlg::OnMidiMsg)
	ON_BN_CLICKED(IDC_UPDATE, &CMIDIKeyboardDlg::OnClickedUpdate)
	ON_CBN_SELCHANGE(IDC_COMPORT, &CMIDIKeyboardDlg::OnSelchangeComport)
	ON_BN_CLICKED(IDC_CONNECT, &CMIDIKeyboardDlg::OnClickedConnect)
	ON_BN_CLICKED(IDC_KILLSWITCH, &CMIDIKeyboardDlg::OnClickedKillswitch)
	ON_MESSAGE(WM_MIDISTATUS, &CMIDIKeyboardDlg::OnMidiStatus)
	ON_COMMAND(IDOK, &CMIDIKeyboardDlg::OnIdok)
	ON_COMMAND(IDCANCEL, &CMIDIKeyboardDlg::OnIdCancel)
END_MESSAGE_MAP()


// CMIDIKeyboardDlg message handlers

BOOL CMIDIKeyboardDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	InitControls();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMIDIKeyboardDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMIDIKeyboardDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMIDIKeyboardDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

afx_msg LRESULT CMIDIKeyboardDlg::OnMidiMsg(WPARAM wParam, LPARAM lParam) {
	std::vector<unsigned char>().swap(mMidiMsg);

	mMidiMsg.push_back(static_cast<unsigned char>(wParam));
	mMidiMsg.push_back(static_cast<unsigned char>(lParam >> 8));
	mMidiMsg.push_back(static_cast<unsigned char>(lParam & 0xff));

	UpdateListCtrl(mMidiMsg);

	return 0;
}

afx_msg LRESULT CMIDIKeyboardDlg::OnMidiStatus(WPARAM wParam, LPARAM /*lParam*/) {
	switch (wParam) {
	case MIDI_KEYBOARD_ON:
		mCtrlSelectText.EnableWindow(FALSE);
		mCtrlComPort.EnableWindow(FALSE);
		mCtrlUpdate.EnableWindow(FALSE);
		mCtrlConnect.EnableWindow(FALSE);
		mCtrlKillSwitch.EnableWindow(TRUE);
		mCtrlMidiMsg.EnableWindow(TRUE);
		mIsMidiConnected = MIDI_KEYBOARD_ON;
		break;
	case MIDI_KEYBOARD_STANDBY:
		mCtrlSelectText.EnableWindow(FALSE);
		mCtrlComPort.EnableWindow(FALSE);
		mCtrlUpdate.EnableWindow(FALSE);
		mCtrlConnect.EnableWindow(FALSE);
		mCtrlKillSwitch.EnableWindow(FALSE);
		mCtrlMidiMsg.EnableWindow(TRUE);
		mIsMidiConnected = MIDI_KEYBOARD_STANDBY;
		InitListCtrl();
		break;
	case MIDI_KEYBOARD_OFF:
		mCtrlSelectText.EnableWindow(TRUE);
		mCtrlComPort.EnableWindow(TRUE);
		mCtrlUpdate.EnableWindow(TRUE);
		if (mSelectedPort >= 0) {
			mCtrlConnect.EnableWindow(TRUE);
		}
		else {
			mCtrlConnect.EnableWindow(FALSE);
		}
		mCtrlKillSwitch.EnableWindow(FALSE);
		mIsMidiConnectionKilled = false;
		mCtrlMidiMsg.EnableWindow(FALSE);
		mIsMidiConnected = MIDI_KEYBOARD_OFF;
		if (mIsDialogExitRequested) {
			CDialogEx::OnCancel();
		}
		break;
	case COM_FALIURE:
	{
		MessageBox(_T("Com Failiure!"), _T("Ups!"), MB_ICONERROR);
		bool isComPorts = ComPort().RetrieveComPorts(mComPortNumbers);
		mCtrlComPort.SetEditSel(0, -1);
		mCtrlComPort.Clear();
		mCtrlComPort.ResetContent();
		mSelectedPort = -1;
		if (isComPorts) {
			for (int comPort : mComPortNumbers) {
				CString comPortStr;
				comPortStr.Format(_T("COM%d"), comPort);
				mCtrlComPort.AddString(comPortStr);
			}
		}
		mCtrlConnect.EnableWindow(FALSE);
		mIsMidiConnected = MIDI_KEYBOARD_OFF;
	}
		break;
	}
		
	return 0;
}

void CMIDIKeyboardDlg::OnSelchangeComport() {
	mSelectedPort = mCtrlComPort.GetCurSel();
	if (mSelectedPort >= 0) {
		mCtrlConnect.EnableWindow(TRUE);
	}
	else {
		mCtrlConnect.EnableWindow(FALSE);
	}
}

void CMIDIKeyboardDlg::OnClickedUpdate() {
	UpdateComPort();
	MessageBeep(MB_ICONINFORMATION);
}

void CMIDIKeyboardDlg::OnClickedConnect() {
	g_process_serialcom.hWnd = this->m_hWnd;
	g_process_serialcom.pIsMidiKilled = &mIsMidiConnectionKilled;
	g_process_serialcom.commPort = mComPortNumbers[mSelectedPort];

	AfxBeginThread(ThreadSerialCom, &g_process_serialcom);
}

void CMIDIKeyboardDlg::OnClickedKillswitch() {
	mIsMidiConnectionKilled = true;
}

void CMIDIKeyboardDlg::OnIdok() {
	// This function is disabled to prevent shutdown
}

void CMIDIKeyboardDlg::OnIdCancel() {
	switch (mIsMidiConnected) {
	case MIDI_KEYBOARD_OFF:
	case MIDI_KEYBOARD_STANDBY:
		CDialogEx::OnCancel();
		break;
	case MIDI_KEYBOARD_ON:
		mIsMidiConnectionKilled = true;
		mIsDialogExitRequested = true;
		break;
	}
}

void CMIDIKeyboardDlg::InitControls() {
	UpdateComPort();
	mCtrlSelectText.EnableWindow(TRUE);
	mCtrlComPort.EnableWindow(TRUE);
	mCtrlUpdate.EnableWindow(TRUE);
	if (mSelectedPort >= 0) {
		mCtrlConnect.EnableWindow(TRUE);
	}
	else {
		mCtrlConnect.EnableWindow(FALSE);
	}
	mCtrlKillSwitch.EnableWindow(FALSE);
	mIsMidiConnectionKilled = false;
	mCtrlMidiMsg.EnableWindow(FALSE);
}

void CMIDIKeyboardDlg::UpdateComPort() {
	bool isComPorts = ComPort().RetrieveComPorts(mComPortNumbers);

	mCtrlComPort.SetEditSel(0, -1);
	mCtrlComPort.Clear();
	mCtrlComPort.ResetContent();
	mSelectedPort = -1;

	if (isComPorts) {
		for (int comPort : mComPortNumbers) {
			CString comPortStr;
			comPortStr.Format(_T("COM%d"), comPort);
			mCtrlComPort.AddString(comPortStr);
		}
	}

	mCtrlConnect.EnableWindow(FALSE);
}

void CMIDIKeyboardDlg::InitListCtrl() {
	mCtrlMidiMsg.ModifyStyle(0, LVS_SHOWSELALWAYS | LVS_SINGLESEL);
	mCtrlMidiMsg.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	int i;
	LVCOLUMN col;
	int numOfColums = 0;

	col.mask = LVCF_WIDTH;

	while (mCtrlMidiMsg.GetColumn(numOfColums++, &col));
	numOfColums--;

	mCtrlMidiMsg.DeleteAllItems();

	for (i = numOfColums; i >= 0; i--) {
		mCtrlMidiMsg.DeleteColumn(i);
	}

	// insert columns
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	i = 0;

	lvc.iSubItem = i;
	//lvc.pszText = (LPWSTR)(LPCTSTR)sAmount; // This is an example
	lvc.pszText = _T("MIDI Msg (hex)");
	lvc.cx = 100;
	lvc.fmt = LVCFMT_LEFT;
	mCtrlMidiMsg.InsertColumn(i++, &lvc);

	lvc.iSubItem = i;
	lvc.pszText = _T("Chanel");
	lvc.cx = 100;
	lvc.fmt = LVCFMT_LEFT;
	mCtrlMidiMsg.InsertColumn(i++, &lvc);

	lvc.iSubItem = i;
	lvc.pszText = _T("Message");
	lvc.cx = 100;
	lvc.fmt = LVCFMT_LEFT;
	mCtrlMidiMsg.InsertColumn(i++, &lvc);

	lvc.iSubItem = i;
	lvc.pszText = _T("Data 1");
	lvc.cx = 100;
	lvc.fmt = LVCFMT_LEFT;
	mCtrlMidiMsg.InsertColumn(i++, &lvc);

	lvc.iSubItem = i;
	lvc.pszText = _T("Data 2");
	lvc.cx = 100;
	lvc.fmt = LVCFMT_LEFT;
	mCtrlMidiMsg.InsertColumn(i++, &lvc);
}

void CMIDIKeyboardDlg::UpdateListCtrl(
	const std::vector<unsigned char>& midiMsg) {

	// Sanity check
	if (midiMsg.size() != 3) {
		return;
	}

	// insert items

	LV_ITEM lvi;
	ZeroMemory(&lvi, sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	lvi.iSubItem = 0; // MIDI Msg (hex)
	CString val;
	val.Format(_T("%02X:%02X:%02X"), midiMsg[0], midiMsg[1], midiMsg[2]);
	lvi.pszText = (LPWSTR)val.GetString();
	mCtrlMidiMsg.InsertItem(&lvi);

	lvi.iSubItem = 1; // Chanel
	val.Format(_T("ch:%d"), (midiMsg[0] & 0x0F));
	lvi.pszText = (LPWSTR)val.GetString();
	mCtrlMidiMsg.SetItem(&lvi);
	
	CString msg, data1, data2;
	ParseMidiMsg(midiMsg, msg, data1, data2);

	lvi.iSubItem = 2; // Message
	lvi.pszText = (LPWSTR)msg.GetString();
	mCtrlMidiMsg.SetItem(&lvi);

	lvi.iSubItem = 3; // Data 1
	lvi.pszText = (LPWSTR)data1.GetString();
	mCtrlMidiMsg.SetItem(&lvi);

	lvi.iSubItem = 4; // Data 2
	lvi.pszText = (LPWSTR)data2.GetString();
	mCtrlMidiMsg.SetItem(&lvi);
}

// The parser is based on document ...; link ...
void CMIDIKeyboardDlg::ParseMidiMsg(const std::vector<unsigned char>& midiMsg,
	CString& msg, CString& data1, CString& data2) {

	// Sanity check
	if (midiMsg.size() != 3) {
		return;
	}

	unsigned char binMsg = midiMsg[0] >> 4;
	CString key;
	CString code;

	switch (binMsg) {
	case NOTE_OFF:
		msg = NOTE_OFF_STR;
		key = GetKey(midiMsg[1]);
		data1.Format(NOTE_NUMBER_STR, key.GetString(), midiMsg[1]);
		data2.Format(VELOCITY_STR, midiMsg[2]);
		break;
	case NOTE_ON:
		msg = NOTE_ON_STR;
		key = GetKey(midiMsg[1]);
		data1.Format(NOTE_NUMBER_STR, key.GetString(), midiMsg[1]);
		data2.Format(VELOCITY_STR, midiMsg[2]);
		break;
	case POLYFONIC_AFTERTOUCH:
		msg = POLYFONIC_AFTERTOUCH_STR;
		key = GetKey(midiMsg[1]);
		data1.Format(NOTE_NUMBER_STR, key.GetString(), midiMsg[1]);
		data2.Format(PRESSURE_STR, midiMsg[2]);
		break;
	case CONTROL_CHANGE:
		msg = CONTROL_CHANGE_STR;
		code = GetCCData1(midiMsg[1]);
		data1 = code;
		data2.Format(DATA_STR, midiMsg[2]);
		break;
	case PROGRAM_CHANGE:
		msg = PROGRAM_CHANGE_STR;
		data1.Format(PROG_NUMBER_STR, midiMsg[1]);
		data2 = NA_STR;
		break;
	case CHANNEL_AFTERTOUCH:
		msg = CHANNEL_AFTERTOUCH_STR;
		data1.Format(PRESSURE_STR, midiMsg[1]);
		data2 = NA_STR;
		break;
	case PITCH_WHEEL:
		msg = PITCH_WHEEL_STR;
		data1.Format(LSB_STR, midiMsg[1]);
		data2.Format(MSB_STR, midiMsg[2]);
		break;
	case SYS_MSG:
		break;
	}
}

CString CMIDIKeyboardDlg::GetKey(const unsigned char noteNumber) {
	// Sanity check
	if (noteNumber > 127) {
		return NA_STR;
	}

	std::wstring keyArr[12] = { L"C", L"C#", L"D", L"D#",
						L"E", L"F", L"F#", L"G",
						L"G#", L"A", L"A#", L"B" };

	unsigned char nn = noteNumber;
	unsigned char note = nn % 12;
	unsigned char oct = nn / 12;

	CString key;
	key.Format(_T("%ws%d"), 
		keyArr[note].c_str(), 
		static_cast<int>(oct) - 1);

	return key;
}

CString CMIDIKeyboardDlg::GetCCData1(const unsigned char data1) {
	CString code;
	switch (data1) {
	case MODULATION_WHEEL:
	case MODULATION_WHEEL_2:
		code = MODULATION_WHEEL_STR;
		break;
	case BREATH_CONTROLLER:
	case BREATH_CONTROLLER_2:
		code = BREATH_CONTROLLER_STR;
		break;
	case FOOT_CONTROLLER:
	case FOOT_CONTROLLER_2:
		code = FOOT_CONTROLLER_STR;
		break;
	case PORTAMENTO_TIME:
	case PORTAMENTO_TIME_2:
		code = PORTAMENTO_TIME_STR;
		break;
	case DATA_ENTRY_SLIDER:
	case DATA_ENTRY_SLIDER_2:
		code = DATA_ENTRY_SLIDER_STR;
		break;
	case MAIN_VOLUME:
	case MAIN_VOLUME_2:
		code = MAIN_VOLUME_STR;
		break;
	default:
		code.Format(_T("0x%02X"), data1);
		break;
	}

	return code;
}
