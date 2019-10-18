#pragma once

#include <Windows.h>
#include <vector>
#include "Command.h"

class Console
{
public:
	Console();

	void Run();

private:
	void LoadCommands();

	void DisplayTitle();
	void ProcessInput();
	void UpdateOutput();

	int GetCursorPosX() const;
	int GetCursorPosY() const;
	int GetCursorMaxX() const;
	void SetCursorPos(int x, int y);
	bool CurLineHasMaxCharacters() const;

	void ResetAutoComplete();
	void SetNextAutoCompleteOption();

	std::string m_prompt = "> ";
	std::string m_curInput;
	std::vector<Command> m_commands;

	std::string m_autoCompleteInput;
	unsigned int m_autoCompleteIdx = -1;

	HANDLE m_stdIn = NULL;
	HANDLE m_stdOut = NULL;
};