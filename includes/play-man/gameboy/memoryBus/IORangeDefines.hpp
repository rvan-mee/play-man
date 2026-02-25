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

constexpr uint16_t JoyPadInputAddress = 0xFF00;

constexpr uint16_t SerialTransferAddressStart = 0xFF01;
constexpr uint16_t SerialTransferAddressEnd = 0xFF02;

constexpr uint16_t TimerAndDividerAddressStart = 0xFF04;
constexpr uint16_t TimerAndDividerAddressEnd = 0xFF07;

constexpr uint16_t InterruptRequestAddress = 0xFF0F;

constexpr uint16_t AudioAddressStart = 0xFF10;
constexpr uint16_t AudioAddressEnd = 0xFF26;

constexpr uint16_t AudioWavePatternAddressStart = 0xFF30;
constexpr uint16_t AudioWavePatternAddressEnd = 0xFF3F;

constexpr uint16_t PPURegistersAddressStart = 0xFF40;
constexpr uint16_t PPURegistersAddressEnd = 0xFF4B;

constexpr uint16_t BootRomMappingControlAddress = 0xFF50;

/* Following address ranges are CGB Only */

constexpr uint16_t DMGCompatibilityModeAddress = 0xFF4C;

constexpr uint16_t SpeedSwitchRegisterAddress = 0xFF4D;

constexpr uint16_t VRamBankSelectAddress = 0xFF4F;

constexpr uint16_t VRamDMAAddressStart = 0xFF51;
constexpr uint16_t VRamDMAAddressEnd = 0xFF55;

constexpr uint16_t IRPortAddress = 0xFF56;

constexpr uint16_t PPUPalettesAddressStart = 0xFF68;
constexpr uint16_t PPUPalettesAddressEnd = 0xFF6B;

constexpr uint16_t ObjectPriorityModeAddress = 0xFF6C;

constexpr uint16_t WorkRamBankSelect = 0xFF70;

#define MEMBUS_WRITE_OUT_OF_RANGE "MemoryBus: Trying to perform a write on an invalid address"
#define MEMBUS_READ_OUT_OF_RANGE "MemoryBus: Trying to perform a read on an invalid address"
