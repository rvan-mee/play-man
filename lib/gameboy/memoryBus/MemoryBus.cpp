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

#include <play-man/gameboy/memoryBus/MemoryBus.hpp>
#include <play-man/gameboy/memoryBus/MemoryBusDefines.hpp>
#include <play-man/logger/Logger.hpp>
#include <assert.h>

namespace GameBoy {

MemoryBus::MemoryBus(Cpu* _cpu) : cpu(_cpu)
{
}

uint16_t MemoryBus::PopStack()
{
    uint8_t lowerByte;
    uint8_t upperByte;

    lowerByte = ReadByte(cpu->GetCpuCore().GetStackPointerInc());
    upperByte = ReadByte(cpu->GetCpuCore().GetStackPointerInc());

    return ((upperByte << 8) | lowerByte);
}

void MemoryBus::PushStack(uint16_t value)
{
    const uint8_t lowerByte = value & 0xFF;
    const uint8_t upperByte = (value & 0xFF00) >> 8;

    WriteByte(cpu->GetCpuCore().GetStackPointerDec(), upperByte);
    WriteByte(cpu->GetCpuCore().GetStackPointerDec(), lowerByte);
}

uint8_t MemoryBus::ReadByte(const Register reg)
{
    return (ReadByte(reg.Value()));
}

uint8_t MemoryBus::ReadByte(const uint16_t address)
{
    if (address >= romAddressStart && address <= romAddressEnd)
    {
        return cpu->GetCartridge().ReadByte(address);
    }
    else if (address >= romBankAddressStart && address <= romBankAddressEnd)
    {
        return cpu->GetCartridge().ReadByte(address);
    }
    else if (address >= vRamAddressStart && address <= vRamAddressEnd)
    {
        return cpu->GetPPU().ReadByte(address);
    }
    else if (address >= externalRamAddressStart && address <= externalRamAddressEnd)
    {
        return cpu->GetCartridge().ReadByte(address);
    }
    else if (address >= wRamAddressStart && address <= wRamAddressEnd)
    {
        return (workRam[0][address - wRamAddressStart]);
    }
    else if (address >= wRamBankAddressStart && address <= wRamBankAddressEnd)
    {
        if (cpu->GetCpuCore().GetCgbMode() == true)
        {
            // return (workRam[GetWorkRamBank()][address - wRamAddressStart]);
            assert(false && "CGB mode not yet supported");
        }
        else
        {
            return (workRam[1][address - wRamBankAddressStart]);
        }
    }
    else if (address >= echoRamAddressStart && address <= echoRamAddressEnd)
    {
        // This memory area is a mirror of the range 0xC000 – 0xDDFF so to access it
        // we subtract an offset and call the memory bus again.
        //
        // Nintendo has prohibited the use of this area.
        LOG_DEBUG(MEMBUS_READ_FROM_ECHO);
        return this->ReadByte(address - EchoRamOffset);
    }
    else if (address >= oamAddressStart && address <= oamAddressEnd)
    {
        return cpu->GetPPU().ReadByte(address);
    }
    else if (address >= prohibitedAddressStart && address <= prohibitedAddressEnd)
    {
        LOG_WARNING(MEMBUS_READ_FROM_PROHIBITED_AREA);
        return OpenBusValue;
    }
    else if (address >= ioAddressStart && address <= ioAddressEnd)
    {
        return ReadIO(address);
    }
    else if (address >= hRamAddressStart && address <= hRamAddressEnd)
    {
        return cpu->GetHighRam()[address - hRamAddressStart];
    }
    else if (address == interruptAddress)
    {
        return cpu->GetCpuCore().GetInterruptRegister();
    }
    else
    {
        LOG_WARNING(MEMBUS_READ_OUT_OF_RANGE);
    }
    return OpenBusValue;
}

void MemoryBus::WriteByte(const Register reg, const uint8_t value)
{
    WriteByte(reg.Value(), value);
}

void MemoryBus::WriteByte(const uint16_t address, const uint8_t value)
{
    if (address >= romAddressStart && address <= romAddressEnd)
    {
        cpu->GetCartridge().WriteByte(address, value);
    }
    else if (address >= romBankAddressStart && address <= romBankAddressEnd)
    {
        cpu->GetCartridge().WriteByte(address, value);
    }
    else if (address >= vRamAddressStart && address <= vRamAddressEnd)
    {
        cpu->GetPPU().WriteByte(address, value);
    }
    else if (address >= externalRamAddressStart && address <= externalRamAddressEnd)
    {
        cpu->GetCartridge().WriteByte(address, value);
    }
    else if (address >= wRamAddressStart && address <= wRamAddressEnd)
    {
        workRam[0][address - wRamAddressStart] = value;
    }
    else if (address >= wRamBankAddressStart && address <= wRamBankAddressEnd)
    {
        if (cpu->GetCpuCore().GetCgbMode() == true)
        {
            // workRam[GetWorkRamBank()][wRamAddress] = value;
            assert(false && "CGB mode not yet supported");
        }
        else
        {
            workRam[1][address - wRamBankAddressStart] = value;
        }
    }
    else if (address >= echoRamAddressStart && address <= echoRamAddressEnd)
    {
        // This memory area is a mirror of the range 0xC000 – 0xDDFF so to access it
        // we subtract an offset and call the memory bus again.
        //
        // Nintendo has prohibited the use of this area.
        LOG_DEBUG(MEMBUS_WRITE_TO_ECHO);
        this->WriteByte(address - EchoRamOffset, value);
    }
    else if (address >= oamAddressStart && address <= oamAddressEnd)
    {
        cpu->GetPPU().WriteByte(address, value);
    }
    else if (address >= prohibitedAddressStart && address <= prohibitedAddressEnd)
    {
        LOG_WARNING(MEMBUS_WRITE_TO_PROHIBITED_AREA);
    }
    else if (address >= ioAddressStart && address <= ioAddressEnd)
    {
        WriteIO(address, value);
    }
    else if (address >= hRamAddressStart && address <= hRamAddressEnd)
    {
        cpu->GetHighRam()[address - hRamAddressStart] = value;
    }
    else if (address == interruptAddress)
    {
        cpu->GetCpuCore().SetInterruptRegister(value);
    }
    else if (address >= wRamAddressStart && address <= wRamAddressEnd)
    {
        workRam[0][address - wRamAddressStart] = value;
    }
    else if (address >= wRamBankAddressStart && address <= wRamBankAddressEnd)
    {
        if (cpu->GetCpuCore().GetCgbMode() == true)
        {
            // workRam[GetWorkRamBank()][wRamAddress] = value;
            assert(false && "CGB mode not yet supported");
        }
        else
        {
            workRam[1][address - wRamBankAddressStart] = value;
        }
    }
    else
    {
        LOG_WARNING(MEMBUS_WRITE_OUT_OF_RANGE);
    }
}

}
