
#include <iostream>

#include "play-man/logger/Logger.hpp"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	LOG_INFO("Welcome to play-man!");
	LOG_WARNING("Welcome to play-man!");
	LOG_ERROR("Welcome to play-man!");
	LOG_DEBUG("Welcome to play-man!");
	return 0;
}
