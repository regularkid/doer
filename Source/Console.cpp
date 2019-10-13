#include <Windows.h>
#include <iostream>
#include "Console.h"

Console::Console()
{
	m_commands.insert({ "test", Command("test") });
}

void Console::Run()
{
	HANDLE hStdout, hStdin;

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleMode(hStdin, 0);

	std::string prompt = "> ";
	std::string curInput;
	while (1)
	{
		INPUT_RECORD irInput;
		DWORD InputsRead = 0;

		ReadConsoleInput(hStdin, &irInput, 1, &InputsRead);

		if (irInput.Event.KeyEvent.bKeyDown)
		{
			WORD virtualKey = irInput.Event.KeyEvent.wVirtualKeyCode;
			if (virtualKey == VK_RETURN)
			{
				// Process command here
				SetCursorPos(0, GetCursorPosY() + 1);
				curInput.clear();
			}
			else if (virtualKey == VK_BACK)
			{
				if (!curInput.empty())
				{
					curInput.pop_back();
				}
			}
			else
			{
				CHAR asciiKey = irInput.Event.KeyEvent.uChar.AsciiChar;
				if (asciiKey != 0)
				{
					curInput += asciiKey;
				}
			}
		}

		CONSOLE_SCREEN_BUFFER_INFO screenInfo;
		GetConsoleScreenBufferInfo(hStdout, &screenInfo);

		DWORD numCharsWritten;
		COORD linePos;
		linePos.X = 0;
		linePos.Y = screenInfo.dwCursorPosition.Y;
		std::string curLine = prompt + curInput;
		curLine += std::string(screenInfo.dwMaximumWindowSize.X - curLine.size() - 1, ' ');
		WriteConsoleOutputCharacter(hStdout, curLine.c_str(), curLine.size(), linePos, &numCharsWritten);

		SetCursorPos(static_cast<SHORT>(prompt.size() + curInput.size()), GetCursorPosY());
	}
}

int Console::GetCursorPosX() const
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenInfo);

	return static_cast<int>(screenInfo.dwCursorPosition.X);
}

int Console::GetCursorPosY() const
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenInfo);

	return static_cast<int>(screenInfo.dwCursorPosition.Y);
}

void Console::SetCursorPos(int x, int y)
{
	COORD cursorPos;
	cursorPos.X = static_cast<SHORT>(x);
	cursorPos.Y = static_cast<SHORT>(y);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
}