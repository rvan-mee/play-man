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

namespace GameBoy {

	size_t Cpu::Compare_8bit_High_ImmediateData(Register CpuCore::*reg)
	{
		const uint8_t regContents = (core.*reg).HighByte();
		const uint8_t data = FetchPcAddress();
		const uint8_t compareResult = regContents - data;

		core.SetFlag(FlagRegisterFlag::ZERO, compareResult == 0);
		core.SetFlag(FlagRegisterFlag::ADD_SUB, true);
		core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((regContents & 0xf) - (data & 0xf)) < 0);
		core.SetFlag(FlagRegisterFlag::CARRY, regContents < data);

		constexpr size_t numberOfCycles = 2;
		return numberOfCycles;
	}

} // namespace GameBoy
