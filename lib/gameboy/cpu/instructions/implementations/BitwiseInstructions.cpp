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

    size_t Cpu::BitwiseAnd_High(Register CpuCore::* oppReg)
    {
        const uint8_t baseValue = core.AF.HighByte();
        const uint8_t opperandValue = (core.*oppReg).HighByte();
        const uint8_t result = baseValue & opperandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }


    size_t Cpu::BitwiseAnd_Low(Register CpuCore::* oppReg)
    {
        const uint8_t baseValue = core.AF.HighByte();
        const uint8_t opperandValue = (core.*oppReg).LowByte();
        const uint8_t result = baseValue & opperandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }


    size_t Cpu::BitwiseAnd_Addr(Register CpuCore::* addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        const uint8_t  baseValue = core.AF.HighByte();
        const uint8_t  opperandValue = memoryBus.ReadByte(address);
        const uint8_t  result = baseValue & opperandValue;

        core.AF.SetHighByte(result);

        core.SetFlag(FlagRegisterFlag::ZERO, result == 0);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, true);
        core.SetFlag(FlagRegisterFlag::CARRY, false);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }
} // namespace GameBoy
