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

#include <memory_resource>
#include <play-man/gameboy/cpu/Cpu.hpp>

namespace GameBoy
{

    size_t Cpu::BitwiseAnd(RegisterPointer reg, RegisterGet8Bit GetOperand)
    {
        const uint8_t baseValue = core.AF.HighByte();
        const uint8_t operandValue = ((core.*reg).*GetOperand)();
        const uint8_t result = baseValue & operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::BitwiseAnd_Addr(RegisterPointer addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        const uint8_t  baseValue = core.AF.HighByte();
        const uint8_t  operandValue = memoryBus.ReadByte(address);
        const uint8_t  result = baseValue & operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::BitwiseAnd_ImmediateData()
    {
        const uint8_t  baseValue = core.AF.HighByte();
        const uint8_t  operandValue = FetchPcAddress();
        const uint8_t  result = baseValue & operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::BitwiseXor(RegisterPointer reg, RegisterGet8Bit GetOperand)
    {
        const uint8_t baseValue = core.AF.HighByte();
        const uint8_t operandValue = ((core.*reg).*GetOperand)();
        const uint8_t result = baseValue ^ operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::BitwiseXor_Addr(RegisterPointer addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        const uint8_t  baseValue = core.AF.HighByte();
        const uint8_t  operandValue = memoryBus.ReadByte(address);
        const uint8_t  result = baseValue ^ operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::BitwiseXor_ImmediateData()
    {
        const uint8_t  baseValue = core.AF.HighByte();
        const uint8_t  operandValue = FetchPcAddress();
        const uint8_t  result = baseValue ^ operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::BitwiseOr(RegisterPointer reg, RegisterGet8Bit GetOperand)
    {
        const uint8_t baseValue = core.AF.HighByte();
        const uint8_t operandValue = ((core.*reg).*GetOperand)();
        const uint8_t result = baseValue | operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::BitwiseOr_Addr(RegisterPointer addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        const uint8_t  baseValue = core.AF.HighByte();
        const uint8_t  operandValue = memoryBus.ReadByte(address);
        const uint8_t  result = baseValue | operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::BitwiseOr_ImmediateData()
    {
        const uint8_t  baseValue = core.AF.HighByte();
        const uint8_t  operandValue = FetchPcAddress();
        const uint8_t  result = baseValue | operandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, false);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

} // namespace GameBoy
