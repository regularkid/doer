#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include "Step.h"

class Command
{
public:
	Command() = delete;
	Command(const std::experimental::filesystem::path& path);

	virtual void Run();

	const std::string& GetName() const;
	void AddStep(const std::string& step);

private:
	void Load();

	std::string m_name;
	std::experimental::filesystem::path m_path;
	std::vector<std::unique_ptr<Step>> m_steps;
};