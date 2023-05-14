#pragma once

// The WM_MIDIMSG (MIDI message) consists of WPARAM as MIDI Status
// LPARAM as MIDI Data 1, and Data 2
#define WM_MIDIMSG		WM_USER+1

// The WM_MIDISTATUS tells if MIDI keyboard is connected and
// is able to send MIDI data. WPARAM == 1: MIDI Keybord is running
// WPARAM == 0: MIDI Keybord is disconnected
#define WM_MIDISTATUS	WM_USER+2
