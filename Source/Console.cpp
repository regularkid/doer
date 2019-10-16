#include <iostream>
#include <algorithm>
#include "Console.h"

Console::Console()
{
	m_commands.emplace_back("test");
	m_commands.emplace_back("test2");
	m_commands.emplace_back("blah");
	m_commands.emplace_back("log");
	std::sort(m_commands.begin(), m_commands.end(), [](const Command& a, const Command& b) {
		return a.GetName() < b.GetName();
	});

	m_stdIn = GetStdHandle(STD_INPUT_HANDLE);
	m_stdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// No auto echo to std out
	SetConsoleMode(m_stdIn, 0);

	// Initialize output (ProcessInput is blocking if no input pressed, so
	// UpdateOutput inside ::Run may not be called until user enters something
	UpdateOutput();
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
			// Process command
			case VK_RETURN:
			{
				SetCursorPos(0, GetCursorPosY() + 1);
				m_curInput.clear();
			} break;

			// Delete character
			case VK_BACK:
			{
				if (!m_curInput.empty())
				{
					m_curInput.pop_back();
					ResetAutoComplete();
				}
			} break;

			// Clear
			case VK_ESCAPE:
			{
				m_curInput.clear();
				ResetAutoComplete();
			} break;

			// Auto-complete
			case VK_TAB:
			{
				SetNextAutoCompleteOption();
			} break;

			// Normal character
			default:
			{
				CHAR asciiKey = inputRead.Event.KeyEvent.uChar.AsciiChar;
				if (asciiKey != 0 && !CurLineHasMaxCharacters())
				{
					m_curInput += asciiKey;
					ResetAutoComplete();
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

void Console::ResetAutoComplete()
{
	m_autoCompleteInput = m_curInput;
	m_autoCompleteIdx = -1;
}

void Console::SetNextAutoCompleteOption()
{
	if (m_autoCompleteInput.empty())
	{
		return;
	}

	// Find command (if any) matching the currently entered input starting
	// from the current auto complete command index (with wrap around)
	int numCommandsToCheck = m_commands.size();
	while (numCommandsToCheck-- > 0)
	{
		m_autoCompleteIdx = (m_autoCompleteIdx + 1) % m_commands.size();

		const std::string& commandName = m_commands[m_autoCompleteIdx].GetName();
		if (commandName.find(m_autoCompleteInput) == 0)
		{
			m_curInput = m_commands[m_autoCompleteIdx].GetName();
			break;
		}
	}
}