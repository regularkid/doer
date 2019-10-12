#include <Windows.h>
#include <thread>
#include "GlobalHotKey.h"

void BringWindowToFront()
{
	HWND hwnd = GetConsoleWindow();

	WINDOWPLACEMENT place = { sizeof(WINDOWPLACEMENT) };
	GetWindowPlacement(hwnd, &place);
	switch (place.showCmd)
	{
		case SW_SHOWMAXIMIZED: ShowWindow(hwnd, SW_SHOWMAXIMIZED); break;
		case SW_SHOWMINIMIZED: ShowWindow(hwnd, SW_RESTORE);
		default: ShowWindow(hwnd, SW_NORMAL);
	}

	SetForegroundWindow(hwnd);
}

void MessageLoop()
{
	// CTRL+SHIFT+J from anywhere to bring Doer window to the front
	RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, 0x4A);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		switch (msg.message)
		{
			case WM_HOTKEY: BringWindowToFront(); break;
		}
	}
}

void EnableGlobalHotKey()
{
	std::thread messageLoop(MessageLoop);
	messageLoop.detach();
}