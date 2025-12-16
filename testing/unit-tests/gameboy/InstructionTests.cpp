
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/gameboy/cpu/Cpu.hpp"

#define GB_ROM_PATH "../test-data/custom_gb_test_roms/"

namespace TestFixtures
{
	/**
	 * @brief
	 */
	struct GameBoyCpuFixture
	{
		GameBoyCpuFixture()
			: cpu(GB_ROM_PATH "test_rom.gb")
			, AF(cpu.core.AF)
			, BC(cpu.core.BC)
			, DE(cpu.core.DE)
			, HL(cpu.core.HL)
			, SP(cpu.core.SP)
			, PC(cpu.core.PC)
			, IE(cpu.core.IE)
		{

		}

		GameBoy::Cpu 		cpu;
		GameBoy::Register&	AF;
        GameBoy::Register&	BC;
        GameBoy::Register&	DE;
        GameBoy::Register&	HL;
        GameBoy::Register&	SP;
        GameBoy::Register&	PC;
        uint8_t&			IE;

		size_t ExecuteInstruction(GameBoy::OpCode op)
		{
			return cpu.instructions.at(op)(&cpu);
		}

		void LoadTestRom(const char *filePath)
		{
			cpu.LoadTestRom(filePath);
		}

	};
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "Nop, 0x00")
{
	const auto pcBefore = PC.Value();
	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::NOP);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);

	REQUIRE(PC.Value() == pcBefore);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_BC_n16, 0x01")
{
	// File containing the bytes 0xF0 0x0F
	// The first byte should become the lower half of the 16 bit register.
	LoadTestRom(GB_ROM_PATH "load_immediate_16.gb");
	const auto pcBefore = PC.Value();
	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_BC_n16);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x0F'F0);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);

	const auto expectedPC = pcBefore + 2;
	REQUIRE(PC.Value() == expectedPC);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "JP_a16, 0xC3")
{
	// File containing the bytes 0xF0 0x0F
	// The first byte should become the lower half of the 16 bit register.
	LoadTestRom(GB_ROM_PATH "load_immediate_16.gb");
	REQUIRE(PC.Value() == 0x00'00);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JP_a16);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x0F'F0);
	REQUIRE(IE == 0x00);
}
