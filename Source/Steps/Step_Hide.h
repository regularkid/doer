#pragma once

#include "../Step.h"

class Step_Hide : public Step
{
public:
	Step_Hide(const std::vector<std::string>& params);
	Step_Hide() = delete;

	void Run() override;

private:
};