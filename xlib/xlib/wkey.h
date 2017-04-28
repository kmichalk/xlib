#ifndef _X_WKEY_H_
#define _X_WKEY_H_

//
//#include <Windows.h>
//#include "bitliteral.h"
#include "byte.h"

/// <summary>
/// Enumerator converting Windows VirtualKey values to unified and friendly names.
/// </summary>
//enum class WKey: byte
//{
//	Empty = 0,
//	LMB = VK_LBUTTON,
//	RMB = VK_RBUTTON,
//	MMB = VK_MBUTTON,
//	Cancel = VK_CANCEL,
//	BackSpace = VK_BACK,
//	Tab = VK_TAB,
//	Clear = VK_CLEAR,
//	Enter = VK_RETURN,
//	LShift = VK_LSHIFT,
//	RShift = VK_RSHIFT,
//	LCtrl = VK_LCONTROL,
//	RCtrl = VK_RCONTROL,
//	LAlt = VK_LMENU,
//	RAlt = VK_RMENU,
//	Pause = VK_PAUSE,
//	CapsLock = VK_CAPITAL,
//	Esc = VK_ESCAPE,
//	Space = VK_SPACE,
//	PageUp = VK_PRIOR,
//	PageDown = VK_NEXT,
//	End = VK_END,
//	Home = VK_HOME,
//	Left = VK_LEFT,
//	Right = VK_RIGHT,
//	Up = VK_UP,
//	Down = VK_DOWN,
//	Select = VK_SELECT,
//	Print = VK_PRINT,
//	Execute = VK_EXECUTE,
//	PrtScr = VK_SNAPSHOT,
//	Ins = VK_INSERT,
//	Del = VK_DELETE,
//	Help = VK_HELP,
//	KeyNum0 = 0x30, KNum1, KNum2, KNum3, KNum4, KNum5, KNum6, KNum7, KNum8, KNum9,
//	A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
//	Num0 = VK_NUMPAD0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
//	Win = VK_LWIN,
//	Mult = VK_MULTIPLY,
//	Add = VK_ADD,
//	Separator = VK_SEPARATOR,
//	Sub = VK_SUBTRACT,
//	Div = VK_DIVIDE,
//	F1 = VK_F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
//	NumLock = VK_NUMLOCK,
//	ScrollLock = VK_SCROLL,
//	Sleep = VK_SLEEP
//};

enum class WKey: byte
{
	Empty = 0,
	LMB = 0x01,
	RMB = 0x02,
	MMB = 0x04,
	Cancel = 0x03,
	BackSpace = 0x08,
	Tab = 0x09,
	Clear = 0x0C,
	Enter = 0x0D,
	LShift = 0xA0,
	RShift = 0xA1,
	LCtrl = 0xA2,
	RCtrl = 0xA3,
	LAlt = 0xA4,
	RAlt = 0xA5,
	Pause = 0x13,
	CapsLock = 0x14,
	Esc = 0x1B,
	Space = 0x20,
	PageUp = 0x21,
	PageDown = 0x22,
	End = 0x23,
	Home = 0x24,
	Left = 0x25,
	Right = 0x27,
	Up = 0x26,
	Down = 0x28,
	Select = 0x29,
	Print = 0x2A,
	Execute = 0x2B,
	PrtScr = 0x2C,
	Ins = 0x2D,
	Del = 0x2E,
	Help = 0x2F,
	KNum0 = 0x30, KNum1, KNum2, KNum3, KNum4, KNum5, KNum6, KNum7, KNum8, KNum9,
	A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Num0 = 0x60, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
	Win = 0x5B,
	Mult = 0x6A,
	Add = 0x6B,
	Separator = 0x6C,
	Sub = 0x6D,
	Div = 0x6F,
	F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	NumLock = 0x90,
	ScrollLock = 0x91,
	Sleep = 0x5F
};

//inline bool isPressed(WKey key)
//{
//	return GetKeyState((int)key) & x::FIRST_BIT<SHORT>;
//}
//
//inline bool isPressed(BYTE key)
//{
//	return GetKeyState(key) & x::FIRST_BIT<SHORT>;
//}

bool isPressed(WKey key);

bool isPressed(byte key);

#endif //_X_WKEY_h_