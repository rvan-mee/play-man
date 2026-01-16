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
    size_t Cpu::Increment_8bit_High(Register CpuCore:: *reg)
    {
        Register& r = core.*reg;
        const uint8_t val = r.HighByte();
        const uint8_t res = val + 1;

        r.SetHighByte(res);

        core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) + 1) & HALF_CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Increment_8bit_Low(Register CpuCore:: *reg)
    {
        Register& r = core.*reg;
        const uint8_t val = r.LowByte();
        const uint8_t res = val + 1;

        r.SetLowByte(res);

        core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) + 1) & HALF_CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Increment_Dereferenced(Register CpuCore::* addrReg)
    {
        const auto address = (core.*addrReg).Value();
        const uint8_t val = memoryBus.ReadByte(address);
        const uint8_t res = val + 1;

        memoryBus.WriteByte(address, res);

        core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) + 1) & HALF_CARRY_BIT);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Decrement_8bit_High(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t val = r.HighByte();
        const uint8_t res = val - 1;

        r.SetHighByte(res);

        core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) - 1) & HALF_CARRY_BIT);	

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Decrement_8bit_Low(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t val = r.LowByte();
        const uint8_t res = val - 1;

        r.SetLowByte(res);

        core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) - 1) & HALF_CARRY_BIT);	

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Decrement_Dereferenced(Register CpuCore::* addrReg)
    {
        const auto address = (core.*addrReg).Value();
        const uint8_t val = memoryBus.ReadByte(address);
        const uint8_t res = val - 1;

        memoryBus.WriteByte(address, res);

        core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) - 1) & HALF_CARRY_BIT);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Compare_8bit_High_ImmediateData(Register CpuCore::*reg)
    {
        const uint8_t regContents = (core.*reg).HighByte();
        const uint8_t data = FetchPcAddress();
        const uint8_t compareResult = regContents - data;

        core.SetFlag(FlagRegisterFlag::ZERO, compareResult == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((regContents & 0xF) - (data & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, (regContents - data) & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Increment_16bit(Register CpuCore::* reg)
    {
        Register& r = core.*reg;

        r++;
        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Decrement_16bit(Register CpuCore::* reg)
    {
        Register& r = core.*reg;

        r--;
        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Add_16bit(Register CpuCore::* toReg, Register CpuCore::* fromReg)
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

    size_t Cpu::Add_8bit_High(Register CpuCore:: *fromReg)
    {
        const Register& from = core.*fromReg;
        const uint16_t fromValue = from.HighByte();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = fromValue + baseValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (fromValue & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Add_8bit_Low(Register CpuCore:: *fromReg)
    {
        const Register& from = core.*fromReg;
        const uint16_t fromValue = from.LowByte();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = fromValue + baseValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (fromValue & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Add_8bit_Addr(Register CpuCore:: *addrReg)
    {
        const uint16_t addr = (core.*addrReg).Value();
        const uint16_t fromValue = memoryBus.ReadByte(addr);
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = fromValue + baseValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (fromValue & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::AddCarry_8bit_High(Register CpuCore:: *fromReg)
    {
        const Register& from = core.*fromReg;
        const uint16_t fromValue = from.HighByte();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t result = fromValue + baseValue + carryValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (fromValue & 0xF) + carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::AddCarry_8bit_Low(Register CpuCore:: *fromReg)
    {
        const Register& from = core.*fromReg;
        const uint16_t fromValue = from.LowByte();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t result = fromValue + baseValue + carryValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (fromValue & 0xF) + carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::AddCarry_8bit_Addr(Register CpuCore:: *addrReg)
    {
        const uint16_t addr = (core.*addrReg).Value();
        const uint16_t fromValue = memoryBus.ReadByte(addr);
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t result = fromValue + baseValue + carryValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, (baseValue & 0xF) + (fromValue & 0xF) + carryValue > 0xF);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Sub_8bit_High(Register CpuCore:: *fromReg)
    {
        const uint16_t fromValue = (core.*fromReg).HighByte();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - fromValue;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (fromValue & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Sub_8bit_Low(Register CpuCore:: *fromReg)
    {
        const uint16_t fromValue = (core.*fromReg).LowByte();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - fromValue;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (fromValue & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Sub_8bit_Addr(Register CpuCore:: *addrReg)
    {
        const uint16_t addr = (core.*addrReg).Value();
        const uint16_t fromValue = memoryBus.ReadByte(addr);
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - fromValue;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (fromValue & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::SubCarry_8bit_High(Register CpuCore:: *fromReg)
    {
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t fromValue = (core.*fromReg).HighByte();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - fromValue - carryValue;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (fromValue & 0xF) - carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::SubCarry_8bit_Low(Register CpuCore:: *fromReg)
    {
        const uint16_t carryValue = 1 * core.GetFlag(FlagRegisterFlag::CARRY);
        const uint16_t fromValue = (core.*fromReg).LowByte();
        const uint16_t baseValue = core.AF.HighByte();
        const uint16_t result = baseValue - fromValue - carryValue;

        core.AF.SetHighByte(static_cast<uint8_t>(result));

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(result) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, true);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) - (fromValue & 0xF) - carryValue) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, result & CARRY_BIT);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::SubCarry_8bit_Addr(Register CpuCore:: *addrReg)
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
