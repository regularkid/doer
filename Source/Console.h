#pragma once

#include <unordered_map>
#include "Command.h"

class Console
{
public:
	Console();

	void Run();

private:

	std::unordered_map<std::string, Command> m_commands;
};