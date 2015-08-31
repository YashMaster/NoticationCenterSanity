#pragma once
#include <Windows.h>
#include <string>
#include "C.h"

unsigned int q = 0;
static HHOOK g_hhook;




LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT * kbll = (KBDLLHOOKSTRUCT *)lParam;
	kbll->flags ^= 16;
	return(CallNextHookEx(g_hhook, nCode, wParam, lParam));
}

class Hooker
{
protected:
	//HHOOK g_hhook;

public:

	enum class Type 
	{ 
		Mouse, 
		Keyboard 
	};

	Hooker(Type type)
	{
		if (type == Type::Mouse)
		{
			g_hhook = NULL;
			g_hhook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(NULL), NULL);
		}
		else if (type == Type::Keyboard)
		{
			g_hhook = NULL;
			g_hhook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), NULL);
		}
	}


	~Hooker()
	{
		UnhookWindowsHookEx(g_hhook);
		g_hhook = NULL;
	}

	static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		MSLLHOOKSTRUCT * msll = (MSLLHOOKSTRUCT *)lParam;
		printf("Something happened %d\n", q);


		if (nCode == HC_ACTION)
		{
			switch (wParam)
			{
			case WM_LBUTTONDOWN: //Get the first point
				return 1;
			case WM_LBUTTONUP: //Get the second point
				return 1;
			}
		}

		return CallNextHookEx(g_hhook, nCode, wParam, lParam);
	}
};


