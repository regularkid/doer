#include <iostream>
#include "Command.h"

void Command::Run()
{
	std::cout << "Running command '" << m_name << "'" << std::endl;
}

const std::string& Command::GetName() const
{
	return m_name;
}