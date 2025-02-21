#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/utility/MetaUtility.hpp"

TEST_CASE("Concatenate string views")
{
	static constexpr std::string_view first = "abc";
	static constexpr std::string_view second = "def";
	static constexpr std::string_view third = "ghi";

	constexpr auto concatenatedResult = MetaUtility::ConcatenateStringViews<first, second, third>::value;
	REQUIRE(concatenatedResult == "abcdefghi");
}


TEST_CASE("Concatenate single")
{
	static constexpr std::string_view first = "abc";

	constexpr auto concatenatedResult = MetaUtility::ConcatenateStringViews<first>::value;
	REQUIRE(concatenatedResult == "abc");
}