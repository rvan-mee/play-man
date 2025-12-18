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
    void Cpu::LoadTestRom(const char* filePath)
    {
        core.ClearRegisters();
        rom.LoadTestRom(filePath);
    }

    void Cpu::ExecuteInstruction(OpCode opCode)
    {
        instructions[opCode](this);
    }

    void Cpu::ExecuteInstruction(PrefixedOpCode opCode)
    {
        prefixedInstructions[opCode](this);
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
            cycles += currentInstruction.Execute(this);
            LogInstruction();
            std::cout << "Core after instruction:\n" << core;
        }
        catch (const std::exception& e)
        {
            LOG_FATAL("Failed to execute instruction " + currentInstruction.OpCodeAsHexString() + ": " + e.what());
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

    uint16_t Cpu::FetchPcAddress16bit()
    {
		const auto data = (Fetch(core.PC.Value()) << 8) | Fetch(core.PC.Value() + 1);
		core.PC++;
        core.PC++;
		return data;
    }

    void Cpu::FetchInstruction()
    {
		try
		{
			const auto opCode = static_cast<OpCode>(FetchPcAddress());
			if (opCode == OpCode::PREFIX)
			{
				const auto prefixedOpCode = static_cast<PrefixedOpCode>(FetchPcAddress());
				currentInstruction = Instruction(prefixedOpCode, prefixedInstructions.at(prefixedOpCode));
			}
			else
			{
				currentInstruction = Instruction(opCode, instructions.at(opCode));
			}
		}
		catch (const std::exception& e)
		{
			LOG_FATAL(std::string("Unable to fetch instruction: ") + e.what());
			abort();
		}
    }
}
