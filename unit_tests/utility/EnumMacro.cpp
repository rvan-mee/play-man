
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/utility/EnumMacro.hpp"

#define HatColorsSeq(ENTRY, enumName) \
	ENTRY(enumName, Red) \
	ENTRY(enumName, Green) \
	ENTRY(enumName, Blue)

DEFINE_ENUM_WITH_STRING(HatColor, HatColorsSeq)
#undef HatColorsSeq

TEST_CASE("Enum macro")
{
	REQUIRE(HatColor::Red == static_cast<HatColor>(0x00));
	REQUIRE(HatColor::Green == static_cast<HatColor>(0x01));
	REQUIRE(HatColor::Blue == static_cast<HatColor>(0x02));
}
