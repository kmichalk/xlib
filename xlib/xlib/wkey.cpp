#include "wkey.h"
#include "bitliteral.h"
#include <Windows.h>

bool isPressed(WKey key)
{
	return GetKeyState((int)key) & x::FIRST_BIT<SHORT>;
}

bool isPressed(byte key)
{
	return GetKeyState(key) & x::FIRST_BIT<SHORT>;
}

