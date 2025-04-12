#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/containers/EnumIndexableArray.hpp"

enum class IndexEnum
{
	firstElem = 0x00,
	secondElem = 0x01,
	nonExistent = 0x69
};

TEST_CASE("Enum indexable array at")
{
	EnumIndexableArray<IndexEnum, std::string, 2> array {"first", "second"};

	REQUIRE(array.at(IndexEnum::firstElem) == "first");
	REQUIRE(array.at(IndexEnum::secondElem) == "second");
	REQUIRE(array.at(0) == "first");
	REQUIRE(array.at(1) == "second");

	REQUIRE_THROWS(array.at(IndexEnum::nonExistent));
	REQUIRE_THROWS(array.at(69));
}

TEST_CASE("Enum indexable array []")
{
	EnumIndexableArray<IndexEnum, std::string, 2> array {"first", "second"};

	REQUIRE(array[IndexEnum::firstElem] == "first");
	REQUIRE(array[IndexEnum::secondElem] == "second");
}