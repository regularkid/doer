#include "Console.h"
#include "GlobalHotKey.h"

int main()
{
	EnableGlobalHotKey();

	Console console;
	console.Run();

	return 0;
}