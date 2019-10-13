#include <iostream>
#include "Console.h"

Console::Console()
{
	m_commands.insert({ "test", Command("test") });

	m_stdIn = GetStdHandle(STD_INPUT_HANDLE);
	m_stdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// No auto echo to std out
	SetConsoleMode(m_stdIn, 0);
}

void Console::Run()
{
	while (1)
	{		
		ProcessInput();
		UpdateOutput();
	}
}

void Console::ProcessInput()
{
	DWORD numInputsRead = 0;
	INPUT_RECORD inputRead;
	ReadConsoleInput(m_stdIn, &inputRead, 1, &numInputsRead);

	// Key was pressed
	if (inputRead.Event.KeyEvent.bKeyDown)
	{
		WORD virtualKey = inputRead.Event.KeyEvent.wVirtualKeyCode;
		switch (virtualKey)
		{
			// Enter
			case VK_RETURN:
			{
				SetCursorPos(0, GetCursorPosY() + 1);
				m_curInput.clear();
			} break;

			// Backspace
			case VK_BACK:
			{
				if (!m_curInput.empty())
				{
					m_curInput.pop_back();
				}
			} break;

			// Escape
			case VK_ESCAPE:
			{
				SetCursorPos(0, GetCursorPosY());
				m_curInput.clear();
			} break;

			// Normal character
			default:
			{
				CHAR asciiKey = inputRead.Event.KeyEvent.uChar.AsciiChar;
				if (asciiKey != 0 && !CurLineHasMaxCharacters())
				{
					m_curInput += asciiKey;
				}
			}
		}
	}
}

void Console::UpdateOutput()
{
	// Fill entire line: prompt + current input + fill spaces to end
	std::string curLine = m_prompt + m_curInput;
	curLine += std::string(GetCursorMaxX() - curLine.size() - 1, ' ');

	DWORD numCharsWritten = 0;
	COORD linePos;
	linePos.X = 0;
	linePos.Y = GetCursorPosY();
	WriteConsoleOutputCharacter(m_stdOut, curLine.c_str(), curLine.size(), linePos, &numCharsWritten);

	// Cursor should be 1 char past prompt + current input
	SetCursorPos(static_cast<SHORT>(m_prompt.size() + m_curInput.size()), GetCursorPosY());
}

int Console::GetCursorPosX() const
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(m_stdOut, &screenInfo);
	return static_cast<int>(screenInfo.dwCursorPosition.X);
}

int Console::GetCursorPosY() const
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(m_stdOut, &screenInfo);
	return static_cast<int>(screenInfo.dwCursorPosition.Y);
}

int Console::GetCursorMaxX() const
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(m_stdOut, &screenInfo);
	return static_cast<int>(screenInfo.dwMaximumWindowSize.X);
}

void Console::SetCursorPos(int x, int y)
{
	COORD cursorPos;
	cursorPos.X = static_cast<SHORT>(x);
	cursorPos.Y = static_cast<SHORT>(y);
	SetConsoleCursorPosition(m_stdOut, cursorPos);
}

bool Console::CurLineHasMaxCharacters() const
{
	const int maxCharacters = GetCursorMaxX() - 1;
	const int numCharacters = static_cast<int>(m_prompt.size()) + static_cast<int>(m_curInput.size());
	return numCharacters >= maxCharacters;
}