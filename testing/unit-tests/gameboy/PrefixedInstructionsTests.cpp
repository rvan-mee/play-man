#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "play-man/gameboy/cpu/Cpu.hpp"
#include "play-man/gameboy/memory/MemoryBus.hpp"

#define GB_ROM_PATH "../test-data/custom_gb_test_roms/"

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
			, memoryBus(cpu.memoryBus)
			, AF(cpu.core.AF)
			, BC(cpu.core.BC)
			, DE(cpu.core.DE)
			, HL(cpu.core.HL)
			, SP(cpu.core.SP)
			, PC(cpu.core.PC)
			, IE(cpu.core.IE)
		{
			ClearRegisters();
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

		size_t ExecuteInstruction(GameBoy::PrefixedOpCode op)
		{
			return cpu.prefixedInstructions.at(op)(&cpu);
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
			std::cout << "Register AF: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(AF.HighByte()) << "'" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(AF.LowByte()) << std::endl;
			std::cout << "Register BC: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(BC.HighByte()) << "'" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(BC.LowByte()) << std::endl;
			std::cout << "Register DE: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(DE.HighByte()) << "'" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(DE.LowByte()) << std::endl;
			std::cout << "Register HL: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(HL.HighByte()) << "'" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(HL.LowByte()) << std::endl;
			std::cout << "Register SP: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(SP.HighByte()) << "'" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(SP.LowByte()) << std::endl;
			std::cout << "Register PC: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(PC.HighByte()) << "'" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(PC.LowByte()) << std::endl;
			std::cout << "Register IE: 0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint16_t>(IE) << std::endl;
		}
	};
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLC_B, 0xCB00")
{
	ClearRegisters();
	BC.SetHighByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b0000'0011);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0b1010'1010);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLC_C, 0xCB01")
{
	ClearRegisters();
	BC.SetLowByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0011);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b1010'1010);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLC_D, 0xCB02")
{
	ClearRegisters();
	DE.SetHighByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0011);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1010'1010);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLC_E, 0xCB03")
{
	ClearRegisters();
	DE.SetLowByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0011);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b1010'1010);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLC_H, 0xCB04")
{
	ClearRegisters();
	HL.SetHighByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0011);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1010'1010);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLC_L, 0xCB05")
{
	ClearRegisters();
	HL.SetLowByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0011);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b1010'1010);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLC_HL_NI, 0xCB06")
{
	ClearRegisters();
    HL.SetValue(interruptAddress);
    IE = 0b1000'0001;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0011);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b01010101;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1010'1010);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b00000000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RLC_A, 0xCB07")
{
	ClearRegisters();
	AF.SetHighByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0011);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1010'1010);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RLC_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRC_B, 0xCB08")
{
	ClearRegisters();
	BC.SetHighByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b1100'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b1010'1010);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRC_C, 0xCB09")
{
	ClearRegisters();
	BC.SetLowByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b1100'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b1010'1010);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRC_D, 0xCB0A")
{
	ClearRegisters();
	DE.SetHighByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1100'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1010'1010);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRC_E, 0xCB0B")
{
	ClearRegisters();
	DE.SetLowByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b1100'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b1010'1010);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRC_H, 0xCB0C")
{
	ClearRegisters();
	HL.SetHighByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1100'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1010'1010);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRC_L, 0xCB0D")
{
	ClearRegisters();
	HL.SetLowByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b1100'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b1010'1010);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRC_HL_NI, 0xCB0E")
{
	ClearRegisters();
    HL.SetValue(interruptAddress);
    IE = 0b1000'0001;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1100'0000);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b01010101;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1010'1010);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b00000000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RRC_A, 0xCB0F")
{
	ClearRegisters();
	AF.SetHighByte(0b1000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1100'0000);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b01010101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1010'1010);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RRC_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RL_B, 0xCB10")
{
	ClearRegisters();
	BC.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b0000'0010);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	BC.SetHighByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0b1010'1011);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RL_C, 0xCB11")
{
	ClearRegisters();
	BC.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0010);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	BC.SetLowByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b1010'1011);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RL_D, 0xCB12")
{
	ClearRegisters();
	DE.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0010);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	DE.SetHighByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1010'1011);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RL_E, 0xCB13")
{
	ClearRegisters();
	DE.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0010);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	DE.SetLowByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b1010'1011);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RL_H, 0xCB14")
{
	ClearRegisters();
	HL.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0010);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	HL.SetHighByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1010'1011);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RL_L, 0xCB15")
{
	ClearRegisters();
	HL.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0010);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	HL.SetLowByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b1010'1011);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RL_HL_NI, 0xCB16")
{
	ClearRegisters();
    HL.SetValue(interruptAddress);
    IE = 0b1000'0001;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0010);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	AF.SetLowByte(0b0001'0000);
	IE = 0b110'10101;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1010'1011);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b00000000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RL_A, 0xCB17")
{
	ClearRegisters();
	AF.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0010);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	AF.SetHighByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1010'1011);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RL_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RR_B, 0xCB18")
{
	ClearRegisters();
	BC.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b0100'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	BC.SetHighByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b1010'1010);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RR_C, 0xCB19")
{
	ClearRegisters();
	BC.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0100'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	BC.SetLowByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b1010'1010);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RR_D, 0xCB1A")
{
	ClearRegisters();
	DE.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0100'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	DE.SetHighByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1010'1010);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RR_E, 0xCB1B")
{
	ClearRegisters();
	DE.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0100'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	DE.SetLowByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b1010'1010);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RR_H, 0xCB1C")
{
	ClearRegisters();
	HL.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0100'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	HL.SetHighByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1010'1010);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RR_L, 0xCB1D")
{
	ClearRegisters();
	HL.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0100'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	HL.SetLowByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b1010'1010);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RR_HL_NI, 0xCB1E")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b1000'0001;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0100'0000);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	HL.SetValue(interruptAddress);
	IE = 0b0101'0101;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1010'1010);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b00000000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RR_A, 0xCB1F")
{
	ClearRegisters();
	AF.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0100'0000);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetLowByte(0b0001'0000);
	AF.SetHighByte(0b0101'0101);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1010'1010);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RR_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SLA_B, 0xCB20")
{
	ClearRegisters();
	BC.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b0000'0010);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0101'0101);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0b1010'1010);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SLA_C, 0xCB21")
{
	ClearRegisters();
	BC.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0010);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0101'0101);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b1010'1010);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SLA_D, 0xCB22")
{
	ClearRegisters();
	DE.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0010);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0101'0101);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1010'1010);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SLA_E, 0xCB23")
{
	ClearRegisters();
	DE.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0010);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0101'0101);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b1010'1010);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SLA_H, 0xCB24")
{
	ClearRegisters();
	HL.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0010);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0101'0101);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1010'1010);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SLA_L, 0xCB25")
{
	ClearRegisters();
	HL.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0010);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0101'0101);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b1010'1010);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SLA_HL_NI, 0xCB26")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b10000001;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0010);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0101'0101;
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1010'1010);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b00000000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SLA_A, 0xCB27")
{
	ClearRegisters();
	AF.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0010);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0101'0101);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1010'1010);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SLA_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRA_B, 0xCB28")
{
	ClearRegisters();
	BC.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b1100'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0b0010'1101);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRA_C, 0xCB29")
{
	ClearRegisters();
	BC.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b1100'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0010'1101);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRA_D, 0xCB2A")
{
	ClearRegisters();
	DE.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1100'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0010'1101);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRA_E, 0xCB2B")
{
	ClearRegisters();
	DE.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b1100'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0010'1101);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRA_H, 0xCB2C")
{
	ClearRegisters();
	HL.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1100'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0010'1101);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRA_L, 0xCB2D")
{
	ClearRegisters();
	HL.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b1100'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0010'1101);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRA_HL_NI, 0xCB2E")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b10000001;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1100'0000);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	AF.SetLowByte(0b0001'0000);
	IE = 0b0101'1010;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0010'1101);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b00000000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRA_A, 0xCB2F")
{
	ClearRegisters();
	AF.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1100'0000);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0010'1101);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRA_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SWAP_B, 0xCB30")
{
	ClearRegisters();
	BC.SetHighByte(0b1100'0011);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0b0011'1100);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SWAP_C, 0xCB31")
{
	ClearRegisters();
	BC.SetLowByte(0b1100'0011);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0011'1100);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SWAP_D, 0xCB32")
{
	ClearRegisters();
	DE.SetHighByte(0b1100'0011);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0011'1100);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SWAP_E, 0xCB33")
{
	ClearRegisters();
	DE.SetLowByte(0b1100'0011);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0011'1100);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SWAP_H, 0xCB34")
{
	ClearRegisters();
	HL.SetHighByte(0b1100'0011);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0011'1100);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SWAP_L, 0xCB35")
{
	ClearRegisters();
	HL.SetLowByte(0b1100'0011);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0011'1100);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SWAP_HL_NI, 0xCB36")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b1100'0011;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0011'1100);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SWAP_A, 0xCB37")
{
	ClearRegisters();
	AF.SetHighByte(0b1100'0011);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0011'1100);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SWAP_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRL_B, 0xCB38")
{
	ClearRegisters();
	BC.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0b0100'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0b0010'1101);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRL_C, 0xCB39")
{
	ClearRegisters();
	BC.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0100'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0010'1101);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRL_D, 0xCB3A")
{
	ClearRegisters();
	DE.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0100'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0010'1101);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRL_E, 0xCB3B")
{
	ClearRegisters();
	DE.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0100'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0010'1101);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRL_H, 0xCB3C")
{
	ClearRegisters();
	HL.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0100'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0010'1101);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRL_L, 0xCB3D")
{
	ClearRegisters();
	HL.SetLowByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0100'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0010'1101);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRL_HL_NI, 0xCB3E")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b10000001;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0100'0000);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	AF.SetLowByte(0b0001'0000);
	IE = 0b0101'1010;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0010'1101);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b00000000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "SRL_A, 0xCB3C")
{
	ClearRegisters();
	AF.SetHighByte(0b10000001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0100'0000);
	REQUIRE(AF.LowByte() == 0b0001'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0101'1010);
	AF.SetLowByte(0b0001'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0010'1101);
	REQUIRE(AF.LowByte() == 0b0000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b00000000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::SRL_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1000'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}
