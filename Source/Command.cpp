#include <iostream>
#include "Command.h"

void Command::Run()
{
	std::cout << "Running command '" << m_name << "'" << std::endl;
}