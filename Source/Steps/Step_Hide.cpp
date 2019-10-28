#include <Windows.h>
#include "Step_Hide.h"

Step_Hide::Step_Hide(const std::vector<std::string>& params)
	: Step(params)
{}

void Step_Hide::Run()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}