#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/utility/MetaUtility.hpp"

#include "play-man/utility/UtilFunc.hpp"

#include <regex>

TEST_CASE("Current Time as String format")
{
	std::regex regexToMatch;
	std::string format;

	SECTION("Format yyyy-mm-dd hh.MM.SS")
	{
		regexToMatch = std::regex("\\d\\d\\d\\d-\\d\\d-\\d\\d \\d\\d.\\d\\d.\\d\\d");
		format = "%Y-%m-%d %H.%M.%S";
	}
	SECTION("Format yyyy-mm-dd")
	{
		regexToMatch = std::regex("\\d\\d\\d\\d-\\d\\d-\\d\\d");
		format = "%Y-%m-%d";
	}
	SECTION("Format hh.MM.SS")
	{
		regexToMatch = std::regex("\\d\\d:\\d\\d:\\d\\d");
		format = "%H:%M:%S";
	}

	INFO("format used: " + format);
	REQUIRE(std::regex_match(Utility::CurrentTimeAsString(format), regexToMatch));
}
