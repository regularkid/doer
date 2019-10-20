#include <fstream>
#include <iostream>
#include "Command.h"
#include "Steps/Step_Output.h"
#include "Steps/Step_SysCommand.h"

Command::Command(const std::experimental::filesystem::path& path)
	: m_path(path)
{
	m_name = path.stem().string();
	std::transform(m_name.begin(), m_name.end(), m_name.begin(), std::tolower);

	Load();
}

void Command::Load()
{
	std::cout << "Loading command '" << m_path.filename().string() << "'..." << std::endl;

	std::ifstream infile(m_path);

	std::string line;
	Command* curCommand = nullptr;
	while (std::getline(infile, line))
	{
		if (line.empty())
			continue;

		std::transform(line.begin(), line.end(), line.begin(), std::tolower);

		AddStep(line);
	}
}

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
	else if (step.find("syscommand") == 0)
	{
		m_steps.push_back(std::make_unique<Step_SysCommand>(step.substr(11, std::string::npos)));
	}
}