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

	MSG msg;
	SystemTrayItem STI;
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	MouseHooker::Start();
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

