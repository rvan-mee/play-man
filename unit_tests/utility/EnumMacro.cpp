
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/utility/EnumMacro.hpp"

#define COLOR_DEFINITION(EXPANSION_MACRO, enum_class_name) \
    EXPANSION_MACRO(enum_class_name, Red, 2) \
    EXPANSION_MACRO(enum_class_name, Green, 4) \
    EXPANSION_MACRO(enum_class_name, Blue, 6)

CREATE_ENUM_WITH_UTILS(COLOR_DEFINITION, Color);

#undef COLOR_DEFINITION

// ***************************************************************

#define SECOND_COLOR_DEFINITION(EXPANSION_MACRO, enum_class_name) \
    EXPANSION_MACRO(enum_class_name, Red, 1) \
    EXPANSION_MACRO(enum_class_name, Green, 2) \
    EXPANSION_MACRO(enum_class_name, Blue, 3)

namespace Second {
    CREATE_ENUM_WITH_UTILS(SECOND_COLOR_DEFINITION, Color);
}

#undef SECOND_COLOR_DEFINITION

// ***************************************************************

#define DEFAULT_ENUM_DEFINITION(EXPANSION_MACRO, enum_class_name) \
    EXPANSION_MACRO(enum_class_name, A) \
    EXPANSION_MACRO(enum_class_name, B) \
    EXPANSION_MACRO(enum_class_name, C) \
    EXPANSION_MACRO(enum_class_name, D)

CREATE_ENUM_WITH_UTILS(DEFAULT_ENUM_DEFINITION, DefaultEnum);

#undef DEFAULT_ENUM_DEFINITION

// ***************************************************************

#define SECOND_DEFAULT_ENUM_DEFINITION(EXPANSION_MACRO, enum_class_name) \
    EXPANSION_MACRO(enum_class_name, A) \
    EXPANSION_MACRO(enum_class_name, B) \
    EXPANSION_MACRO(enum_class_name, C) \
    EXPANSION_MACRO(enum_class_name, D) \
	EXPANSION_MACRO(enum_class_name, E)

namespace Second {
	CREATE_ENUM_WITH_UTILS(SECOND_DEFAULT_ENUM_DEFINITION, DefaultEnum);
}

#undef SECOND_DEFAULT_ENUM_DEFINITION

// ***************************************************************

TEST_CASE("Defined enum macro values")
{
	REQUIRE(Color::Red == static_cast<Color>(0x02));
	REQUIRE(Color::Green == static_cast<Color>(0x04));
	REQUIRE(Color::Blue == static_cast<Color>(0x06));
}

TEST_CASE("Defined enum macro strings")
{
	REQUIRE(GetEnumAsString(Color::Red) == "Red");
	REQUIRE(GetEnumAsString(Color::Green) == "Green");
	REQUIRE(GetEnumAsString(Color::Blue) == "Blue");
}

TEST_CASE("Defined enum in namespace macro strings")
{
	REQUIRE(GetEnumAsString(Second::Color::Red) == "Red");
	REQUIRE(GetEnumAsString(Second::Color::Green) == "Green");
	REQUIRE(GetEnumAsString(Second::Color::Blue) == "Blue");
}

TEST_CASE("Defined enum in namespace macro values")
{
	REQUIRE(Second::Color::Red == static_cast<Second::Color>(0x01));
	REQUIRE(Second::Color::Green == static_cast<Second::Color>(0x02));
	REQUIRE(Second::Color::Blue == static_cast<Second::Color>(0x03));
}

TEST_CASE("Default enum macro strings")
{
	REQUIRE(GetEnumAsString(DefaultEnum::A) == "A");
	REQUIRE(GetEnumAsString(DefaultEnum::B) == "B");
	REQUIRE(GetEnumAsString(DefaultEnum::C) == "C");
	REQUIRE(GetEnumAsString(DefaultEnum::D) == "D");
}

TEST_CASE("Default enum macro values")
{
	REQUIRE(DefaultEnum::A == static_cast<DefaultEnum>(0x00));
	REQUIRE(DefaultEnum::B == static_cast<DefaultEnum>(0x01));
	REQUIRE(DefaultEnum::C == static_cast<DefaultEnum>(0x02));
	REQUIRE(DefaultEnum::D == static_cast<DefaultEnum>(0x03));
}

TEST_CASE("Default enum in namespace macro strings")
{
	REQUIRE(GetEnumAsString(Second::DefaultEnum::A) == "A");
	REQUIRE(GetEnumAsString(Second::DefaultEnum::B) == "B");
	REQUIRE(GetEnumAsString(Second::DefaultEnum::C) == "C");
	REQUIRE(GetEnumAsString(Second::DefaultEnum::D) == "D");
	REQUIRE(GetEnumAsString(Second::DefaultEnum::E) == "E");
}

TEST_CASE("Default enum in namespace macro values")
{
	REQUIRE(Second::DefaultEnum::A == static_cast<Second::DefaultEnum>(0x00));
	REQUIRE(Second::DefaultEnum::B == static_cast<Second::DefaultEnum>(0x01));
	REQUIRE(Second::DefaultEnum::C == static_cast<Second::DefaultEnum>(0x02));
	REQUIRE(Second::DefaultEnum::D == static_cast<Second::DefaultEnum>(0x03));
	REQUIRE(Second::DefaultEnum::E == static_cast<Second::DefaultEnum>(0x04));
}
