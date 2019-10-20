#include <iostream>
#include "Step_Output.h"

Step_Output::Step_Output(const std::vector<std::string>& params)
	: Step(params)
{
}

void Step_Output::Run()
{
	std::cout << m_paramsFullString << std::endl;
}