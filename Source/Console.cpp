#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include "Console.h"

Console::Console()
{
	m_stdIn = GetStdHandle(STD_INPUT_HANDLE);
	m_stdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// No auto echo to std out
	SetConsoleMode(m_stdIn, 0);

	DisplayTitle();

	LoadCommands();

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

void Console::LoadCommands()
{
	for (const auto& entry : std::experimental::filesystem::directory_iterator("commands"))
	{
		m_commands.emplace_back(entry.path());
	}

	std::cout << std::endl;
}

void Console::DisplayTitle()
{
	std::cout << " ____   __  ____  ____   " << std::endl;
	std::cout << "(    \\ /  \\(  __)(  _ \\  " << std::endl;
	std::cout << " ) D ((  O )) _)  )   /  " << std::endl;
	std::cout << "(____/ \\__/(____)(__\\_)  " << std::endl;
	std::cout << std::endl;
	std::cout << "Copyright Bryan Perfetto 2019" << std::endl;
	std::cout << std::endl;
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
				
				auto itr = std::find_if(m_commands.begin(), m_commands.end(),
					[&](const Command& c) { return c.GetName() == m_curInput; });
				if (itr != m_commands.end())
				{
					Command& command = *itr;
					command.Run();
				}

				m_curInput.clear();
				std::cout << std::endl;
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

			// Clear / Hide
			case VK_ESCAPE:
			{
				m_curInput.clear();
				ResetAutoComplete();

				ShowWindow(GetConsoleWindow(), SW_HIDE);
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
	m_autoCompleteIdx = -1;
	m_autoCompleteInput = m_curInput;
	std::transform(m_autoCompleteInput.begin(), m_autoCompleteInput.end(), m_autoCompleteInput.begin(), std::tolower);
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