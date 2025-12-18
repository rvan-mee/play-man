
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/gameboy/cpu/Cpu.hpp"

#define GB_ROM_PATH "../test-data/custom_gb_test_roms/"

// Address for the interrupt register 
constexpr uint16_t interruptAddress = 0xFFFF;

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

		void ClearRegisters()
		{
			AF.SetValue(0x0000);
			BC.SetValue(0x0000);
			DE.SetValue(0x0000);
			HL.SetValue(0x0000);
			SP.SetValue(0x0000);
			PC.SetValue(0x0000);
			IE = 0x00;
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

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_BC_NI_A, 0x02")
{
	ClearRegisters();

	// Set the address inside the BC register and the value in A
	BC.SetValue(interruptAddress);
	AF.SetHighByte(0xF0);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_BC_NI_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0xF0'00);
	REQUIRE(BC.Value() == interruptAddress);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0xF0);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_BC, 0x03")
{
	ClearRegisters();

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_BC);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'01);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_B, 0x04")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'00000000);
	REQUIRE(BC.Value() == 0x01'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'00100000);
	REQUIRE(BC.Value() == 0x10'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'10100000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
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

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "CP_A_n8, 0xFE")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(AF.Value() == 0x00'00);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::CP_A_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'01110000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);

	// Test equal
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(AF.Value() == 0x00'00);

	AF.SetHighByte(0x0F);
	REQUIRE(AF.Value() == 0x0F'00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::CP_A_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00001111'11000000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}
