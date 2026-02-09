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

#define HALF_CARRY_BIT 0b1'0000
#define CARRY_BIT 0b1'0000'0000

namespace GameBoy
{

    size_t Cpu::Store_8bit_8bitImmediateAddr(RegisterPointer dataReg, RegisterGet8Bit GetData)
    {
        uint8_t data = ((core.*dataReg).*GetData)();
        uint16_t address = 0xFF'00 | FetchPcAddress();

        memoryBus.WriteByte(address, data);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Store_8bit_16bitImmediateAddr(RegisterPointer dataReg, RegisterGet8Bit GetData)
    {
        uint8_t data = ((core.*dataReg).*GetData)();
        uint16_t address = FetchPcAddress16bit();

        memoryBus.WriteByte(address, data);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Store_8bit_8bitAddr(RegisterPointer addrReg, RegisterGet8Bit GetAddr, RegisterPointer dataReg, RegisterGet8Bit GetData)
    {
        uint8_t addrValue = ((core.*addrReg).*GetAddr)();
        uint8_t data = ((core.*dataReg).*GetData)();
        uint16_t addressRegister = 0xFF'00 | addrValue;

        memoryBus.WriteByte(addressRegister, data);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Store_8bit_Addr(RegisterPointer addrReg, RegisterPointer dataReg, RegisterGet8Bit GetData)
    {
        uint8_t data = ((core.*dataReg).*GetData)();
        uint16_t addressRegister = (core.*addrReg).Value();

        memoryBus.WriteByte(addressRegister, data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Store_8bit_AddrIncrement(RegisterPointer addrReg, RegisterPointer dataReg, RegisterGet8Bit GetData)
    {
        uint8_t data = ((core.*dataReg).*GetData)();
        Register& addressRegister = (core.*addrReg);

        memoryBus.WriteByte(addressRegister.Value(), data);
        addressRegister++;

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles; 
    }

    size_t Cpu::Store_8bit_AddrDecrement(RegisterPointer addrReg, RegisterPointer dataReg, RegisterGet8Bit GetData)
    {
        uint8_t data = ((core.*dataReg).*GetData)();
        Register& addressRegister = (core.*addrReg);

        memoryBus.WriteByte(addressRegister.Value(), data);
        addressRegister--;

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles; 
    }

    size_t Cpu::Store_8bit_Addr_ImmediateData(RegisterPointer addrReg)
    {
        const uint8_t data = FetchPcAddress();
        const uint16_t address = (core.*addrReg).Value();

        memoryBus.WriteByte(address, data);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Store_StackPointerPlusSignedImmediateData(RegisterPointer reg)
    {
        const uint16_t baseValue = core.SP.Value();
        const int8_t immData = FetchPcAddress();
        const uint16_t result = baseValue + immData;

        (core.*reg).SetValue(result);

        core.SetFlag(FlagRegisterFlag::ZERO, false);
        core.SetFlag(FlagRegisterFlag::SUB, false);
        core.SetFlag(FlagRegisterFlag::HALF_CARRY, ((baseValue & 0xF) + (immData & 0xF)) & HALF_CARRY_BIT);
        core.SetFlag(FlagRegisterFlag::CARRY, ((baseValue & 0xFF) + (immData & 0xFF)) & CARRY_BIT);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_ImmediateData(RegisterPointer reg, RegisterSet8Bit SetValue)
    {
        Register& r = core.*reg;
        const uint8_t data = FetchPcAddress();

        (r.*SetValue)(data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_ImmediateAddr(RegisterPointer reg, RegisterSet8Bit SetValue)
    {
        const uint16_t addr = FetchPcAddress16bit();
        const uint8_t data = memoryBus.ReadByte(addr);
        Register& r = core.*reg;

        (r.*SetValue)(data);

        constexpr size_t numberOfCycles = 4;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_8bitImmediateAddr(RegisterPointer reg, RegisterSet8Bit SetValue)
    {
        const uint16_t addr = 0xFF00 + FetchPcAddress();
        const uint8_t data = memoryBus.ReadByte(addr);
        Register& r = core.*reg;

        (r.*SetValue)(data);

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_Addr(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer addrReg)
    {
        const uint16_t address = (core.*addrReg).Value();
        Register& dest = core.*destReg;

        (dest.*SetValue)(memoryBus.ReadByte(address));

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_8bitAddr(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer addrReg, RegisterGet8Bit GetAddr)
    {
        const uint16_t address = 0xFF00 + ((core.*addrReg).*GetAddr)();
        const uint8_t data = memoryBus.ReadByte(address);
        Register& dest = core.*destReg;

        (dest.*SetValue)(data);

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_AddrIncrement(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer addrReg)
    {
        Register& dest = core.*destReg;
        Register& addr = core.*addrReg;

        (dest.*SetValue)(memoryBus.ReadByte(addr.Value()));
        addr++;

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_8bit_AddrDecrement(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer addrReg)
    {
        Register& dest = core.*destReg;
        Register& addr = core.*addrReg;

        (dest.*SetValue)(memoryBus.ReadByte(addr.Value()));
        addr--;

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;   
    }

    size_t Cpu::Load_8bit(RegisterPointer destReg, RegisterSet8Bit SetValue, RegisterPointer fromReg, RegisterGet8Bit GetValue)
    {
        Register& dest = core.*destReg;
        Register& from = core.*fromReg;

        (dest.*SetValue)((from.*GetValue)());

        constexpr size_t numberOfCycles = 1;
        return numberOfCycles;
    }

    size_t Cpu::Load_16bit(RegisterPointer destReg, RegisterPointer fromReg)
    {
        Register& dest = core.*destReg;
        Register& from = core.*fromReg;

        dest.SetValue(from.Value());

        constexpr size_t numberOfCycles = 2;
        return numberOfCycles;
    }

    size_t Cpu::Load_16bit_ImmediateData(RegisterPointer reg)
    {
        Register& r = core.*reg;
        r.SetLowByte(FetchPcAddress());
        r.SetHighByte(FetchPcAddress());

        constexpr auto numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::Load_16bit_RegToImmediateAddr(RegisterPointer reg)
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

    size_t Cpu::Push(RegisterPointer reg)
    {
        const Register& r = core.*reg;
        const uint16_t value = r.Value();

        memoryBus.PushStack(value);

        constexpr size_t numberOfCycles = 4;
        return numberOfCycles;
    }

    size_t Cpu::Pop(RegisterPointer reg)
    {
        Register& r = core.*reg;

        r.SetValue(memoryBus.PopStack());

        constexpr size_t numberOfCycles = 3;
        return numberOfCycles;
    }

    size_t Cpu::RST(const uint8_t address)
    {
        memoryBus.PushStack(core.PC.Value());

        core.PC.SetValue(address);

        constexpr size_t numberOfCycles = 4;
        return numberOfCycles;
    }

}
