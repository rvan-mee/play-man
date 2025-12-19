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

    size_t Cpu::Store_8bit_Addr_High(Register CpuCore::* addrReg, Register CpuCore::* dataReg)
    {
        uint8_t data = (core.*dataReg).HighByte();
        uint16_t addr = (core.*addrReg).Value();

        memoryBus.WriteByte(addr, data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_High_ImmediateData(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t data = FetchPcAddress();

        r.SetHighByte(data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_High_Addr(Register CpuCore::* destReg, Register CpuCore::* addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        Register& dest = core.*destReg;

        dest.SetHighByte(memoryBus.ReadByte(address));

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

}
