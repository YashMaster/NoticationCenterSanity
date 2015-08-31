#include <stdio.h>
#include <Windows.h>
#include "RedirectIO.h"
#include "MouseHooker.h"
#include "Hooker.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	RedirectIO::ToConsole();
	printf("Starting...\n");


	MouseHooker::Start();

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		printf("q = %d\n", q);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

