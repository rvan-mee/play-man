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

    void Cpu::LogInstruction()
    {
        std::stringstream ss;

        ss << "Executing instruction: ";
        ss << (opcodeIsPrefixed ? GetEnumAsString(static_cast<PrefixedOpCode>(currentOpcode)) : GetEnumAsString(static_cast<OpCode>(currentOpcode)));
        ss << " - Opcode: " << Utility::IntAsHexString(currentOpcode);
        LOG_DEBUG(ss.str());
    }

    void Cpu::ExecuteInstruction()
    {
        try
        {
            std::cout << "\nCore before instruction:\n" << core;
            instructions[opcodeIsPrefixed].at(currentOpcode)();
            LogInstruction();
            std::cout << "Core after instruction:\n" << core;
        }
        catch(const std::exception& e)
        {
            std::string logMessage;
            
            logMessage = "Illegal instruction call for";
            logMessage += (opcodeIsPrefixed ? " prefixed " : " ") ;
            logMessage += "opcode: ";
            logMessage += Utility::IntAsHexString(currentOpcode);
            LOG_FATAL(logMessage);
            assert(false);
        }

        // TODO: handle instruction timing
        // Take branching into account
        // cyclesPassed += cycleTable[opcodeIsPrefixed].at(currentOpcode);
    }

    void Cpu::FetchInstruction()
    {
        currentOpcode = memoryBus.ReadByte(core.PC++);
        if (currentOpcode == opcodePrefix)
        {
            opcodeIsPrefixed = true;
            currentOpcode = memoryBus.ReadByte(core.PC++);

			nextInstructionToExecute = Instruction(OpCode::PREFIX, static_cast<PrefixedOpCode>(currentOpcode), prefixedInstructions.at(currentOpcode));
        }
        else
        {
            opcodeIsPrefixed = false;
			nextInstructionToExecute = Instruction(static_cast<OpCode>(currentOpcode), std::nullopt, instructions.at(currentOpcode));
        }
    }
}
