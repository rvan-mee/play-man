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

#define HALF_CARRY_BIT 0b1'0000
#define CARRY_BIT 0b1'0000'0000

namespace GameBoy
{
    size_t Cpu::Increment_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& operandRegister = core.*opReg;
        const uint8_t value = (operandRegister.*GetValue)();
        const uint8_t result = value + 1;

        (operandRegister.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((value & 0xF) + 1) & HALF_CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Increment_Dereferenced(RegisterPointer addrReg)
    {
        const auto address = (core.*addrReg).Value();
        const uint8_t value = memoryBus.ReadByte(address);
        const uint8_t result = value + 1;

        memoryBus.WriteByte(address, result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((value & 0xF) + 1) & HALF_CARRY_BIT);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Decrement_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue, RegisterSet8Bit SetValue)
    {
        Register& operandRegister = core.*opReg;
        const uint8_t value = (operandRegister.*GetValue)();
        const uint8_t result = value - 1;

        (operandRegister.*SetValue)(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((value & 0xF) - 1) & HALF_CARRY_BIT);	

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Decrement_Dereferenced(RegisterPointer addrReg)
    {
        const auto address = (core.*addrReg).Value();
        const uint8_t value = memoryBus.ReadByte(address);
        const uint8_t result = value - 1;

        memoryBus.WriteByte(address, result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((value & 0xF) - 1) & HALF_CARRY_BIT);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Compare_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue)
    {
        const uint8_t baseValue = core.AF.HighByte();
        const uint8_t operand = ((core.*opReg).*GetValue)();
        const uint8_t compareResult = baseValue - operand;

        core.SetFlag(FlagRegisterFlag::ZERO, compareResult == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (operand & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, (baseValue - operand) & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Compare_8bit_Addr(RegisterPointer addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        const uint8_t  baseValue = core.AF.HighByte();
        const uint8_t  operand = memoryBus.ReadByte(address);
        const uint8_t  compareResult = baseValue - operand;

        core.SetFlag(FlagRegisterFlag::ZERO, compareResult == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (operand & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, (baseValue - operand) & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Compare_8bit_ImmediateData()
    {
        const uint8_t baseValue = core.AF.HighByte();
        const uint8_t operand = FetchPcAddress();
        const uint8_t compareResult = baseValue - operand;

        core.SetFlag(FlagRegisterFlag::ZERO, compareResult == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (operand & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, (baseValue - operand) & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Increment_16bit(RegisterPointer reg)
    {
        Register& r = core.*reg;

        r++;
        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Decrement_16bit(RegisterPointer reg)
    {
        Register& r = core.*reg;

        r--;
        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Add_16bit(RegisterPointer toReg, RegisterPointer fromReg)
    {
        Register& to = core.*toReg;
        const Register& from = core.*fromReg;
        const uint16_t fromValue = from.Value();
        const uint16_t toValue = to.Value();

        to.SetValue(fromValue + toValue);

        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, (toValue & 0xFFF) + (fromValue & 0xFFF) > 0xFFF);
        core.SetFlag(FlagRegisterFlag::CARRY, static_cast<uint32_t>(toValue) + static_cast<uint32_t>(fromValue) > 0xFFFF);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Add_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue)
    {
        const Register& operandRegister = core.*opReg;
        const uint16_t operandValue = (operandRegister.*GetValue)();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = operandValue + baseValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (operandValue & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Add_8bit_ImmediateData()
    {
        const uint16_t operandValue = FetchPcAddress();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = operandValue + baseValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (operandValue & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Add_8bit_Addr(RegisterPointer addrReg)
    {
        const uint16_t addr = (core.*addrReg).Value();
        const uint16_t operand = memoryBus.ReadByte(addr);
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = operand + baseValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (operand & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::AddCarry_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue)
    {
        const Register& operandRegister = core.*opReg;
        const uint16_t operand = (operandRegister.*GetValue)();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t result = operand + baseValue + carryValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (operand & 0xF) + carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::AddCarry_8bit_ImmediateData()
    {
        const uint16_t operand = FetchPcAddress();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t result = operand + baseValue + carryValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (operand & 0xF) + carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::AddCarry_8bit_Addr(RegisterPointer addrReg)
    {
        const uint16_t addr = (core.*addrReg).Value();
        const uint16_t operand = memoryBus.ReadByte(addr);
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t result = operand + baseValue + carryValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, (baseValue & 0xF) + (operand & 0xF) + carryValue > 0xF);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Sub_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue)
    {
        const uint16_t operand = ((core.*opReg).*GetValue)();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - operand;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (operand & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Sub_8bit_ImmediateData()
    {
        const uint16_t operand = FetchPcAddress();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - operand;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (operand & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;  
    }

    size_t Cpu::Sub_8bit_Addr(RegisterPointer addrReg)
    {
        const uint16_t addr = (core.*addrReg).Value();
        const uint16_t operand = memoryBus.ReadByte(addr);
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - operand;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (operand & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::SubCarry_8bit(RegisterPointer opReg, RegisterGet8Bit GetValue)
    {
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t operand = ((core.*opReg).*GetValue)();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - operand - carryValue;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (operand & 0xF) - carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::SubCarry_8bit_ImmediateData()
    {
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t operand = FetchPcAddress();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - operand - carryValue;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (operand & 0xF) - carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::SubCarry_8bit_Addr(RegisterPointer addrReg)
    {
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        uint16_t addr = (core.*addrReg).Value();
        uint16_t fromValue = memoryBus.ReadByte(addr);
        uint16_t baseValue = core.AF.HighByte();
        uint16_t result = baseValue - fromValue - carryValue;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (fromValue & 0xF) - carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

} // namespace GameBoy
