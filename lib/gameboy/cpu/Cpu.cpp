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
#include <play-man/logger/Logger.hpp>
#include <play-man/utility/UtilFunc.hpp>

namespace GameBoy
{
    void Cpu::ExecuteInstruction(OpCode opCode)
    {
        instructions[0][static_cast<uint8_t>(opCode)]();
    }

    void Cpu::ExecuteInstruction(PrefixedOpCode opCode)
    {
        instructions[1][static_cast<uint8_t>(opCode)]();
    }

    void Cpu::ExecuteInstruction()
    {
        try
        {
            instructions[opcodeIsPrefixed].at(currentOpcode)();
            /* code */
        }
        catch(const std::exception& e)
        {
            std::string logMessage;

            logMessage = "Illegal instruction call for";
            logMessage += (opcodeIsPrefixed ? " prefixed " : " ") ;
            logMessage += "opcode: ";
            logMessage += Utility::Uint8AsHexString(currentOpcode);
            LOG_FATAL(logMessage);
            assert(false);
        }

        // TODO: handle instruction timing
    }

    void Cpu::FetchInstruction()
    {
        currentOpcode = memoryBus.ReadByte(core.PC++);
        if (currentOpcode == opcodePrefix)
        {
            opcodeIsPrefixed = true;
            currentOpcode = memoryBus.ReadByte(core.PC++);
        }
        else
        {
            opcodeIsPrefixed = false;
        }
    }
}
