#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Step.h"

class Command
{
public:
	Command() = delete;
	Command(const std::string& name)
		: m_name(name)
	{}

	virtual void Run();

	const std::string& GetName() const;
	void AddStep(const std::string& step);

private:
	std::string m_name;
	std::vector<std::unique_ptr<Step>> m_steps;
};