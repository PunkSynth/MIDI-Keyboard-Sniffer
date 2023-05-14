
// MIDI_KeyboardDlg.h : header file
//

#pragma once
#include <vector>

// CMIDIKeyboardDlg dialog
class CMIDIKeyboardDlg : public CDialogEx
{
// Construction
public:
	CMIDIKeyboardDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MIDI_KEYBOARD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Attributes
protected:
	HICON m_hIcon;
	CStatic mCtrlSelectText;
	CComboBox mCtrlComPort;
	CButton mCtrlUpdate;
	CButton mCtrlConnect;
	CMFCButton mCtrlKillSwitch;
	CListCtrl mCtrlMidiMsg;

	std::vector<int> mComPortNumbers;
	int mSelectedPort{ -1 };
	int mIsMidiConnected{ 0 };
	bool mIsMidiConnectionKilled{ false };
	bool mIsDialogExitRequested{ false };
	std::vector<unsigned char> mMidiMsg;

// Generated message map functions
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMidiMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMidiStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeComport();
	afx_msg void OnClickedUpdate();
	afx_msg void OnClickedConnect();
	afx_msg void OnClickedKillswitch();
	afx_msg void OnIdok();
	afx_msg void OnIdCancel();

	DECLARE_MESSAGE_MAP()

// Implementation
protected:
	void InitControls();
	void UpdateComPort();
	void InitListCtrl();
	void UpdateListCtrl(const std::vector<unsigned char>& midiMsg);
	void ParseMidiMsg(const std::vector<unsigned char>& midiMsg,
		CString& msg, CString& data1, CString& data2);
	CString GetKey(const unsigned char noteNumber);
	CString GetCCData1(const unsigned char data1);
};
