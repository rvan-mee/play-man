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
    size_t Cpu::Increment_8bit_High(Register CpuCore:: *reg)
    {
        Register& r = core.*reg;
        const uint8_t val = r.HighByte();
        const uint8_t res = val + 1;

        r.SetHighByte(res);

        core.SetFlag(FlagRegisterFlag::ZERO, res == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) + 1 > 0xF));

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
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) + 1 > 0xF));

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
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) + 1 > 0xF));

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
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) == 0));	

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
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) == 0));	

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
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((val & 0xF) == 0));

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
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((regContents & 0xF) - (data & 0xF)) < 0);
        core.SetFlag(FlagRegisterFlag::CARRY, regContents < data);

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

        core.AF.SetHighByte(fromValue + baseValue);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(fromValue + baseValue) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, (baseValue & 0xF) + (fromValue & 0xF) > 0xF);
        core.SetFlag(FlagRegisterFlag::CARRY, baseValue + fromValue > 0xFF);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Add_8bit_Low(Register CpuCore:: *fromReg)
    {
        const Register& from = core.*fromReg;
        const uint16_t fromValue = from.LowByte();
        const uint16_t baseValue = core.AF.HighByte();

        core.AF.SetHighByte(fromValue + baseValue);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(fromValue + baseValue) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, (baseValue & 0xF) + (fromValue & 0xF) > 0xF);
        core.SetFlag(FlagRegisterFlag::CARRY, baseValue + fromValue > 0xFF);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Add_8bit_Addr(Register CpuCore:: *addrReg)
    {
        const uint16_t addr = (core.*addrReg).Value();
        const uint16_t fromValue = memoryBus.ReadByte(addr);
        const uint16_t baseValue = core.AF.HighByte();

        core.AF.SetHighByte(fromValue + baseValue);

        core.SetFlag(FlagRegisterFlag::ZERO, static_cast<uint8_t>(fromValue + baseValue) == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, (baseValue & 0xF) + (fromValue & 0xF) > 0xF);
        core.SetFlag(FlagRegisterFlag::CARRY, baseValue + fromValue > 0xFF);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

} // namespace GameBoy
