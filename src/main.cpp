
#include <iostream>

#include "play-man/logger/Logger.hpp"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	Logger::LogInterface::Initialize("Logging", Logger::LogLevel::Debug);

	LOG_INFO("Welcome to play-man!");
	LOG_INFO_STREAM << "Welcome to play-man!\n";

	LOG_WARNING("Welcome to play-man!");
	LOG_WARNING_STREAM << "Welcome to play-man!\n";

	LOG_ERROR("Welcome to play-man!");
	LOG_ERROR_STREAM << "Welcome to play-man!\n";

	LOG_FATAL("Welcome to play-man!");
	LOG_FATAL_STREAM << "Welcome to play-man!\n";

	LOG_DEBUG("Welcome to play-man!");
	LOG_DEBUG_STREAM << "Welcome to play-man!\n";
	return 0;
}
