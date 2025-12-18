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

#define HIGH_BIT 0b10000000
#define LOW_BIT 0b00000001

namespace GameBoy
{
    size_t Cpu::Rotate_8bit_High_Left(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t shiftedBit = ((r.HighByte() & HIGH_BIT) != 0) ? 0x1 : 0x0;
        const uint8_t value = r.HighByte();

        r.SetHighByte(value << 1 | shiftedBit);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::ADD_SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit != 0);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }
}
