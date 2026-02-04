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

#include <play-man/gameboy/memory/MemoryBus.hpp>
#include <play-man/gameboy/memory/MemoryBusDefines.hpp>
#include <play-man/logger/Logger.hpp>
#include <assert.h>

namespace GameBoy {

uint16_t MemoryBus::PopStack()
{
    uint8_t lowerByte;
    uint8_t upperByte;

    lowerByte = ReadByte(core.GetStackPointerInc());
    upperByte = ReadByte(core.GetStackPointerInc());

    return ((upperByte << 8) | lowerByte);
}

void MemoryBus::PushStack(uint16_t value)
{
    const uint8_t lowerByte = value & 0xFF;
    const uint8_t upperByte = (value & 0xFF00) >> 8;

    WriteByte(core.GetStackPointerDec(), upperByte);
    WriteByte(core.GetStackPointerDec(), lowerByte);
}

uint8_t MemoryBus::ReadByte(const Register reg)
{
    return (ReadByte(reg.Value()));
}

uint8_t MemoryBus::ReadByte(const uint16_t address)
{
    if (address >= romAddressStart && address <= romAddressEnd)
    {
        return (cartridge->ReadByte(address));
    }
    else if (address >= romBankAddressStart && address <= romBankAddressEnd)
    {
        return (cartridge->ReadByte(address));
    }
    else if (address >= vRamAddressStart && address <= vRamAddressEnd)
    {
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else if (address >= externalRamAddressStart && address <= externalRamAddressEnd)
    {
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else if (address >= wRamAddressStart && address <= wRamAddressEnd)
    {
        return (workRam[0][address - wRamAddressStart]);
    }
    else if (address >= wRamBankAddressStart && address <= wRamBankAddressEnd)
    {
        if (core.GetCgbMode() == true)
        {
            // workRam[GetWorkRamBank()][wRamAddress] = value;
            assert(false && "CGB mode not yet supported");
        }
        else
        {
            return (workRam[1][address - wRamAddressStart]);
        }
    }
    else if (address >= echoRamAddressStart && address <= echoRamAddressEnd)
    {
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else if (address >= oamAddressStart && address <= oamAddressEnd)
    {
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else if (address >= oamAddressStart && address <= oamAddressEnd)
    {
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else if (address >= prohibitedAddressStart && address <= prohibitedAddressEnd)
    {
        LOG_WARNING("Fetching memory from a prohibited address");
    }
    else if (address >= ioAddressStart && address <= ioAddressEnd)
    {
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else if (address >= hRamAddressStart && address <= hRamAddressEnd)
    {
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else if (address == interruptAddress)
    {
        return (core.GetInterruptRegister());
    }
    else
    {
        LOG_FATAL("Trying to read from an invalid address");
        assert(false);
        return (-1);
    }
    return (-1);
}

void MemoryBus::WriteByte(const Register reg, const uint8_t value)
{
    WriteByte(reg.Value(), value);
}

void MemoryBus::WriteByte(const uint16_t address, const uint8_t value)
{
    if (address >= romAddressStart && address <= romAddressEnd)
    {
        return (cartridge->WriteByte(address, value));
    }
    else if (address >= romBankAddressStart && address <= romBankAddressEnd)
    {
        return (cartridge->WriteByte(address, value));
    }
    else if (address >= vRamAddressStart && address <= vRamAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= externalRamAddressStart && address <= externalRamAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= wRamAddressStart && address <= wRamAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= wRamBankAddressStart && address <= wRamBankAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= echoRamAddressStart && address <= echoRamAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= oamAddressStart && address <= oamAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= oamAddressStart && address <= oamAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= prohibitedAddressStart && address <= prohibitedAddressEnd)
    {
        LOG_WARNING("Fetching memory from a prohibited address");
    }
    else if (address >= ioAddressStart && address <= ioAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= hRamAddressStart && address <= hRamAddressEnd)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address == interruptAddress)
    {
        assert(false && "Writing to this memory address is not supported yet!");
    }
    else if (address >= wRamAddressStart && address <= wRamAddressEnd)
    {
        workRam[0][address - wRamAddressStart] = value;
    }
    else if (address >= wRamBankAddressStart && address <= wRamBankAddressEnd)
    {
        if (core.GetCgbMode() == true)
        {
            // workRam[GetWorkRamBank()][wRamAddress] = value;
            assert(false && "CGB mode not yet supported");
        }
        else
        {
            workRam[1][address - wRamAddressStart] = value;
        }
    }
    else
    {
        LOG_FATAL("Trying to read from an invalid address");
        assert(false);
    }
}

}
