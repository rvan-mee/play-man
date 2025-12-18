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

namespace GameBoy
{
	size_t Cpu::Increment_8bit_High(Register CpuCore:: *reg)
	{
		Register& r = core.*reg;
		const uint8_t val = r.HighByte();
		const uint8_t res = val + 1;

		r.SetHighByte(res);

		core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
		core.SetFlag(FlagRegisterFlag::SUB, false);
		core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) + 1 > 0xF));

		constexpr size_t numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::Decrement_8bit_High(Register CpuCore::* reg)
	{
		Register& r = core.*reg;
		const uint8_t val = r.HighByte();
		const uint8_t res = val - 1;

		r.SetHighByte(res);

		core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
		core.SetFlag(FlagRegisterFlag::SUB, true);
		core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) == 0));	

		constexpr size_t numberOfCycles = 1;
		return numberOfCycles;
	}

	size_t Cpu::Compare_8bit_High_ImmediateData(Register CpuCore::*reg)
	{
		const uint8_t regContents = (core.*reg).HighByte();
		const uint8_t data = FetchPcAddress();
		const uint8_t compareResult = regContents - data;

		core.SetFlag(FlagRegisterFlag::ZERO, compareResult == 0);
		core.SetFlag(FlagRegisterFlag::SUB, true);
		core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((regContents & 0xF) - (data & 0xF)) < 0);
		core.SetFlag(FlagRegisterFlag::CARRY, regContents < data);

		constexpr size_t numberOfCycles = 2;
		return numberOfCycles;
	}

} // namespace GameBoy
