
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/signal/Signal.hpp"

TEST_CASE("single target")
{
	int i = 0;

	auto someCallback = [&]()
	{
		i++;
	};

	Signal<void()> sig;
	sig.Connect(someCallback);
	sig();

    REQUIRE(i == 1);

	sig();

	REQUIRE(i == 2);
}


TEST_CASE("signal disconnect")
{
	int i = 0;

	auto someCallback = [&]()
	{
		i++;
	};

	Signal<void()> sig;
	sig.Connect(someCallback);
	sig.Disconnect(someCallback);
	sig();

	REQUIRE(i == 0);
}

TEST_CASE("multiple targets")
{
	int i = 0;

	auto someCallback = [&]()
	{
		i++;
	};

	auto anotherCallback = [&]()
	{
		i += 3;
	};

	Signal<void()> sig;
	sig.Connect(someCallback);
	sig.Connect(anotherCallback);
	sig();

	REQUIRE(i == 4);
}