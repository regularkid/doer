#pragma once

#include <unordered_map>
#include "Command.h"

class Console
{
public:
	Console();
	~Console();

	void Run();

private:
	void BringWindowToFront();

	std::unordered_map<std::string, Command> m_commands;
};