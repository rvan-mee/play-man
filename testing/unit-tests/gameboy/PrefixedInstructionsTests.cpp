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

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_0_B, 0xCB40")
{
	ClearRegisters();
	BC.SetHighByte(0b0000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0001);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_0_C, 0xCB41")
{
	ClearRegisters();
	BC.SetLowByte(0b0000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0001);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_0_D, 0xCB42")
{
	ClearRegisters();
	DE.SetHighByte(0b0000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0001);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_0_E, 0xCB43")
{
	ClearRegisters();
	DE.SetLowByte(0b0000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0001);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_0_H, 0xCB44")
{
	ClearRegisters();
	HL.SetHighByte(0b0000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0001);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_0_L, 0xCB45")
{
	ClearRegisters();
	HL.SetLowByte(0b0000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0001);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_0_HL_NI, 0xCB46")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0001;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0001);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_0_A, 0xCB47")
{
	ClearRegisters();
	AF.SetHighByte(0b0000'0001);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0001);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_1_B, 0xCB48")
{
	ClearRegisters();
	BC.SetHighByte(0b0000'0010);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0010);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_1_C, 0xCB49")
{
	ClearRegisters();
	BC.SetLowByte(0b0000'0010);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0010);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_1_D, 0xCB4A")
{
	ClearRegisters();
	DE.SetHighByte(0b0000'0010);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0010);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_1_E, 0xCB4B")
{
	ClearRegisters();
	DE.SetLowByte(0b0000'0010);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0010);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_1_H, 0xCB4C")
{
	ClearRegisters();
	HL.SetHighByte(0b0000'0010);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0010);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_1_L, 0xCB4D")
{
	ClearRegisters();
	HL.SetLowByte(0b0000'0010);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0010);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_1_HL_NI, 0xCB4E")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0010;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0010);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_1_A, 0xCB4F")
{
	ClearRegisters();
	AF.SetHighByte(0b0000'0010);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0010);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_1_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_2_B, 0xCB50")
{
	ClearRegisters();
	BC.SetHighByte(0b0000'0100);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0100);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_2_C, 0xCB51")
{
	ClearRegisters();
	BC.SetLowByte(0b0000'0100);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0100);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_2_D, 0xCB52")
{
	ClearRegisters();
	DE.SetHighByte(0b0000'0100);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0100);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_2_E, 0xCB53")
{
	ClearRegisters();
	DE.SetLowByte(0b0000'0100);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0100);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_2_H, 0xCB54")
{
	ClearRegisters();
	HL.SetHighByte(0b0000'0100);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0100);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_2_L, 0xCB55")
{
	ClearRegisters();
	HL.SetLowByte(0b0000'0100);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0100);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_2_HL_NI, 0xCB56")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0100;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0100);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_2_A, 0xCB57")
{
	ClearRegisters();
	AF.SetHighByte(0b0000'0100);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0100);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_2_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_3_B, 0xCB58")
{
	ClearRegisters();
	BC.SetHighByte(0b0000'1000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0b0000'1000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_3_C, 0xCB59")
{
	ClearRegisters();
	BC.SetLowByte(0b0000'1000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'1000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_3_D, 0xCB5A")
{
	ClearRegisters();
	DE.SetHighByte(0b0000'1000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'1000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_3_E, 0xCB5B")
{
	ClearRegisters();
	DE.SetLowByte(0b0000'1000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'1000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_3_H, 0xCB5C")
{
	ClearRegisters();
	HL.SetHighByte(0b0000'1000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'1000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_3_L, 0xCB5D")
{
	ClearRegisters();
	HL.SetLowByte(0b0000'1000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'1000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_3_HL_NI, 0xCB5E")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'1000;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'1000);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_3_A, 0xCB5F")
{
	ClearRegisters();
	AF.SetHighByte(0b0000'1000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'1000);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_3_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_4_B, 0xCB60")
{
	ClearRegisters();
	BC.SetHighByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0b0001'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_4_C, 0xCB61")
{
	ClearRegisters();
	BC.SetLowByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0001'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_4_D, 0xCB62")
{
	ClearRegisters();
	DE.SetHighByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0001'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_4_E, 0xCB63")
{
	ClearRegisters();
	DE.SetLowByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0001'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_4_H, 0xCB64")
{
	ClearRegisters();
	HL.SetHighByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0001'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_4_L, 0xCB65")
{
	ClearRegisters();
	HL.SetLowByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0001'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_4_HL_NI, 0xCB66")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0001'0000;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0001'0000);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_4_A, 0xCB67")
{
	ClearRegisters();
	AF.SetHighByte(0b0001'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0001'0000);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_4_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_5_B, 0xCB68")
{
	ClearRegisters();
	BC.SetHighByte(0b0010'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0b0010'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_5_C, 0xCB69")
{
	ClearRegisters();
	BC.SetLowByte(0b0010'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0010'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_5_D, 0xCB6A")
{
	ClearRegisters();
	DE.SetHighByte(0b0010'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0010'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_5_E, 0xCB6B")
{
	ClearRegisters();
	DE.SetLowByte(0b0010'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0010'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_5_H, 0xCB6C")
{
	ClearRegisters();
	HL.SetHighByte(0b0010'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0010'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_5_L, 0xCB6D")
{
	ClearRegisters();
	HL.SetLowByte(0b0010'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0010'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_5_HL_NI, 0xCB6E")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0010'0000;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0010'0000);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_5_A, 0xCB6F")
{
	ClearRegisters();
	AF.SetHighByte(0b0010'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0010'0000);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_5_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_6_B, 0xCB70")
{
	ClearRegisters();
	BC.SetHighByte(0b0100'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0b0100'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_6_C, 0xCB71")
{
	ClearRegisters();
	BC.SetLowByte(0b0100'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0100'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_6_D, 0xCB72")
{
	ClearRegisters();
	DE.SetHighByte(0b0100'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0100'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_6_E, 0xCB73")
{
	ClearRegisters();
	DE.SetLowByte(0b0100'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0100'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_6_H, 0xCB74")
{
	ClearRegisters();
	HL.SetHighByte(0b0100'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0100'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_6_L, 0xCB75")
{
	ClearRegisters();
	HL.SetLowByte(0b0100'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0100'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_6_HL_NI, 0xCB76")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0100'0000;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0100'0000);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_6_A, 0xCB77")
{
	ClearRegisters();
	AF.SetHighByte(0b0100'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0100'0000);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_6_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_7_B, 0xCB78")
{
	ClearRegisters();
	BC.SetHighByte(0b1000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0b1000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0b0000'0000);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_7_C, 0xCB79")
{
	ClearRegisters();
	BC.SetLowByte(0b1000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b1000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	BC.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(BC.LowByte() == 0b0000'0000);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_7_D, 0xCB7A")
{
	ClearRegisters();
	DE.SetHighByte(0b1000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b0000'0000);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_7_E, 0xCB7B")
{
	ClearRegisters();
	DE.SetLowByte(0b1000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b1000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	DE.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(DE.LowByte() == 0b0000'0000);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_7_H, 0xCB7C")
{
	ClearRegisters();
	HL.SetHighByte(0b1000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b0000'0000);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_7_L, 0xCB7D")
{
	ClearRegisters();
	HL.SetLowByte(0b1000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b1000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	HL.SetLowByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(HL.LowByte() == 0b0000'0000);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_7_HL_NI, 0xCB7E")
{
	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b1000'0000;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1000'0000);

	ClearRegisters();
	HL.SetValue(interruptAddress);
	IE = 0b0000'0000;

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_HL_NI);

	REQUIRE(numberOfCycles == 3);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b0000'0000);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "BIT_7_A, 0xCB7F")
{
	ClearRegisters();
	AF.SetHighByte(0b1000'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1000'0000);
	REQUIRE(AF.LowByte() == 0b0010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);

	ClearRegisters();
	AF.SetHighByte(0b0000'0000);

	numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_7_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b0000'0000);
	REQUIRE(AF.LowByte() == 0b1010'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_0_B, 0xCB80")
{
	ClearRegisters();
	BC.SetHighByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_0_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.HighByte() == 0b1111'1110);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_0_C, 0xCB81")
{
	ClearRegisters();
	BC.SetLowByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_0_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.LowByte() == 0b1111'1110);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_0_D, 0xCB82")
{
	ClearRegisters();
	DE.SetHighByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_0_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1111'1110);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_0_E, 0xCB83")
{
	ClearRegisters();
	DE.SetLowByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_0_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.LowByte() == 0b1111'1110);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_0_H, 0xCB84")
{
	ClearRegisters();
	HL.SetHighByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_0_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1111'1110);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_0_L, 0xCB85")
{
	ClearRegisters();
	HL.SetLowByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_0_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.LowByte() == 0b1111'1110);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_0_HL_NI, 0xCB86")
{
	ClearRegisters();
	AF.SetLowByte(0b1111'0000);
	HL.SetValue(interruptAddress);
	IE = 0b1111'1111;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_0_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1111'1110);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_0_A, 0xCB87")
{
	ClearRegisters();
	AF.SetHighByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_0_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1111'1110);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_1_B, 0xCB88")
{
	ClearRegisters();
	BC.SetHighByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_1_B);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.HighByte() == 0b1111'1101);
	REQUIRE(BC.LowByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_1_C, 0xCB89")
{
	ClearRegisters();
	BC.SetLowByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_1_C);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.LowByte() == 0b1111'1101);
	REQUIRE(BC.HighByte() == 0x00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_1_D, 0xCB8A")
{
	ClearRegisters();
	DE.SetHighByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_1_D);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.HighByte() == 0b1111'1101);
	REQUIRE(DE.LowByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_1_E, 0xCB8B")
{
	ClearRegisters();
	DE.SetLowByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_1_E);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.LowByte() == 0b1111'1101);
	REQUIRE(DE.HighByte() == 0x00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_1_H, 0xCB8C")
{
	ClearRegisters();
	HL.SetHighByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_1_H);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.HighByte() == 0b1111'1101);
	REQUIRE(HL.LowByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_1_L, 0xCB8D")
{
	ClearRegisters();
	HL.SetLowByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_1_L);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.LowByte() == 0b1111'1101);
	REQUIRE(HL.HighByte() == 0x00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_1_HL_NI, 0xCB8E")
{
	ClearRegisters();
	AF.SetLowByte(0b1111'0000);
	HL.SetValue(interruptAddress);
	IE = 0b1111'1111;

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_1_HL_NI);

	REQUIRE(numberOfCycles == 4);
	REQUIRE(AF.HighByte() == 0x00);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == interruptAddress);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0b1111'1101);
}

TEST_CASE_METHOD(TestFixtures::GameBoyCpuFixture, "RES_1_A, 0xCB8F")
{
	ClearRegisters();
	AF.SetHighByte(0b1111'1111);
	AF.SetLowByte(0b1111'0000);

	auto numberOfCycles = ExecuteInstruction(GameBoy::PrefixedOpCode::RES_1_A);

	REQUIRE(numberOfCycles == 2);
	REQUIRE(AF.HighByte() == 0b1111'1101);
	REQUIRE(AF.LowByte() == 0b1111'0000);
	REQUIRE(BC.Value() == 0x00'00);
	REQUIRE(DE.Value() == 0x00'00);
	REQUIRE(HL.Value() == 0x00'00);
	REQUIRE(SP.Value() == 0x00'00);
	REQUIRE(PC.Value() == 0x00'00);
	REQUIRE(IE == 0x00);
}
