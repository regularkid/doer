#include <iostream>
#include "Step_SysCommand.h"

void Step_SysCommand::Run()
{
	system(m_command.c_str());
}