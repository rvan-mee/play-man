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
        ss << " - Opcode: " << Utility::IntAsHexString(GetEnumAsValue(currentInstruction.GetOpCode()));
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
            LOG_FATAL("Failed to exectue instruction " + currentInstruction.OpCodeAsHexString() + ": " + e.what());
            abort();
        }

        // TODO: handle instruction timing
        // Take branching into account
        // cyclesPassed += cycleTable[opcodeIsPrefixed].at(currentOpcode);
    }

	uint8_t Cpu::Fetch(uint16_t address)
	{
		return memoryBus.ReadByte(address);
	}

	uint8_t Cpu::FetchPcAddress()
	{	
		const auto data = Fetch(core.PC.Value());
		core.PC++;
		return data;
	}

    void Cpu::FetchInstruction()
    {
		try
		{
			auto currentOpcode = FetchPcAddress();
			if (currentOpcode == GetEnumAsValue(OpCode::PREFIX))
			{
				currentOpcode = FetchPcAddress();
				currentInstruction = Instruction(static_cast<PrefixedOpCode>(currentOpcode), prefixedInstructions.at(currentOpcode));
			}
			else
			{
				currentInstruction = Instruction(static_cast<OpCode>(currentOpcode), instructions.at(currentOpcode));
			}
		}
		catch (const std::exception& e)
		{
			LOG_FATAL(std::string("Unable to fetch instruction: ") + e.what());
			abort();
		}
    }
}
