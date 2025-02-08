
#include <iostream>

#include "play-man/test.hpp"
#include <play-man/settings/PlayManSettings.hpp>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	PlayManSettings settings = PlayManSettings::ReadFromFile("settings.json");
	Logger::LogInterface::Initialize(settings.logDirectory, settings.logLevel);
	
	std::cout << settings.ToString() << std::endl;
	return 0;
}
