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

// Address inside the ROM, usually a fixed bank
constexpr uint16_t romAddressStart = 0x0000;
constexpr uint16_t romAddressEnd = 0x3FFF;

// Addresses inside the ROM, from a switchable bank
constexpr uint16_t romBankAddressStart = 0x4000;
constexpr uint16_t romBankAddressEnd = 0x7FFF;

// Addresses for the video RAM
constexpr uint16_t vRamAddressStart = 0x8000;
constexpr uint16_t vRamAddressEnd = 0x9FFF;

// Addresses for the external RAM
constexpr uint16_t externalRamAddressStart = 0xA000;
constexpr uint16_t externalRamAddressEnd = 0xBFFF;

// Addresses for the internal work RAM
constexpr uint16_t wRamAddressStart = 0xC000;
constexpr uint16_t wRamAddressEnd = 0xCFFF;

// Addresses for the internal work RAM, in CBG mode a switchable bank
constexpr uint16_t wRamBankAddressStart = 0xD000;
constexpr uint16_t wRamBankAddressEnd = 0xDFFF;

// Addresses for the echo RAM, a prohibited area and a mirror of both the wRam addresses
constexpr uint16_t echoRamAddressStart = 0xE000;
constexpr uint16_t echoRamAddressEnd = 0xFDFF;

// Addresses for the Object Attribute Memory (OAM)
constexpr uint16_t oamAddressStart = 0xFE00;
constexpr uint16_t oamAddressEnd = 0xFE9F;

// Addresses located inside a not usable and prohibited area
constexpr uint16_t prohibitedAddressStart = 0xFEA0;
constexpr uint16_t prohibitedAddressEnd = 0xFEFF;

// Addresses for the IO registers
constexpr uint16_t ioAddressStart = 0xFF00;
constexpr uint16_t ioAddressEnd = 0xFF00;

// Addresses for the high RAM
constexpr uint16_t hRamAddressStart = 0xFF80;
constexpr uint16_t hRamAddressEnd = 0xFFFE;

// Address for the interrupt register 
constexpr uint16_t interruptAddress = 0xFFFF;

// The offset required to reach the right section of memory.
constexpr uint16_t EchoRamOffset = 0x2000;

#define MEMBUS_READ_FROM_PROHIBITED_AREA "MemoryBus: Trying to read from a prohibited address"
#define MEMBUS_WRITE_TO_PROHIBITED_AREA "MemoryBus: Trying to write to a prohibited address"

#define MEMBUS_READ_FROM_ECHO "MemoryBus: Trying to read from the echo ram section, this is prohibited by Nintendo"
#define MEMBUS_WRITE_TO_ECHO "MemoryBus: Trying to write to the echo ram section, this is prohibited by Nintendo"

#define MEMBUS_WRITE_OUT_OF_RANGE "MemoryBus: Trying to perform a write on an invalid address"
#define MEMBUS_READ_OUT_OF_RANGE "MemoryBus: Trying to perform a read on an invalid address"
