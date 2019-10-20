#pragma once

#include <vector>
#include "../Step.h"

class Step_Output : public Step
{
public:
	Step_Output(const std::vector<std::string>& params);
	Step_Output() = delete;

	void Run() override;

private:
};