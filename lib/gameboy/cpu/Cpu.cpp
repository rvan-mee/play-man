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
#include <play-man/gameboy/cpu/CycleDefines.hpp>

constexpr uint16_t InterruptHandlerAddressVBlank = 0x00'40;
constexpr uint16_t InterruptHandlerAddressLCD    = 0x00'48;
constexpr uint16_t InterruptHandlerAddressTimer  = 0x00'50;
constexpr uint16_t InterruptHandlerAddressSerial = 0x00'58;
constexpr uint16_t InterruptHandlerAddressJoypad = 0x00'60;

namespace GameBoy
{
    void Cpu::LoadTestRom(const char* filePath)
    {
        core.ClearRegisters();
        cartridge->LoadTestRom(filePath);
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

        ss << "Executed instruction: ";
        ss << currentInstruction;
        ss << " - Opcode: " << Utility::IntAsHexString(static_cast<uint16_t>(GetEnumAsValue(currentInstruction.GetOpCode())));
        LOG_DEBUG(ss.str());
    }

    void Cpu::ExecuteInstruction()
    {
        try
        {
            // Instructions return M-ticks, the CpuCycles are ticked every T-tick.
            CpuCyclesLeft += (currentInstruction.Execute(this) * M_Tick);
            LogInstruction();
        }
        catch (const std::exception& e)
        {
            LOG_FATAL("Failed to execute instruction " + currentInstruction.OpCodeAsHexString() + ": " + e.what());
            abort();
        }
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
		core.PC += 2;
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

    void Cpu::InstructionTick()
    {
        // Check if we can perform a fetch & execute or if we have to wait
        // till the current instruction has 'finished'.
        if (CpuCyclesLeft == 0)
        {
            HandleInterrupts();

            // Check if an interrupt handler was called
            if (CpuCyclesLeft != 0)
                return ;

            FetchInstruction();
            ExecuteInstruction();

            // If an EI instruction is called we need to update the IME flag at the right time
            UpdateIME();
        }
        else
            CpuCyclesLeft--;
    }

    ACartridge& Cpu::GetCartridge()
    {
        return *cartridge;
    }

    CpuCore& Cpu::GetCpuCore()
    {
        return core;
    }

    PPU& Cpu::GetPPU()
    {
        return ppu;
    }

    MemoryBus& Cpu::GetMemoryBus()
    {
        return memoryBus;
    }

    std::array<uint8_t, HighRamSize>& Cpu::GetHighRam()
    {
        return highRam;
    }

    void Cpu::UpdateIME()
    {
        if (core.stateIME == InterruptState::NONE)
            return ;
        else if (core.stateIME == InterruptState::DELAY_ENABLE_IME)
            core.stateIME = InterruptState::ENABLE_IME;
        else if (core.stateIME == InterruptState::ENABLE_IME)
        {
            core.stateIME = InterruptState::NONE;
            core.IME = true;
            LOG_DEBUG("Enabling IME");
        }
    }

    bool Cpu::HandleInterrupt(const InterruptFlags interruptToHandle, const uint16_t interruptHandlerAddress)
    {
        // This interrupt is not enabled or requested
        if (!(core.IE & core.IF & GetEnumAsValue(interruptToHandle)))
            return false;

        // This interrupt is both requested and enabled, CPU should reset the corresponding bit and the IME flag
        core.IME = false;
        core.IF = core.IF & ~GetEnumAsValue(interruptToHandle);

        // Jump to the interrupts handler after pushing the current program counter to the stack
        memoryBus.PushStack(core.PC.Value());
        core.PC.SetValue(interruptHandlerAddress);

        std::stringstream ss;
        ss << "Handling interrupt: ";
        ss << interruptToHandle;
        LOG_DEBUG(ss.str());

        return true;
    }

    void Cpu::HandleInterrupts()
    {
        // Interrupts are disabled or there are no enabled interrupts pending
        if (core.IME == false || (core.IE & core.IF) == 0)
            return ;

        if (HandleInterrupt(InterruptFlags::VBLANK, InterruptHandlerAddressVBlank) ||
            HandleInterrupt(InterruptFlags::LCD,    InterruptHandlerAddressLCD)    ||
            HandleInterrupt(InterruptFlags::TIMER,  InterruptHandlerAddressTimer)  ||
            HandleInterrupt(InterruptFlags::SERIAL, InterruptHandlerAddressSerial) ||
            HandleInterrupt(InterruptFlags::JOYPAD, InterruptHandlerAddressJoypad))
        {
            // An interrupt has been handled, this process has taken 5 M-Ticks
            CpuCyclesLeft += 5 * M_Tick;
        }
    }

    void Cpu::RenderFrame()
    {
        // Speed multiplication is done is by doing the logic for multiple
        // frames in a single RenderFrame.
        for (uint32_t Speed = 0; Speed < SpeedMultiplier; Speed++)
        {
            for (uint32_t Cycles = 0; Cycles < CyclesPerFrame; Cycles++)
            {
                // This loop produces T-ticks, it is up to the Tick() functions to handle
                // conversion from M-ticks to T-ticks.

                this->InstructionTick();
                // ppu.TickPPU();
                ppu.TickDMA();

            }
        }
        // TODO: Sleep for duration of the time left that a single frame should take on a GameBoy.
        // "roughly 16.7ms"
    }
}
