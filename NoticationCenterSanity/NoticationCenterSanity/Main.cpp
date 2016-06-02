#include <stdio.h>
#include <Windows.h>
#include "RedirectIO.h"
#include "MouseHooker.h"
#include "SystemTrayItem.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef DEBUG
	RedirectIO::ToConsole();
#endif
	RedirectIO::ToConsole();
	MSG msg;

	SystemTrayItem STI;

	SendShortcut(L"{VK_CONTROL}{VK_LWIN}{VK_F24}");

	//MouseHooker::Start();
	printf("Overloaded the 'Show Desktop' button with the 'Show Action Center' command...\n");

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	MouseHooker::Stop();
	return (int)msg.wParam;
}

