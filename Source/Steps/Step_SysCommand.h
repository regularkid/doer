#pragma once

#include "../Step.h"

class Step_SysCommand : public Step
{
public:
	Step_SysCommand(const std::vector<std::string>& params);
	Step_SysCommand() = delete;

	void Run() override;

private:
};