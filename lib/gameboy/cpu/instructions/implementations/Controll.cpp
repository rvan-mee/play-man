// ****************************************************************************** //
//   _______   __                              __       __                        //
//  /       \ /  |                            /  \     /  |                       //
//  $$$$$$$  |$$ |  ______   __    __         $$  \   /$$ |  ______   _______     //
//  $$ |__$$ |$$ | /      \ /  |  /  | ______ $$$  \ /$$$ | /      \ /       \    //
//  $$    $$/ $$ | $$$$$$  |$$ |  $$ |/      |$$$$  /$$$$ | $$$$$$  |$$$$$$$  |   //
//  $$$$$$$/  $$ | /    $$ |$$ |  $$ |$$$$$$/ $$ $$ $$/$$ | /    $$ |$$ |  $$ |   //
//  $$ |      $$ |/$$$$$$$ |$$ \__$$ |        $$ |$$$/ $$ |/$$$$$$$ |$$ |  $$ |   //
//  $$ |      $$ |$$    $$ |$$    $$ |        $$ | $/  $$ |$$    $$ |$$ |  $$ |   //
//  $$/       $$/  $$$$$$$/  $$$$$$$ |        $$/      $$/  $$$$$$$/ $$/   $$/    //
//                          /  \__$$ |                                            //
//                          $$    $$/                                             //
//                           $$$$$$/                                              //
//                                                                                //
//                            By: K1ngmar and rvan-mee                            //
// ****************************************************************************** //

#include <play-man/gameboy/cpu/Cpu.hpp>
#include <play-man/logger/Logger.hpp>

// The offset comes from the difference between binary and hex ie 10 vs 16
#define DDA_TENTHS_OFFSET 0x60
#define DDA_UNITDIGIT_OFFSET 0x06

// In binary decimal format the values can only range from 0 to 9 per digit
#define DDA_TENTHS_CUTOFF 0x99
#define DDA_UNITDIGIT_CUTOFF 0x09

namespace GameBoy
{
	size_t Cpu::HardLock()
	{
		LOG_WARNING("Invalid OpCode called, CPU hard-locked.\nRestart required.");

		// TODO: consider making this optional with a setting
		while (true)
			;

		constexpr auto numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::NOP()
	{
		constexpr auto numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::DDA()
	{
		const bool subtractionSet = core.GetFlag(FlagRegisterFlag::SUB);
		const bool halfCarrySet = core.GetFlag(FlagRegisterFlag::HALF_CARRY);
		const bool carrySet = core.GetFlag(FlagRegisterFlag::CARRY);
		const auto value = core.AF.HighByte();
		uint8_t offset = 0;

		if ((!subtractionSet && (value & 0xF) > DDA_UNITDIGIT_CUTOFF) || halfCarrySet)
		{
			offset |= DDA_UNITDIGIT_OFFSET;
		}
		if ((!subtractionSet && value > DDA_TENTHS_CUTOFF) || carrySet)
		{
			offset |= DDA_TENTHS_OFFSET;
			core.SetFlag(FlagRegisterFlag::CARRY, true);
		}

		if (subtractionSet)
			core.AF.SetHighByte(value - offset);
		else
			core.AF.SetHighByte(value + offset);

		core.SetFlag(FlagRegisterFlag::ZERO, core.AF.HighByte() == 0);
		core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
		
		constexpr auto numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::CPL()
	{
		const uint8_t valueA = core.AF.HighByte();

		core.AF.SetHighByte(~valueA);

		core.SetFlag(FlagRegisterFlag::SUB, true);
		core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);

		constexpr auto numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::SCF()
	{
		core.SetFlag(FlagRegisterFlag::SUB, false);
		core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
		core.SetFlag(FlagRegisterFlag::CARRY, true);

		constexpr auto numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::CCF()
	{
		const bool flagState = core.GetFlag(FlagRegisterFlag::CARRY);
		core.SetFlag(FlagRegisterFlag::SUB, false);
		core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
		core.SetFlag(FlagRegisterFlag::CARRY, !flagState);

		constexpr auto numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::EnableInterrupts()
	{
		LOG_DEBUG("EI called, delaying IME enable");

		core.stateIME = InterruptState::DELAY_ENABLE_IME;

		constexpr auto numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::DisableInterrupts()
	{
		core.IME = false;
		core.stateIME = InterruptState::NONE;

		constexpr auto numberOfCycles = 1;
		return numberOfCycles;
	}
}
