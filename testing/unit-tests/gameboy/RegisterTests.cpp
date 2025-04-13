
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/gameboy/cpu/Register.hpp"

TEST_CASE("Accessing bytes in register")
{
	GameBoy::Register reg(0xFFAA);

	REQUIRE(reg.LowByte() == 0xAA);
	REQUIRE(reg.HighByte() == 0xFF);
	REQUIRE(reg.Value() == 0xFFAA);
}

TEST_CASE("Writing low byte")
{
	GameBoy::Register reg(0xFFAA);
	
	reg.SetLowByte(0x42);
	REQUIRE(reg.LowByte() == 0x42);
	REQUIRE(reg.HighByte() == 0xFF);
	REQUIRE(reg.Value() == 0xFF42);
}

TEST_CASE("Writing hight byte")
{
	GameBoy::Register reg(0xFFAA);
	
	reg.SetHighByte(0x69);
	REQUIRE(reg.LowByte() == 0xAA);
	REQUIRE(reg.HighByte() == 0x69);
	REQUIRE(reg.Value() == 0x69AA);
}

TEST_CASE("Writing two bytes")
{
	GameBoy::Register reg(0xFFAA);
	
	reg.SetValue(0x1234);
	REQUIRE(reg.LowByte() == 0x34);
	REQUIRE(reg.HighByte() == 0x12);
	REQUIRE(reg.Value() == 0x1234);
}
