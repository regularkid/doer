#pragma once

#include "../Step.h"

class Step_Output : public Step
{
public:
	Step_Output() = delete;
	Step_Output(const std::string& output)
		: m_output(output)
	{}

	void Run() override;

private:
	std::string m_output;
};