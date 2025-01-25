
#include <iostream>

#include "play-man/test.hpp"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	std::cout << "Welcome to play-man! " << SomeTest::GetValue() << std::endl;
	return 0;
}
