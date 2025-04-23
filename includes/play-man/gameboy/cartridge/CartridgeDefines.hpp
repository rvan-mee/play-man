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

#pragma once

#include <stdint.h>
#include <vector>

namespace GameBoy {

    /**
     * @note Banking: Banking is a way to use the same address space for much more data
     *       then it should be able to fit. A cartridge contains registers that keep track of
     *       which bank is activated. Changing the register will change which bank is active
     *       and where the address will point to, allowing for a lot more memory to be addressed.
     *       
     *       Since ROMs are read only (as the name suggest) trying to write to the address space
     *       connected to it seem counter productive but it will not attempt to write
     *       into ROM memory, instead it will change a register values inside the cartridge.
     */
    using MemoryBanks = std::vector<std::vector<uint8_t>>;
    constexpr uint32_t KiB = 1024;
    constexpr uint32_t RamBankSize = KiB * 8;
    constexpr uint32_t RomBankSize = KiB * 16;
    constexpr uint32_t MCB2RamSize = 512;

    // TODO: figure out the correct value: https://gbdev.io/pandocs/MBC1.html
    // mentions that 0xFF is often the value, but not guaranteed.
    constexpr uint8_t  OpenBusValue = 0xFF;

    #define RAM_BANK_INVALID "Cartridge: Trying to access an invalid RAM bank"

    #define READ_RAM_DISABLED "Cartridge: Trying to read from RAM whilst it is not enabled"
    #define WRITE_RAM_DISABLED "Cartridge: Trying to write to RAM whilst it is not enabled"

    #define READ_TIMER_DISABLED "Cartridge: Trying to read from the RTC registers whilst they are not enabled"
    #define WRITE_TIMER_DISABLED "Cartridge: Trying to write to the RTC registers whilst they are not enabled"

    #define READ_OUT_OF_RANGE "Cartridge: Trying to read from an address that is not within range"
    #define WRITE_OUT_OF_RANGE "Cartridge: Trying to write to an address that is not within range"
}
