#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include "Command.h"
#include "Steps/Step_Output.h"
#include "Steps/Step_SysCommand.h"

Command::Command(const std::experimental::filesystem::path& path)
	: m_path(path)
{
	m_name = path.stem().string();
	std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);

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

		std::transform(line.begin(), line.end(), line.begin(), ::tolower);

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
	// Super simple factory
#define CREATE_STEP(className) [](const std::vector<std::string>& params) -> std::unique_ptr<##className> \
	{ return std::make_unique<##className>(params); }

	using createStepFunc = std::function<std::unique_ptr<Step>(const std::vector<std::string>& params)>;
	std::unordered_map<std::string, createStepFunc> createStepFuncs =
	{
		{"output", CREATE_STEP(Step_Output)},
		{"syscommand", CREATE_STEP(Step_SysCommand)},
	};

#undef CREATE_STEP

	// Parse line: first word = step name, remainder are params
	std::istringstream iss(step);
	std::vector<std::string> params(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
	std::string stepName = params.front();
	params.erase(params.begin());

	// Create step from factory
	const auto& itr = createStepFuncs.find(stepName);
	if (itr != createStepFuncs.end())
	{
		m_steps.push_back(itr->second(params));
	}
}