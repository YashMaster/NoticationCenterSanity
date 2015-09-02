#include <stdio.h>
#include <Windows.h>
#include "RedirectIO.h"
#include "MouseHooker.h"
#include "SystemTrayItem.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	RedirectIO::ToConsole();
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);


	printf("Overloaded the 'Show Desktop' button with the 'Show Action Center' command...\n");
	MouseHooker::Start();
	SystemTrayItem STI;



	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	MouseHooker::Stop();
	return (int)msg.wParam;
}

