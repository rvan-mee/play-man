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

namespace GameBoy {

    
    size_t Cpu::Jump_16bit_ImmediateData()
    {
        Register tmpReg;

        tmpReg.SetLowByte(FetchPcAddress());
        tmpReg.SetHighByte(FetchPcAddress());

        core.PC = tmpReg;

        constexpr auto numberOfCycles = 4;
        return numberOfCycles;
    }

    size_t Cpu::Jump_Relative_8bit_SignedImmediateData()
    {
        const uint8_t dist = FetchPcAddress();

        if (dist > 127)
            core.PC -= dist;
        else
            core.PC += dist;

        constexpr auto numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Jump_Relative_Conditional_8bit_SignedImmediateData(FlagRegisterFlag flag, bool flagCondition)
    {
        const bool flagSet = core.GetFlag(flag);
        const auto dist = FetchPcAddress();
        auto numberOfCycles = 2;

        if (flagSet == flagCondition)
        {
            numberOfCycles += 1;
            if (dist > 127)
                core.PC -= dist;
            else
                core.PC += dist;
        }
        return numberOfCycles;
    }

    size_t Cpu::Return()
    {
        const uint16_t oldPC = memoryBus.PopStack();

        core.PC.SetValue(oldPC);

        constexpr auto numberOfCycles = 16;
        return numberOfCycles;
    }

    size_t Cpu::ConditionalReturn(FlagRegisterFlag conditionalFlag, bool flagEnabled)
    {
        auto numberOfCycles = 8;
        if (core.GetFlag(conditionalFlag) == flagEnabled)
        {
            Return();
            numberOfCycles += 12;
        }
        return numberOfCycles;
    }

}
