#pragma once

#include <string>
#include <vector>

class Step
{
public:
	Step(const std::vector<std::string>& params);

	virtual void Run() = 0;

protected:
	std::vector<std::string> m_params;
	std::string m_paramsFullString;
};