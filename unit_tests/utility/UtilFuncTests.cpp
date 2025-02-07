#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/utility/MetaUtility.hpp"

#include "play-man/utility/UtilFunc.hpp"

#include <regex>

/////////////////////////
// CurrentTimeAsString //
/////////////////////////

TEST_CASE("Current time as string format `yyyy-mm-dd hh.MM.SS`")
{
	std::regex regexToMatch("\\d\\d\\d\\d-\\d\\d-\\d\\d \\d\\d.\\d\\d.\\d\\d");
	REQUIRE(std::regex_match(Utility::CurrentTimeAsString("%Y-%m-%d %H.%M.%S"), regexToMatch));
}

TEST_CASE("Current time as string format `yyyy-mm-dd`")
{
	std::regex regexToMatch("\\d\\d\\d\\d-\\d\\d-\\d\\d");
	REQUIRE(std::regex_match(Utility::CurrentTimeAsString("%Y-%m-%d"), regexToMatch));
}

TEST_CASE("Current time as string format `hh.MM.SS`")
{
	std::regex regexToMatch("\\d\\d:\\d\\d:\\d\\d");
	REQUIRE(std::regex_match(Utility::CurrentTimeAsString("%H:%M:%S"), regexToMatch));
}

TEST_CASE("Current time as string format `yyyy :) %S`")
{
	std::regex regexToMatch("\\d\\d\\d\\d :\\) \\d\\d");
	REQUIRE(std::regex_match(Utility::CurrentTimeAsString("%Y :) %S"), regexToMatch));
}

/////////////
// Replace //
/////////////

TEST_CASE("Replace word in string")
{
	std::string originalString("This is a stringstring for testing purposes");

	SECTION("Single word")
	{
		Utility::Replace(originalString, "is a", "could be a");
		REQUIRE(originalString == "This could be a stringstring for testing purposes");
	}
	SECTION("Connected words")
	{
		Utility::Replace(originalString, "string", "stringstring");
		REQUIRE(originalString == "This is a stringstringstringstring for testing purposes");
	}
	SECTION("Entire string")
	{
		Utility::Replace(originalString, "This is a stringstring for testing purposes", "test");
		REQUIRE(originalString == "test");
	}
	SECTION("Replacing word contains word to replace")
	{
		Utility::Replace(originalString, "for", "fforce");
		REQUIRE(originalString == "This is a stringstring fforce testing purposes");
	}
	SECTION("Replacing nothing")
	{
		Utility::Replace(originalString, "something not in the string", "somethin in the string");
		REQUIRE(originalString == "This is a stringstring for testing purposes");
	}
}
