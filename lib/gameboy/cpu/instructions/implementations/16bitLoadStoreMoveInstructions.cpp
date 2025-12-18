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
	size_t Cpu::Load_16bit_ImmediateData(Register CpuCore::* reg)
	{
		Register& r = core.*reg;
		r.SetLowByte(FetchPcAddress());
		r.SetHighByte(FetchPcAddress());

		constexpr auto numberOfCycles = 3;
		return numberOfCycles;
	}

	size_t Cpu::Load_16bit_RegToImmediateAddr(Register CpuCore::* reg)
	{
		const Register& r = core.*reg;
		const auto lowByte = r.LowByte();
		const auto highByte = r.HighByte();

		const auto addr = FetchPcAddress16bit();

		memoryBus.WriteByte(addr, lowByte);
		memoryBus.WriteByte(addr+1, highByte);

		constexpr size_t numberOfCycles = 5;
		return numberOfCycles;
	}

} /* namespace GameBoy */
