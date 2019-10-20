#include "Step.h"

Step::Step(const std::vector<std::string>& params)
	: m_params(params)
{
	for (size_t i = 0; i < params.size(); ++i)
	{
		m_paramsFullString += params[i];
		if (i < params.size() - 1)
		{
			m_paramsFullString += " ";
		}
	}
}