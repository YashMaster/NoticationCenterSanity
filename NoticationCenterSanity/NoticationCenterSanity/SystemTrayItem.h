#pragma once

#include <windows.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <thread>
#include <vector>
#include "C.h"

#define MY_WM_TRAY (WM_APP+30) //Define the magic for sys tray 
#define CONTEXT_MENU_MSG 900 //high number so default action doesn't take place


//Example of usage:
/*
 
 SystemTrayItem STI;
 auto mi0OnExecute = [](MenuItem::State state)
 {
 printf("state was %d\n", state);
 };
 MenuItem mi0(L"Something", mi0OnExecute);
 STI.AddItem(mi0);

*/


class MenuItem
{
public:
	enum class Type
	{
		Action,
		SubMenu,
		CheckBox,
		Count
	};

	enum class State
	{
		Invoked,
		Checked,
		Unchecked,
		Count
	};

	std::wstring Title = L"Null";
	std::function<void(State)> OnExecute = [&](State state) { printf("Did nothing in MenuItem\n"); return; };

	MenuItem(std::wstring title, std::function<void(State)> onExecute)
	{
		Title = title;
		OnExecute = onExecute;
	}

};


class SystemTrayItem
{
private:
	NOTIFYICONDATA nid = { 0 };
	std::thread Worker;
	wchar_t *wClassName = L"SystemTrayClassname";
	wchar_t *wTitleText = L"SystemTrayTitleText";
	wchar_t *wToolTip = L"SystemTrayToolTip";
	std::vector<MenuItem> Items;

public:

	SystemTrayItem()
	{
		Worker = std::thread{ [this]() { Init(); } };
	}
	
	~SystemTrayItem()
	{
		Shell_NotifyIcon(NIM_DELETE, &nid);
	}

	void AddItem(MenuItem mi)
	{
		Items.push_back(mi);
	}

	BOOL ShowPopupMenu(HWND hWnd)
	{
		//Add items to context menu
		HMENU hPop = CreatePopupMenu();

		//Add Exit to the context menu
		int i = 0;
		for (i = 0; i < Items.size(); i++)
		{
			InsertMenu(hPop, i, MF_BYPOSITION | MF_STRING, CONTEXT_MENU_MSG + i, Items[i].Title.c_str());
		}

		InsertMenu(hPop, i, MF_BYPOSITION | MF_STRING, CONTEXT_MENU_MSG + i, L"Exit");

		//Set default item
		SetFocus(hWnd);
		SendMessage(hWnd, WM_INITMENUPOPUP, (WPARAM)hPop, 0);

		//Display the context menu
		POINT pt;
		GetCursorPos(&pt);
		WORD cmd = TrackPopupMenu(hPop, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hWnd, NULL);
		
		//Send callback message to the application handle (window)
		SendMessage(hWnd, WM_COMMAND, cmd, 0);
		DestroyMenu(hPop);

		return 0;
	}

	void Init()
	{
		HWND hwnd;
		MSG messages;
		WNDCLASSEX wincl = { 0 };

		//wincl.hInstance = hThisInstance;
		wincl.lpszClassName = wClassName;
		wincl.lpfnWndProc = WndProc;
		wincl.style = CS_DBLCLKS;
		wincl.cbSize = sizeof(WNDCLASSEX);
		wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
		wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
		RegisterClassEx(&wincl);

		//Create the HWND and embed a pointer to @this instance. We'll need the ptr later.
		hwnd = CreateWindowEx(0, wClassName, wTitleText, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 544, 375, HWND_DESKTOP, NULL, NULL, NULL);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (long)this);


		//The Tray Icon structure
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = hwnd;
		nid.uID = 30;
		nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		nid.uCallbackMessage = MY_WM_TRAY;
		HMODULE shell32 = LoadLibraryEx(L"shell32.dll", NULL, LOAD_LIBRARY_AS_DATAFILE);
		nid.hIcon = LoadIcon(shell32, MAKEINTRESOURCE(14));
		wsprintf(nid.szTip, wToolTip);
		Shell_NotifyIcon(NIM_ADD, &nid);

		while (GetMessage(&messages, NULL, 0, 0))
		{
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}

		return;
	}


	//WndProc has to be static. So we embedded a pointer to @this via SetWindowLongPtr()
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		SystemTrayItem *me = (SystemTrayItem*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (me) return me->realWndProc(hwnd, msg, wParam, lParam);
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

private:
	LRESULT CALLBACK realWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CLOSE:
			SendMessage(hwnd, WM_DESTROY, NULL, NULL);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			Shell_NotifyIcon(NIM_DELETE, &nid); 
			break;

		case MY_WM_TRAY: //A message arrives from the tray, what could it be?
			switch (lParam)
			{
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				ShowPopupMenu(hwnd);
				break;
			}
			break;

		case WM_COMMAND: //A message comes from the context menu!
		{
			int cmd = (int)wParam - CONTEXT_MENU_MSG;
			if (cmd == Items.size())
			{
				SendMessage(hwnd, WM_DESTROY, NULL, NULL);
				_exit(0);
			}
			else if (cmd < Items.size() && cmd >= 0)
			{
				Items[cmd].OnExecute(MenuItem::State::Invoked);
			}
			else
			{
				printf("Default: Hit an unhandled case (%d)\n", cmd);
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		return 0;
	}
};

