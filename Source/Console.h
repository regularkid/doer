#pragma once

#include <unordered_map>
#include "Command.h"

class Console
{
public:
	Console();

	void Run();

private:
	int GetCursorPosX() const;
	int GetCursorPosY() const;
	void SetCursorPos(int x, int y);

	std::unordered_map<std::string, Command> m_commands;
};