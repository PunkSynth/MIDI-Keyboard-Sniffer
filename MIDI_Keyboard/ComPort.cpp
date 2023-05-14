#include "pch.h"
#include "ComPort.h"

//Find the present serial ports, returns true if ports found or false if no ports found
bool ComPort::RetrieveComPorts(std::vector<int>& comPortNumbers) const {
	// Clear the comPortNumbers
	std::vector<int>().swap(comPortNumbers);
	
	for (int i = 0; i <= 255; i++) { // checking ports from COM0 to COM255
		CString comName;
		comName.Format(_T("COM%d"), i); // port number in the format COM0, COM1, COM2...

		TCHAR lpTargetPath[CHAR_BUFFER]; // buffer to store the path of the COMPORTS
		ZeroMemory(lpTargetPath, CHAR_BUFFER * sizeof(TCHAR));
		DWORD test = QueryDosDeviceW(comName, (LPWSTR)lpTargetPath, CHAR_BUFFER);

		// Test the return value and error if any
		if (test != 0) { //QueryDosDevice returns zero if it didn't find an object
			comPortNumbers.push_back(i);
		}
	}

	return comPortNumbers.size() ? true : false;
}