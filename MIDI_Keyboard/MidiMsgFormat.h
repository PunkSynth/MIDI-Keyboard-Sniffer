
#ifndef MIDIMSGFORMAT_H
#define MIDIMSGFORMAT_H

#define NOTE_OFF					0x8
#define NOTE_ON						0x9
#define POLYFONIC_AFTERTOUCH		0xA
#define CONTROL_CHANGE				0xB
#define PROGRAM_CHANGE				0xC
#define CHANNEL_AFTERTOUCH			0xD
#define PITCH_WHEEL					0xE
#define SYS_MSG						0xF

#define NOTE_OFF_STR				_T("Note Off")
#define NOTE_ON_STR					_T("Note On")
#define POLYFONIC_AFTERTOUCH_STR	_T("Poly Aftertouch")
#define CONTROL_CHANGE_STR			_T("Ctrl Change")
#define PROGRAM_CHANGE_STR			_T("Prog Change")
#define CHANNEL_AFTERTOUCH_STR		_T("Chan Aftertouch")
#define PITCH_WHEEL_STR				_T("Pitch Wheel")

#define NOTE_NUMBER_STR				_T("NN: %ws (%d)")
#define CTRL_NUMBER_STR				_T("Ctrl No.: %d")
#define PROG_NUMBER_STR				_T("Prog No.: %d")

#define VELOCITY_STR				_T("Velocity: %d")
#define PRESSURE_STR				_T("Pressure: %d")
#define LSB_STR						_T("LSB: %d")
#define MSB_STR						_T("MSB: %d")
#define NA_STR						_T("N/A")
#define DATA_STR					_T("Data: %d")
#define DATA1_STR					_T("Data1: %d")
#define DATA2_STR					_T("Data2: %d")

// Data1 When Message is 0xBn (n = channel)
// The values below can be 0x21, 0x22, ..., 0x27
#define MODULATION_WHEEL			0x01
#define MODULATION_WHEEL_2			0x21
#define BREATH_CONTROLLER			0x02
#define BREATH_CONTROLLER_2			0x22
#define FOOT_CONTROLLER				0x04
#define FOOT_CONTROLLER_2			0x24
#define PORTAMENTO_TIME				0x05
#define PORTAMENTO_TIME_2			0x25
#define DATA_ENTRY_SLIDER			0x06
#define DATA_ENTRY_SLIDER_2			0x26
#define MAIN_VOLUME					0x07
#define MAIN_VOLUME_2				0x27

#define MODULATION_WHEEL_STR		_T("Mod Wheel")
#define BREATH_CONTROLLER_STR		_T("Breath Ctrl")
#define FOOT_CONTROLLER_STR			_T("Foot Ctrl")
#define PORTAMENTO_TIME_STR			_T("Portamento Time")
#define DATA_ENTRY_SLIDER_STR		_T("Data Entry Slider")
#define MAIN_VOLUME_STR				_T("Main Volume")

#endif //MIDIMSGFORMAT_H