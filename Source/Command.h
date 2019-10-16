#pragma once

#include <string>

class Command
{
public:
	Command() = delete;
	Command(const std::string& name)
		: m_name(name)
	{}

	virtual void Run();

	const std::string& GetName() const;

private:
	std::string m_name;
};