// ThreadSerialCom.cpp : Implementation file

#include "pch.h"
#include "ThreadSerialCom.h"
#include "WndMsg.h"
#include <array>
#include <vector>
#include <string>

PROCESS_SERIALCOM g_process_serialcom;

UINT ThreadSerialCom(LPVOID pParam) {
	
	PROCESS_SERIALCOM* pTmp = reinterpret_cast<PROCESS_SERIALCOM*>(pParam);

	// Sanity check
	if (!pTmp->hWnd) {
		return 1;
	}

	if (*(pTmp->pIsMidiKilled) == true) {
		return 1;
	}

	if (pTmp->commPort < 1) {
		return 1;
	}

	int commPort = pTmp->commPort;
	HANDLE hComm{ NULL };
	DCB serialParams;

	// Try to open MIDI serial port
	int ret = ConnectToSerialPort(commPort, CBR_31250, hComm, serialParams);

	if (ret < 0) {
		::SendMessage(pTmp->hWnd, WM_MIDISTATUS, COM_FALIURE, 0);
		return -ret + 1;
	}

	// This function will halt until something is received on the serial port
	// unless something went wrong...
	::SendMessage(pTmp->hWnd, WM_MIDISTATUS, MIDI_KEYBOARD_STANDBY, 0);
	ret = InitializeSerialPortCommunication(hComm);
	::SendMessage(pTmp->hWnd, WM_MIDISTATUS, MIDI_KEYBOARD_ON, 0);

	if (ret < 0) {
		::SendMessage(pTmp->hWnd, WM_MIDISTATUS, MIDI_KEYBOARD_OFF, 0);
		return -ret + 1 + 4;
	}

	BOOL commStat{ FALSE };

	while (true) {
		std::vector<unsigned char> serialBuffer;
		std::vector<unsigned char> midiMsg;
		while (true) {
			char tmpCh{ 0 };
			DWORD numBytesRead{ 0 };
			commStat = ReadFile(hComm, &tmpCh, sizeof(tmpCh), &numBytesRead, NULL);

			if (!commStat) {
				CloseSerialPort(hComm);
				return 1 + 4 + 3 + 1;
			}

			if (numBytesRead > 0) {
				serialBuffer.push_back(tmpCh);
			}
			else {
				break;
			}
		}

		if (serialBuffer.size()) {
			for (auto info : serialBuffer) {
				if (info & 0x80) {
					std::vector<unsigned char>().swap(midiMsg);
					midiMsg.push_back(info);
				}
				else if (midiMsg.size()) {
					midiMsg.push_back(info);
				}

				if (midiMsg.size() >= 3) {
					WPARAM wparam = midiMsg[0];
					LPARAM lparam = (LPARAM)(
						((DWORD)midiMsg[1]) << 8 | ((DWORD)midiMsg[2])
						);
					::PostMessage(pTmp->hWnd, WM_MIDIMSG, wparam, lparam);
					std::vector<unsigned char>().swap(midiMsg);
				}
			}
		}

		Sleep(1); // Just calm down the worker thread

		// Check if kill switch has been engaged
		if (*(pTmp->pIsMidiKilled)) {
			break; // Exit the loop
		}
	}

	CloseSerialPort(hComm);
	::SendMessage(pTmp->hWnd, WM_MIDISTATUS, MIDI_KEYBOARD_OFF, 0);
	return 0;
}

int ConnectToSerialPort(int commPortNum, int baudRate, HANDLE& hComm, DCB& serialParams) {
	// Sanity check
	if (commPortNum < 1 || commPortNum > 25) {
		return -1;
	}

	if (baudRate < CBR_110 || baudRate > CBR_256000) {
		return -1;
	}

	// Initialize comm parameters
	hComm = NULL;
	ZeroMemory(&serialParams, sizeof(DCB));

	// Name of the Serial port(May Change) to be opened
	std::string commPortName = "\\\\.\\COM";
	commPortName += std::to_string(commPortNum);

	// Opening the Serial Port
	hComm = CreateFileA(commPortName.c_str(), // Name of the Port to be Opened
		GENERIC_READ | GENERIC_WRITE, // Read/Write Access
		0,                            // No Sharing, ports cant be shared
		NULL,                         // No Security
		OPEN_EXISTING,                // Open existing port only
		0,                            // Non Overlapped I/O
		NULL);                        // Null for Comm Devices

	// The comm port could not be opened
	if (hComm == INVALID_HANDLE_VALUE) {
		return -2;
	}

	// Setting the Parameters for the SerialPort
	serialParams.DCBlength = sizeof(serialParams);

	BOOL commStat = GetCommState(hComm, &serialParams);      // Retrieves the current settings

	if (commStat == FALSE) {
		return -3;
	}

	serialParams.BaudRate = baudRate;      // Setting BaudRate = 9600
	serialParams.ByteSize = 8;             // Setting ByteSize = 8
	serialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	serialParams.Parity = NOPARITY;        // Setting Parity = None 

	commStat = SetCommState(hComm, &serialParams);  //Configuring the port according to settings in DCB 

	return commStat ? 0 : -4;
}

// This function will halt until something is received on the serial port
int InitializeSerialPortCommunication(HANDLE hComm) {
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE) {
		return -1;
	}

	/*------------------------------------ Setting Receive Mask ----------------------------------------------*/

	BOOL commStat = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

	if (commStat == FALSE) {
		return -2;
	}

	/*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/

	DWORD eventMask;
	commStat = WaitCommEvent(hComm, &eventMask, NULL); //Wait for the character to be received

	return commStat ? 0 : -3;
}

int CloseSerialPort(HANDLE hComm) {
	CloseHandle(hComm); // Closing the Serial Port
	return 0;
}
