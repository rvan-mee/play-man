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
#include <play-man/gameboy/memory/MemoryDefines.hpp>
#include <play-man/logger/Logger.hpp>
#include <assert.h>

namespace GameBoy {

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
        // TODO: check how bank switching works
        assert(false && "Rom bank switching is currently not supported");
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
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else if (address >= wRamBankAddressStart && address <= wRamBankAddressEnd)
    {
        assert(false && "Fetching from this memory address is not supported yet!");
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
        assert(false && "Fetching from this memory address is not supported yet!");
    }
    else
    {
        LOG_FATAL("Trying to read from an invalid address");
        assert(false);
        return (-1);
    }
    return (-1);
}

}
