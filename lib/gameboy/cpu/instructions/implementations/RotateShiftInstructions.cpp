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
    size_t Cpu::Rotate_8bit_Left(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const uint8_t shiftedBit = (value & HIGH_BIT) ? LOW_BIT : NO_BIT;

        (r.*SetValue)(value << 1 | shiftedBit);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Rotate_8bit_Left_Carry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t appendBit = core.GetFlag(FlagRegisterFlag::CARRY);
        const uint8_t value = (r.*GetValue)();
        const bool shiftedBit = (value & HIGH_BIT);

        (r.*SetValue)(value << 1 | appendBit);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Rotate_8bit_Right(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const uint8_t shiftedBit = ((value & LOW_BIT) != 0) ? HIGH_BIT : NO_BIT;

        (r.*SetValue)(shiftedBit | (value >> 1));

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Rotate_8bit_Right_Carry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t appendBit = core.GetFlag(FlagRegisterFlag::CARRY);
        const uint8_t value = (r.*GetValue)();
        const bool shiftedBit = (value & LOW_BIT);

        (r.*SetValue)((appendBit << 7) | (value >> 1));

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }
}
