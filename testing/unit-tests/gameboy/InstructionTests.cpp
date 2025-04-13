
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/gameboy/cpu/Cpu.hpp"

namespace TestFixtures
{
	/**
	 * @brief
	 */
	struct GameBoyCpuFixture
	{
		GameBoyCpuFixture() : cpu("/home/king/programming/codam/advanced/play-man/roms/Pokemon_Rouge.gb")
		{

		}

		GameBoy::Cpu cpu;

		size_t ExecuteInstruction(GameBoy::OpCode op)
		{
			return cpu.instructions.at(op)();
		}
	};
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "Nop, 0x00")
{
	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::NOP);

	REQUIRE(numberOfCycles == 1);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_BC_n16, 0x01")
{
	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_BC_n16);

	REQUIRE(numberOfCycles == 3);
}
