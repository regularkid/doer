#include <iostream>
#include "Step_SysCommand.h"

Step_SysCommand::Step_SysCommand(const std::vector<std::string>& params)
	: Step(params)
{}

void Step_SysCommand::Run()
{
	system(m_paramsFullString.c_str());
}