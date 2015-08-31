#pragma once
#include <Windows.h>
#include <string>
#include "C.h"
#include "Taskbar.h"
#include "Geometry.h"
#include "Keyboard.h"

namespace MouseHooker
{
	unsigned int Counter = 0;
	HHOOK Hook;

	LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode != HC_ACTION)
			goto skip;


		POINT cursor;
		GetCursorPos(&cursor);

		RECT showDesktopButton;
		HWND showDesktopHwnd = Taskbar::GetShowDesktopButtonHwnd();
		GetWindowRect(showDesktopHwnd, &showDesktopButton);

		if (Geometry::IsInRect(cursor, showDesktopButton) == false)
			goto skip;


		switch (wParam)
		{
		case WM_LBUTTONDOWN:
			return 1;

		case WM_LBUTTONUP:
			SendShortcut(L"{VK_LWIN}A");
			return 1;
		}


	skip:
		return CallNextHookEx(Hook, nCode, wParam, lParam);
	}

	void Start()
	{
		Hook = NULL;
		Hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(NULL), NULL);
	}

	void Stop()
	{
		UnhookWindowsHookEx(Hook);
		Hook = NULL;
	}

};


