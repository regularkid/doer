#pragma once

#include <Windows.h>
#include <unordered_map>
#include "Command.h"

class Console
{
public:
	Console();

	void Run();

private:
	void ProcessInput();
	void UpdateOutput();

	int GetCursorPosX() const;
	int GetCursorPosY() const;
	int GetCursorMaxX() const;
	void SetCursorPos(int x, int y);
	bool CurLineHasMaxCharacters() const;

	std::string m_prompt = "> ";
	std::string m_curInput;
	std::unordered_map<std::string, Command> m_commands;

	HANDLE m_stdIn = NULL;
	HANDLE m_stdOut = NULL;
};