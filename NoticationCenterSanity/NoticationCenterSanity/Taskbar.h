#pragma once
#include <stdio.h>
#include <Windows.h>

namespace Taskbar
{
	HWND GetShowDesktopButtonHwnd()
	{
		HWND hwnd = NULL;
		HWND parent = FindWindow(L"Shell_TrayWnd", NULL);
		HWND child = FindWindowEx(parent, NULL, L"TrayNotifyWnd", NULL);
		hwnd = FindWindowEx(child, NULL, L"TrayShowDesktopButtonWClass", NULL);
		return hwnd;
	}

	bool HideShowDesktopButton()
	{
		HWND hwnd = GetShowDesktopButtonHwnd();
		bool ret = (bool)ShowWindow(hwnd, 0);
		return ret;
	}
}