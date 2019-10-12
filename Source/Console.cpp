#include <Windows.h>
#include <iostream>
#include "Console.h"

Console::Console()
{
	// CTRL+SHIFT+J from anywhere to bring Doer window to the front
	RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, 0x4A);

	m_commands.insert({ "test", Command("test") });
}

Console::~Console()
{
	UnregisterHotKey(NULL, 1);
}

void Console::Run()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		switch (msg.message)
		{
			case WM_HOTKEY: BringWindowToFront(); break;
		}
	}

	/*while (1)
	{
		std::cout << "> ";

		std::string command;
		std::cin >> command;
	}*/
}

void Console::BringWindowToFront()
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