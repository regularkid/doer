#include <iostream>
#include "Console.h"

Console::Console()
{
	m_commands.insert({ "test", Command("test") });
}

void Console::Run()
{
	while (1)
	{
		std::cout << "> ";

		std::string command;
		std::cin >> command;
	}
}