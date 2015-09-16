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

		if (Geometry::IsInRect(cursor, showDesktopButton, false) == false)
			goto skip;

		printf("ShowDesktopButton: %d %d %d %d\n", showDesktopButton.left, showDesktopButton.top, showDesktopButton.right, showDesktopButton.bottom);
		printf("Cursor: %d, %d\n", cursor.x, cursor.y);

		switch (wParam)
		{
		case WM_LBUTTONDOWN:
			//Eat this message (AKA don't pass it on)
			return 1;

		case WM_LBUTTONUP:
			//Send Win+A to invoke the Action Center
			SendShortcut(L"{VK_LWIN}A");
			return 1;

		case WM_RBUTTONDOWN:
			//Eat this message (AKA don't pass it on)
			return 1;

		case WM_RBUTTONUP:
			//Send Win+Tab to invoke the Task Switcher
			SendShortcut(L"{VK_LWIN}{VK_TAB}");

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


