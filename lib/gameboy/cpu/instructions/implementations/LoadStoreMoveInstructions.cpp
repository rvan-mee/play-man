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

    size_t Cpu::Store_8bit_Addr_High(Register CpuCore::* addrReg, Register CpuCore::* dataReg)
    {
        uint8_t data = (core.*dataReg).HighByte();
        uint16_t addr = (core.*addrReg).Value();

        memoryBus.WriteByte(addr, data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Store_8bit_Addr_Low(Register CpuCore::* addrReg, Register CpuCore::* dataReg)
    {
        uint8_t data = (core.*dataReg).LowByte();
        uint16_t addr = (core.*addrReg).Value();

        memoryBus.WriteByte(addr, data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Store_8bit_AddrIncrement_High(Register CpuCore::* addrReg, Register CpuCore::* dataReg)
    {
        uint8_t data = (core.*dataReg).HighByte();
        Register& addr = (core.*addrReg);

        memoryBus.WriteByte(addr.Value(), data);
        addr++;

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles; 
    }

    size_t Cpu::Store_8bit_AddrDecrement_High(Register CpuCore::* addrReg, Register CpuCore::* dataReg)
    {
        uint8_t data = (core.*dataReg).HighByte();
        Register& addr = (core.*addrReg);

        memoryBus.WriteByte(addr.Value(), data);
        addr--;

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles; 
    }

    size_t Cpu::Store_8bit_Addr_ImmediateData(Register CpuCore::* addrReg)
    {
        const uint8_t data = FetchPcAddress();
        const uint16_t addr = (core.*addrReg).Value();

        memoryBus.WriteByte(addr, data);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_High_ImmediateData(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t data = FetchPcAddress();

        r.SetHighByte(data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_Low_ImmediateData(Register CpuCore::* reg)
    {
        Register& r = core.*reg;
        const uint8_t data = FetchPcAddress();

        r.SetLowByte(data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_Low_Addr(Register CpuCore::* destReg, Register CpuCore::* addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        Register& dest = core.*destReg;

        dest.SetLowByte(memoryBus.ReadByte(address));

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_High_Addr(Register CpuCore::* destReg, Register CpuCore::* addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        Register& dest = core.*destReg;

        dest.SetHighByte(memoryBus.ReadByte(address));

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_High_AddrIncrement(Register CpuCore::* destReg, Register CpuCore::* addrReg)
    {
        Register& dest = core.*destReg;
        Register& addr = core.*addrReg;

        dest.SetHighByte(memoryBus.ReadByte(addr.Value()));
        addr++;

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_High_AddrDecrement(Register CpuCore::* destReg, Register CpuCore::* addrReg)
    {
        Register& dest = core.*destReg;
        Register& addr = core.*addrReg;

        dest.SetHighByte(memoryBus.ReadByte(addr.Value()));
        addr--;

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;   
    }

    size_t Cpu::Load_8bit_High_High(Register CpuCore::* toReg, Register CpuCore::* fromReg)
    {
        Register& to = core.*toReg;
        Register& from = core.*fromReg;

        to.SetHighByte(from.HighByte());

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_High_Low(Register CpuCore::* toReg, Register CpuCore::* fromReg)
    {
        Register& to = core.*toReg;
        Register& from = core.*fromReg;

        to.SetHighByte(from.LowByte());

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_Low_High(Register CpuCore::* toReg, Register CpuCore::* fromReg)
    {
        Register& to = core.*toReg;
        Register& from = core.*fromReg;

        to.SetLowByte(from.HighByte());

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_Low_Low(Register CpuCore::* toReg, Register CpuCore::* fromReg)
    {
        Register& to = core.*toReg;
        Register& from = core.*fromReg;

        to.SetLowByte(from.LowByte());

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

	size_t Cpu::Load_16bit_ImmediateData(Register CpuCore::* reg)
	{
		Register& r = core.*reg;
		r.SetLowByte(FetchPcAddress());
		r.SetHighByte(FetchPcAddress());

		constexpr auto numberOfCycles = 3;
		return numberOfCycles;
	}

	size_t Cpu::Load_16bit_RegToImmediateAddr(Register CpuCore::* reg)
	{
		const Register& r = core.*reg;
		const auto lowByte = r.LowByte();
		const auto highByte = r.HighByte();

		const auto addr = FetchPcAddress16bit();

		memoryBus.WriteByte(addr, lowByte);
		memoryBus.WriteByte(addr+1, highByte);

		constexpr size_t numberOfCycles = 5;
		return numberOfCycles;
	}

}
