
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/gameboy/cpu/Cpu.hpp"
#include "play-man/gameboy/memory/MemoryBus.hpp"

#define GB_ROM_PATH "../test-data/custom_gb_test_roms/"

// Address for the interrupt register 
constexpr uint16_t interruptAddress = 0xFFFF;
constexpr uint16_t wRamAddressStart = 0xC000;

constexpr uint8_t immediateData8bit = 0x0F;

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

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_n8, 0x0E")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'0F);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRCA, 0x0F")
{
	ClearRegisters();
	BC.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RRCA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0001'0000);
	REQUIRE(BC.Value() == 0b00000000'11000000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b10101010);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RRCA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0b00000000'01010101);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

// TODO: STOP instruction 

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_DE_n16, 0x11")
{
	// File containing the bytes 0xF0 0x0F
	// The first byte should become the lower half of the 16 bit register.
	LoadTestRom(GB_ROM_PATH "load_immediate_16.gb");
	const auto pcBefore = PC.Value();
	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_DE_n16);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x0F'F0);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);

	const auto expectedPC = pcBefore + 2;
	REQUIRE(PC.Value() == expectedPC);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_DE_NI_A, 0x12")
{
	ClearRegisters();

	// Set the address inside the BC register and the value in A
	DE.SetValue(interruptAddress);
	AF.SetHighByte(0xF0);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_DE_NI_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0xF0'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == interruptAddress);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0xF0);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_DE, 0x13")
{
	ClearRegisters();

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_DE);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'01);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_D, 0x14")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x01'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x10'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_D, 0x15")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xFF'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0x01);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1100'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0xF0);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xEF'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_n8, 0x16")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x0F'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLA, 0x17")
{
	ClearRegisters();
	AF.SetValue(0b10000001'0000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RLA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000010'0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	AF.SetValue(0b01010101'0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RLA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b10101010'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	AF.SetValue(0b00000000'1111'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RLA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000001'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "JR_e8, 0x18")
{
	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	REQUIRE(PC.Value() == 0x00'00);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_e8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'02);
	REQUIRE(IE == 0x00);

	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	PC.SetValue(1024);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_e8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x03'02);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "ADD_HL_DE, 0x19")
{
	ClearRegisters();
	DE.SetValue(0x0FFF);
	HL.SetValue(0x0001);
	
	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_DE);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x0F'FF);
	REQUIRE(HL.Value() == 0x10'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetValue(0xFFFF);
	HL.SetValue(0x0001);
	
	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_DE);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0011'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xFF'FF);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetValue(0x0001);
	HL.SetValue(0x0001);
	
	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_DE);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'01);
	REQUIRE(HL.Value() == 0x00'02);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_A_DE_NI, 0x1A")
{
	ClearRegisters();

	DE.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x69);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_A_DE_NI);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x69'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == wRamAddressStart);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_DE, 0x1B")
{
	ClearRegisters();

	DE.SetValue(0x01);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_DE);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_DE);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xFF'FF);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	DE.SetValue(0x01'00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_DE);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'FF);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_E, 0x1C")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'01);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'10);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_E, 0x1D")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'FF);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0x01);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1100'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0xF0);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'EF);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_n8, 0x1E")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'0F);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRA, 0x1F")
{
	ClearRegisters();
	AF.SetValue(0b10000001'0000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RRA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b01000000'0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetValue(0b01010101'0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RRA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b10101010'0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetValue(0b00000000'0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::RRA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b10000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "JR_NZ_e8, 0x20")
{
	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	AF.SetLowByte(0b1000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_NZ_e8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);

	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	AF.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_NZ_e8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'02);
	REQUIRE(IE == 0x00);

	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	PC.SetValue(1024);
	AF.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_NZ_e8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x03'02);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_HL_n16, 0x21")
{
	LoadTestRom(GB_ROM_PATH "load_immediate_16.gb");
	const auto pcBefore = PC.Value();
	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_HL_n16);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x0F'F0);
	REQUIRE(SP.Value() == 0x00'00);

	const auto expectedPC = pcBefore + 2;
	REQUIRE(PC.Value() == expectedPC);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_HL_INC_NI_A, 0x22")
{
	ClearRegisters();

	HL.SetValue(wRamAddressStart);
	AF.SetHighByte(0xF0);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_HL_INC_NI_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0xF0'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart + 1);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == AF.HighByte());
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_HL, 0x23")
{
	ClearRegisters();

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_HL);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'01);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_H, 0x24")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x01'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x10'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_H, 0x25")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0xFF'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0x01);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1100'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0xF0);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0xEF'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_H_n8, 0x26")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_H_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x0F'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DAA, 0x27")
{
	ClearRegisters();

	AF.SetHighByte(0xA0);
	AF.SetLowByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DAA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b1001'0000);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	AF.SetHighByte(0x0A);
	AF.SetLowByte(0b0010'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DAA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(AF.HighByte() == 0x10);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	AF.SetHighByte(0x1B);
	AF.SetLowByte(0b0010'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DAA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(AF.HighByte() == 0x21);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	AF.SetHighByte(0x99);
	AF.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DAA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(AF.HighByte() == 0x99);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	AF.SetHighByte(0x99);
	AF.SetLowByte(0b0100'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DAA);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b0100'0000);
	REQUIRE(AF.HighByte() == 0x99);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "JR_Z_e8, 0x28")
{
	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_Z_e8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);


	ClearRegisters();
	AF.SetValue(0b00000000'1000'0000);
	PC.SetValue(1024);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_Z_e8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0b00000000'1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x03'02);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "ADD_HL_HL, 0x29")
{
	ClearRegisters();
	HL.SetValue(0x0F'FF);
	
	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_HL);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x1F'FE);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetValue(0xFF'FF);
	
	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_HL);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0011'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0xFF'FE);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetValue(0x0001);
	
	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_HL);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'02);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_A_HL_INC_NI, 0x2A")
{
	ClearRegisters();

	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x69);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_A_HL_INC_NI);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x69'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart + 1);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_HL, 0x2B")
{
	ClearRegisters();

	HL.SetValue(0x01);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_HL);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_HL);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0xFF'FF);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	HL.SetValue(0x01'00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_HL);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'FF);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_L, 0x2C")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'01);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'10);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_L, 0x2D")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'FF);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0x01);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'1100'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0xF0);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.Value() == 0b00000000'0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'EF);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_L_n8, 0x2E")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_L_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'0F);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "CPL, 0x2F")
{
	ClearRegisters();

	AF.SetHighByte(0b10101010);
	AF.SetLowByte(0b1001'0000);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::CPL);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0b01010101);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "JR_NC_e8, 0x30")
{
	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	AF.SetLowByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_NC_e8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);

	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	AF.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_NC_e8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'02);
	REQUIRE(IE == 0x00);

	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	PC.SetValue(1024);
	AF.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_NC_e8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x03'02);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_SP_n16, 0x31")
{
	LoadTestRom(GB_ROM_PATH "load_immediate_16.gb");
	const auto pcBefore = PC.Value();
	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_SP_n16);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x0F'F0);

	const auto expectedPC = pcBefore + 2;
	REQUIRE(PC.Value() == expectedPC);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_HL_DEC_NI_A, 0x32")
{
	ClearRegisters();

	HL.SetValue(wRamAddressStart);
	AF.SetHighByte(0xF0);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_HL_DEC_NI_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0xF0'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart - 1);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == AF.HighByte());
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_SP, 0x33")
{
	ClearRegisters();

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_SP);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'01);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_HL_NI, 0x34")
{
	ClearRegisters();
	AF.SetLowByte(0b1111'0000);
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x00);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == 0x01);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == 0x10);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == 0x00);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_HL_NI, 0x35")
{
	ClearRegisters();
	AF.SetLowByte(0b1011'0000);
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x02);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == 0x01);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0101'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x10);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == 0x0F);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0110'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x01);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == 0x00);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1100'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_HL_NI_n8, 0x36")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	HL.SetValue(wRamAddressStart);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_HL_NI_n8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(memoryBus.ReadByte(wRamAddressStart) == immediateData8bit);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SCF, 0x37")
{
	ClearRegisters();
	AF.SetLowByte(0b1110'0000);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::SCF);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "JR_C_e8, 0x38")
{
	LoadTestRom(GB_ROM_PATH "jump_relative_test.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_C_e8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetValue(0b00000000'0001'0000);
	PC.SetValue(1024);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::JR_C_e8);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.Value() == 0b00000000'0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x03'02);
	REQUIRE(IE == 0x00);
}


TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "ADD_HL_SP, 0x39")
{
	ClearRegisters();
	SP.SetValue(0x0FFF);
	HL.SetValue(0x0001);
	
	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_SP);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x10'00);
	REQUIRE(SP.Value() == 0x0F'FF);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	SP.SetValue(0xFFFF);
	HL.SetValue(0x0001);
	
	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_SP);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0011'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0xFF'FF);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	SP.SetValue(0x0001);
	HL.SetValue(0x0001);
	
	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::ADD_HL_SP);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0b00000000'0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'02);
	REQUIRE(SP.Value() == 0x00'01);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_A_HL_DEC_NI, 0x3A")
{
	ClearRegisters();

	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0x69);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_A_HL_DEC_NI);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x69'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart - 1);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_SP, 0x3B")
{
	ClearRegisters();

	SP.SetValue(0x01);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_SP);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_SP);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0xFF'FF);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	SP.SetValue(0x01'00);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_SP);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x00'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'FF);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "INC_A, 0x3C")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(AF.HighByte() == 0x01);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0x0F);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(AF.HighByte() == 0x10);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0xFF);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::INC_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "DEC_A, 0x3D")
{
	ClearRegisters();

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b0110'0000);
	REQUIRE(AF.HighByte() == 0xFF);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0x01);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b1100'0000);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0xF0);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::DEC_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.LowByte() == 0b0110'0000);
	REQUIRE(AF.HighByte() == 0xEF);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_A_n8, 0x3E")
{
	// File containing the byte 0x0F
	LoadTestRom(GB_ROM_PATH "load_immediate_8.gb");
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(AF.Value() == 0x00'00);

	const auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_A_n8);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.Value() == 0x0F'00);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'01);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "CCF, 0x3F")
{
	ClearRegisters();
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::CCF);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0110'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::OpCode::CCF);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_B, 0x40")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x0F'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_C, 0x41")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);
	BC.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'F0);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_D, 0x42")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);
	DE.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'00);
	REQUIRE(DE.Value() == 0xF0'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_E, 0x43")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);
	DE.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'00);
	REQUIRE(DE.Value() == 0x00'F0);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_H, 0x44")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);
	HL.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0xF0'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_L, 0x45")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);
	HL.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'F0);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_HL_NI, 0x46")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_HL_NI);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_B_A, 0x47")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);
	AF.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_B_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0xF0);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_B, 0x48")
{
	ClearRegisters();
	BC.SetLowByte(0x0F);
	BC.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'F0);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_C, 0x49")
{
	ClearRegisters();
	BC.SetLowByte(0x0F);
	BC.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_D, 0x4A")
{
	ClearRegisters();
	BC.SetLowByte(0x0F);
	DE.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0xF0'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_E, 0x4B")
{
	ClearRegisters();
	BC.SetLowByte(0x0F);
	DE.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0x00'F0);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_H, 0x4C")
{
	ClearRegisters();
	BC.SetLowByte(0x0F);
	HL.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0xF0'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_L, 0x4D")
{
	ClearRegisters();
	BC.SetLowByte(0x0F);
	HL.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'F0);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_HL_NI, 0x4E")
{
	ClearRegisters();
	BC.SetLowByte(0x0F);
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_HL_NI);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_C_A, 0x4F")
{
	ClearRegisters();
	BC.SetLowByte(0x0F);
	AF.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_C_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0xF0);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_B, 0x50")
{
	ClearRegisters();
	BC.SetHighByte(0x0F);
	DE.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x0F'00);
	REQUIRE(DE.Value() == 0x0F'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_C, 0x51")
{
	ClearRegisters();
	DE.SetHighByte(0x0F);
	BC.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0xF0'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_D, 0x52")
{
	ClearRegisters();
	DE.SetHighByte(0x0F);
	DE.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xF0'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_E, 0x53")
{
	ClearRegisters();
	DE.SetHighByte(0x0F);
	DE.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xF0'F0);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_H, 0x54")
{
	ClearRegisters();
	DE.SetHighByte(0x0F);
	HL.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xF0'00);
	REQUIRE(HL.Value() == 0xF0'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_L, 0x55")
{
	ClearRegisters();
	DE.SetHighByte(0x0F);
	HL.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xF0'00);
	REQUIRE(HL.Value() == 0x00'F0);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_HL_NI, 0x56")
{
	ClearRegisters();
	DE.SetHighByte(0x0F);
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_HL_NI);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xF0'00);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_D_A, 0x57")
{
	ClearRegisters();
	DE.SetHighByte(0x0F);
	AF.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_D_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0xF0);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xF0'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_B, 0x58")
{
	ClearRegisters();
	DE.SetLowByte(0x0F);
	BC.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_B);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0xF0'00);
	REQUIRE(DE.Value() == 0x00'F0);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_C, 0x59")
{
	ClearRegisters();
	DE.SetLowByte(0x0F);
	BC.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_C);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'F0);
	REQUIRE(DE.Value() == 0x00'F0);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_D, 0x5A")
{
	ClearRegisters();
	DE.SetLowByte(0x0F);
	DE.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_D);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0xF0'F0);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_E, 0x5B")
{
	ClearRegisters();
	DE.SetLowByte(0x0F);
	DE.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_E);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'F0);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_H, 0x5C")
{
	ClearRegisters();
	DE.SetLowByte(0x0F);
	HL.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_H);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'F0);
	REQUIRE(HL.Value() == 0xF0'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_L, 0x5D")
{
	ClearRegisters();
	DE.SetLowByte(0x0F);
	HL.SetLowByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_L);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'F0);
	REQUIRE(HL.Value() == 0x00'F0);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_HL_NI, 0x5E")
{
	ClearRegisters();
	DE.SetLowByte(0x0F);
	HL.SetValue(wRamAddressStart);
	memoryBus.WriteByte(wRamAddressStart, 0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_HL_NI);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'F0);
	REQUIRE(HL.Value() == wRamAddressStart);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "LD_E_A, 0x5F")
{
	ClearRegisters();
	DE.SetLowByte(0x0F);
	AF.SetHighByte(0xF0);

	auto numberOfCycles = ExecuteInstruction(GameBoy::OpCode::LD_E_A);

	REQUIRE(numberOfCycles == 1);
	REQUIRE(AF.HighByte() == 0xF0);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'F0);
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
