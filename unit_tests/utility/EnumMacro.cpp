
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/utility/EnumMacro.hpp"

#define HatColorsSeq(ENTRY, ...) \
	ENTRY(__VA_ARGS__, Red) \
	ENTRY(__VA_ARGS__, Green) \
	ENTRY(__VA_ARGS__, Blue)

DEFINE_ENUM_WITH_STRING(HatColor, HatColorsSeq)
#undef HatColorsSeq

TEST_CASE("Enum macro values")
{
	REQUIRE(HatColor::Red == static_cast<HatColor>(0x00));
	REQUIRE(HatColor::Green == static_cast<HatColor>(0x01));
	REQUIRE(HatColor::Blue == static_cast<HatColor>(0x02));
}

TEST_CASE("Enum macro strings")
{
	REQUIRE(EnumToString(HatColor::Red) == "Red");
	REQUIRE(EnumToString(HatColor::Green) == "Green");
	REQUIRE(EnumToString(HatColor::Blue) == "Blue");
}
