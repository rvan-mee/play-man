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

    size_t Cpu::BitComplementToZeroFlag(uint8_t bitMask, RegisterPointer reg, RegisterGet8Bit GetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const bool bitComplement = ((value & bitMask) == 0);

        core.SetFlag(FlagRegisterFlag::ZERO, bitComplement);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::BitComplementToZeroFlag_Addr(uint8_t bitMask, RegisterPointer addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        const uint8_t value = memoryBus.ReadByte(address);
        const bool bitComplement = ((value & bitMask) == 0);

        core.SetFlag(FlagRegisterFlag::ZERO, bitComplement);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }
}
