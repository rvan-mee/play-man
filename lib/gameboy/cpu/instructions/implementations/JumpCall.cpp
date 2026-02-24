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

    size_t Cpu::Jump_Addr(RegisterPointer addrReg)
    {
        Register&      r = (core.*addrReg);
        const uint16_t newPC = r.Value(); 

        core.PC = newPC;

        constexpr auto numberOfCycles = 1;
        return numberOfCycles;
    }

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

    size_t Cpu::Jump_Conditional_16bit_ImmediateData(FlagRegisterFlag flag, bool flagCondition)
    {
        const bool flagSet = core.GetFlag(flag);
        const uint8_t lowerByte = FetchPcAddress();
        const uint8_t upperByte = FetchPcAddress();
        auto numberOfCycles = 3;

        if (flagSet == flagCondition)
        {
            core.PC.SetValue((upperByte << 8) | lowerByte);
            numberOfCycles += 1;
        }
        return numberOfCycles;
    }

    size_t Cpu::Call_16bit_ImmediateData()
    {
        const uint8_t lowerByte = FetchPcAddress();
        const uint8_t upperByte = FetchPcAddress();

        memoryBus.PushStack(core.PC.Value());
    
        core.PC.SetValue((upperByte << 8) | lowerByte);

        constexpr auto numberOfCycles = 6;
        return numberOfCycles;
    }

    size_t Cpu::ConditionalCall_16bit_ImmediateData(FlagRegisterFlag flag, bool flagCondition)
    {
        const bool flagSet = core.GetFlag(flag);
        const uint8_t lowerByte = FetchPcAddress();
        const uint8_t upperByte = FetchPcAddress();
        auto numberOfCycles = 3;

        if (flagSet == flagCondition)
        {
            memoryBus.PushStack(core.PC.Value());

            core.PC.SetValue((upperByte << 8) | lowerByte);

            numberOfCycles += 3;
        }
        return numberOfCycles;
    }

    size_t Cpu::Return()
    {
        const uint16_t oldPC = memoryBus.PopStack();

        core.PC.SetValue(oldPC);

        constexpr auto numberOfCycles = 4;
        return numberOfCycles;
    }

    size_t Cpu::ConditionalReturn(FlagRegisterFlag conditionalFlag, bool flagEnabled)
    {
        auto numberOfCycles = 2;
        if (core.GetFlag(conditionalFlag) == flagEnabled)
        {
            Return();
            numberOfCycles += 3;
        }
        return numberOfCycles;
    }

    size_t Cpu::InterruptReturn()
    {
        const uint16_t oldPC = memoryBus.PopStack();

        core.PC.SetValue(oldPC);
        core.IME = true;

        constexpr auto numberOfCycles = 4;
        return numberOfCycles;
    }

}
