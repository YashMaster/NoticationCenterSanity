#pragma once
#include <Windows.h>
#include <string>
#include "C.h"
#include "Taskbar.h"
#include "Geometry.h"


namespace MouseHooker
{
	unsigned int Counter = 0;
	HHOOK Hook;

	LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		MSLLHOOKSTRUCT * msll = (MSLLHOOKSTRUCT *)lParam;
		printf("Something happened %d\n", Counter++);

		POINT cursor;
		GetCursorPos(&cursor);

		RECT showDesktopButton;
		HWND showDesktopHwnd = Taskbar::GetShowDesktopButtonHwnd();
		GetWindowRect(showDesktopHwnd, &showDesktopButton);

		if (Geometry::IsInRect(cursor, showDesktopButton) == false)
			goto skip;

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


