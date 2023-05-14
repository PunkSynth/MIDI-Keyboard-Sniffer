
// MIDI_Keyboard.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMIDIKeyboardApp:
// See MIDI_Keyboard.cpp for the implementation of this class
//

class CMIDIKeyboardApp : public CWinApp
{
public:
	CMIDIKeyboardApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMIDIKeyboardApp theApp;
