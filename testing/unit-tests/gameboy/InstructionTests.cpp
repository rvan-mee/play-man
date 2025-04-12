
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
		GameBoyCpuFixture() : cpu("someRom")
		{

		}

		GameBoy::Cpu cpu;

		size_t ExecuteInstruction(GameBoy::OpCode op)
		{
			return cpu.instructions.at(op)();
		}
	};
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "Nop")
{
	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::NOP);

	REQUIRE(numberOfCycles == 1);
}
