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
        instructions[opCode]();
    }

    void Cpu::ExecuteInstruction(PrefixedOpCode opCode)
    {
        prefixedInstructions[opCode]();
    }

    void Cpu::LogInstruction()
    {
        std::stringstream ss;

        ss << "Executing instruction: ";
        ss << currentInstruction;
        ss << " - Opcode: " << Utility::IntAsHexString(currentOpcode);
        LOG_DEBUG(ss.str());
    }

    void Cpu::ExecuteInstruction()
    {
        try
        {
            std::cout << "\nCore before instruction:\n" << core;
            cycles += currentInstruction.Execute();
            LogInstruction();
            std::cout << "Core after instruction:\n" << core;
        }
        catch (const std::exception& e)
        {
            std::string logMessage;
            
            logMessage = "Illegal instruction call for";
            logMessage += (currentInstruction.IsPrefixed() ? " prefixed " : " ") ;
            logMessage += "opcode: ";
            logMessage += Utility::IntAsHexString(currentOpcode);
            LOG_FATAL(logMessage);
            abort();
        }

        // TODO: handle instruction timing
        // Take branching into account
        // cyclesPassed += cycleTable[opcodeIsPrefixed].at(currentOpcode);
    }

    void Cpu::FetchInstruction()
    {
        currentOpcode = memoryBus.ReadByte(core.PC++);
        if (currentOpcode == GetEnumAsValue(OpCode::PREFIX))
        {
            currentOpcode = memoryBus.ReadByte(core.PC++);
			LOG_INFO("here");
			currentInstruction = Instruction(static_cast<PrefixedOpCode>(currentOpcode), prefixedInstructions.at(currentOpcode));
        }
        else
        {
			currentInstruction = Instruction(static_cast<OpCode>(currentOpcode), instructions.at(currentOpcode));
        }
    }
}
