// ThreadSerialCom.h : Header file

#pragma once

// MIDI definitions
#define CBR_31250				31250
#define MIDI_KEYBOARD_OFF		0
#define MIDI_KEYBOARD_STANDBY	1
#define MIDI_KEYBOARD_ON		2
#define COM_FALIURE				3

struct PROCESS_SERIALCOM {
	HWND hWnd{ NULL }; // Dialog handle
	bool* pIsMidiKilled{ NULL };
	int commPort{ 0 };
};

extern PROCESS_SERIALCOM g_process_serialcom;

UINT ThreadSerialCom(LPVOID pParam);

int ConnectToSerialPort(const int commPortNum, const int baudRate, 
	HANDLE& hComm, DCB& serialParams);

int InitializeSerialPortCommunication(const HANDLE hComm);

int CloseSerialPort(const HANDLE hComm);