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
    size_t Cpu::Increment_16bit(Register CpuCore::* reg)
    {
        Register& r = core.*reg;

        r++;
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
}
