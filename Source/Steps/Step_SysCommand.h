#pragma once

#include "../Step.h"

class Step_SysCommand : public Step
{
public:
	Step_SysCommand() = delete;
	Step_SysCommand(const std::string& command)
		: m_command(command)
	{}

	void Run() override;

private:
	std::string m_command;
};