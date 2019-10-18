#include <iostream>
#include "Command.h"
#include "Steps/Step_Output.h"

void Command::Run()
{
	std::cout << "Running command '" << m_name << "'..." << std::endl;
	for (std::unique_ptr<Step>& step : m_steps)
	{
		step->Run();
	}
}

const std::string& Command::GetName() const
{
	return m_name;
}

void Command::AddStep(const std::string& step)
{
	// Todo: generalize this
	if (step.find("output") == 0)
	{
		m_steps.push_back(std::make_unique<Step_Output>(step.substr(7, std::string::npos)));
	}
}