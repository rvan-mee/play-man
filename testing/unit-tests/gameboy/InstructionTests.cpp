
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/gameboy/cpu/Cpu.hpp"
#include "play-man/gameboy/memory/MemoryBus.hpp"

#define GB_ROM_PATH "../test-data/custom_gb_test_roms/"

// Address for the interrupt register 
constexpr uint16_t interruptAddress = 0xFFFF;
constexpr uint16_t wRamAddressStart = 0xC000;

namespace TestFixtures
{
	/**
	 * @brief
	 */
	struct GameBoyCpuFixture
	{
		GameBoyCpuFixture()
			: cpu(GB_ROM_PATH "test_rom.gb")
			, memoryBus(cpu.memoryBus)
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
		GameBoy::MemoryBus& memoryBus;
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

		void PrintRegs()
		{
			std::cout << "Register AF: " << std::to_string( AF.Value() ) << std::endl;
			std::cout << "Register BC: " << std::to_string( BC.Value() ) << std::endl;
			std::cout << "Register DE: " << std::to_string( DE.Value() ) << std::endl;
			std::cout << "Register HL: " << std::to_string( HL.Value() ) << std::endl;
			std::cout << "Register SP: " << std::to_string( SP.Value() ) << std::endl;
			std::cout << "Register PC: " << std::to_string( PC.Value() ) << std::endl;
			std::cout << "Register IE: " << std::to_string( IE ) << std::endl;
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

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x01'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x10'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_B, 0x05")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0xFF'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0x01);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1100'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0xF0);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0xEF'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_n8, 0x06")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x0F'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLCA, 0x07")
{
	ClearRegisters();
	AF.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RLCA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000011'0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RLCA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b10101010'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_a16_NI_SP, 0x08")
{
	// File containing the address of the work ram
	LoadTestRom(GB_ROM_PATH "workram_address.gb");
	SP.SetValue(0xF00F);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_a16_NI_SP);

	REQUIRE(numberOfCycles == 5);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0xF0'0F);
	REQUIRE(PC.Value() == 0x00'02);
	REQUIRE(IE == 0x00);

	const uint16_t val = memoryBus.ReadByte(wRamAddressStart) | memoryBus.ReadByte(wRamAddressStart + 1) << 8;
	REQUIRE(SP.Value() == val);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "ADD_HL_BC, 0x09")
{
	ClearRegisters();
	BC.SetValue(0x0FFF);
	HL.SetValue(0x0001);
	
	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_BC);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x0F'FF);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x10'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetValue(0xFFFF);
	HL.SetValue(0x0001);
	
	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_BC);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0011'0000);
	REQUIRE(BC.Value() == 0xFF'FF);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetValue(0x0001);
	HL.SetValue(0x0001);
	
	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_BC);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'01);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'02);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_A_BC_NI, 0x0A")
{
	ClearRegisters();

	BC.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x69);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_A_BC_NI);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x69'00);
	REQUIRE(BC.Value() == wRamAddressStart);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_BC, 0x0B")
{
	ClearRegisters();

	BC.SetValue(0x01);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_BC);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_BC);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0xFF'FF);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	BC.SetValue(0x01'00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_BC);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'FF);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_C, 0x0C")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'01);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x00'10);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_C, 0x0D")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'FF);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0x01);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1100'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0xF0);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'EF);
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
	REQUIRE(AF.Value() == 0b00000000'0111'0000);
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
	REQUIRE(AF.Value() == 0b00001111'1100'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}
