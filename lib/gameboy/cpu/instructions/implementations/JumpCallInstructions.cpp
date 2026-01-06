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

	
	size_t Cpu::Jump_16bit_ImmediateData()
	{
		Register tmpReg;

		tmpReg.SetLowByte(FetchPcAddress());
		tmpReg.SetHighByte(FetchPcAddress());

		core.PC = tmpReg;

		constexpr auto numberOfCycles = 4;
		return numberOfCycles;
	}

	size_t Cpu::Jump_Relative_8bit_SignedImmediateData()
	{
		const uint8_t dist = FetchPcAddress();

		if (dist > 127)
			core.PC -= dist;
		else
			core.PC += dist;

		constexpr auto numberOfCycles = 3;
		return numberOfCycles;
	}

	size_t Cpu::Jump_Relative_FlagNotSet_8bit_SignedImmediateData(FlagRegisterFlag flag)
	{
		const bool flagSet = core.GetFlag(flag);
		const auto dist = FetchPcAddress();
		auto numberOfCycles = 2;

		if (!flagSet)
		{
			numberOfCycles += 1;
			if (dist > 127)
				core.PC -= dist;
			else
				core.PC += dist;
		}
		return numberOfCycles;
	}

	size_t Cpu::Jump_Relative_FlagSet_8bit_SignedImmediateData(FlagRegisterFlag flag)
	{
		const bool flagSet = core.GetFlag(flag);
		const auto dist = FetchPcAddress();
		auto numberOfCycles = 2;

		if (flagSet)
		{
			numberOfCycles += 1;
			if (dist > 127)
				core.PC -= dist;
			else
				core.PC += dist;
		}
		return numberOfCycles;
	}

}
