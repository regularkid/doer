#pragma once

class Command
{
public:
	Command() = delete;
	Command(const std::string& name)
		: m_name(name)
	{}

	virtual void Run();

private:
	std::string m_name;
};