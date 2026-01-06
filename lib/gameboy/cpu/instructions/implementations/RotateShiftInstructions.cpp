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
#define NO_BIT 0b00000000

namespace GameBoy
{
    size_t Cpu::Rotate_8bit_High_Left(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t value = r.HighByte();
        const uint8_t shiftedBit = (value & HIGH_BIT) ? LOW_BIT : NO_BIT;

        r.SetHighByte(value << 1 | shiftedBit);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Rotate_8bit_High_Left_Carry(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t appendBit = core.GetFlag(FlagRegisterFlag::CARRY);
        const uint8_t value = r.HighByte();
        const bool shiftedBit = (value & HIGH_BIT);

        r.SetHighByte(value << 1 | appendBit);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Rotate_8bit_Low_Right(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t shiftedBit = ((r.LowByte() & LOW_BIT) != 0) ? HIGH_BIT : NO_BIT;
        const uint8_t value = r.LowByte();

        r.SetLowByte(shiftedBit | (value >> 1));

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Rotate_8bit_High_Right_Carry(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t appendBit = core.GetFlag(FlagRegisterFlag::CARRY);
        const uint8_t value = r.HighByte();
        const bool shiftedBit = (value & LOW_BIT);

        r.SetHighByte((appendBit << 7) | (value >> 1));

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }
}
