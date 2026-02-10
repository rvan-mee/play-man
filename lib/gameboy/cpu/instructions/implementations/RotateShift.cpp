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
    size_t Cpu::NoPrefixRotateLeft(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const uint8_t appendBit = core.GetFlag(FlagRegisterFlag::CARRY);
        const uint8_t result = (value << 1) | appendBit;
        const bool shiftedBit = (value & HIGH_BIT) > 0;

        (r.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::NoPrefixRotateLeftCarry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const bool shiftedBit = (value & HIGH_BIT) > 0;
        const uint8_t result = value << 1 | shiftedBit; 

        (r.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::NoPrefixRotateRight(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const bool shiftedBit = (value & LOW_BIT) > 0;
        const bool carryBit = core.GetFlag(FlagRegisterFlag::CARRY);
        const uint8_t result = (carryBit << 7) | (value >> 1);

        (r.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::NoPrefixRotateRightCarry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const bool shiftedBit = (value & LOW_BIT);
        const uint8_t result = (shiftedBit << 7) | (value >> 1);

        (r.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }




    /**                                     Prefixed Instructions                                     **/

    size_t Cpu::RotateLeft(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const uint8_t shiftedBit = (value & HIGH_BIT) ? LOW_BIT : NO_BIT;
        const uint8_t appendBit = core.GetFlag(FlagRegisterFlag::CARRY);
        const uint8_t result = value << 1 | appendBit;

        (r.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::RotateLeftCarry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const bool shiftedBit = (value & HIGH_BIT);
        const uint8_t result = (value << 1) | shiftedBit;

        (r.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::RotateLeftCarry_Addr(RegisterPointer addrReg)
    {
        Register& r = core.*addrReg;
        const uint16_t address = r.Value();
        const uint8_t value = memoryBus.ReadByte(address);
        const bool shiftedBit = (value & HIGH_BIT);
        const uint8_t result = (value << 1) | shiftedBit;

        memoryBus.WriteByte(address, result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 4;
        return numberOfCycles;
    }

    size_t Cpu::RotateRight(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const bool shiftedBit = (value & LOW_BIT) > 0;
        const bool carryBit = core.GetFlag(FlagRegisterFlag::CARRY);
        const uint8_t result = (carryBit << 7) | (value >> 1);

        (r.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::RotateRightCarry(RegisterPointer reg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t value = (r.*GetValue)();
        const bool shiftedBit = (value & LOW_BIT);
        const uint8_t result = (shiftedBit << 7) | (value >> 1);

        (r.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, shiftedBit);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }
}
