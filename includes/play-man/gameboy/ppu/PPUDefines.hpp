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

#include <play-man/utility/EnumMacro.hpp>

namespace GameBoy {

#define PpuStateSeq(x, n) \
    x(n, hBlank, 0)       \
    x(n, vBlank, 1)       \
    x(n, ScanOAM, 2)      \
    x(n, Drawing, 3)      \

CREATE_ENUM_WITH_UTILS(PpuStateSeq, PixelProcessingState);
#undef PpuStateSeq

// These are the default values of the registers after the boot rom has ran it course.

constexpr uint8_t DefaultDMAregisterValue = 0x00;
constexpr uint8_t DefaultLCDCregisterValue = 0x00;
constexpr uint8_t DefaultLYregisterValue = 0x00;
constexpr uint8_t DefaultLYCregisterValue = 0x00;
constexpr uint8_t DefaultSTATregisterValue = 0x00;
constexpr uint8_t DefaultSCYregisterValue = 0x00;
constexpr uint8_t DefaultSCXregisterValue = 0x00;
constexpr uint8_t DefaultWYregisterValue = 0x00;
constexpr uint8_t DefaultWXregisterValue = 0x00;
constexpr uint8_t DefaultBGPregisterValue = 0x00;
constexpr uint8_t DefaultOBP0registerValue = 0x00;
constexpr uint8_t DefaultOBP1registerValue = 0x00;
constexpr uint8_t DefaultBGPIregisterValue = 0x00;
constexpr uint8_t DefaultBCPDregisterValue = 0x00;
constexpr uint8_t DefaultOBPIregisterValue = 0x00;
constexpr uint8_t DefaultOBPDregisterValue = 0x00;

constexpr PixelProcessingState DefaultStateValue = PixelProcessingState::ScanOAM;

// The address ranges for addressing the PPU registers

constexpr uint16_t AddressLCDC = 0xFF40;
constexpr uint16_t AddressSTAT = 0xFF41;
constexpr uint16_t AddressSCY = 0xFF42;
constexpr uint16_t AddressSCX = 0xFF43;
constexpr uint16_t AddressLY = 0xFF44; // Read only.
constexpr uint16_t AddressLYC = 0xFF45;
constexpr uint16_t AddressDMA = 0xFF46;
constexpr uint16_t AddressBGP = 0xFF47;
constexpr uint16_t AddressOBP0 = 0xFF48;
constexpr uint16_t AddressOBP1 = 0xFF49;
constexpr uint16_t AddressWY = 0xFF4A;
constexpr uint16_t AddressWX = 0xFF4B;
constexpr uint16_t AddressBGPI = 0xFF68;
constexpr uint16_t AddressBCPD = 0xFF69;
constexpr uint16_t AddressOBPI = 0xFF6A;
constexpr uint16_t AddressOBPD = 0xFF6B;

/**
 * @brief Some bits of the STAT register can only be read from,
 * this mask masks out the bits that can be written to.
 * 
 * The bits that can be changed represent (in order):
 * 
 */
constexpr uint8_t WriteMaskSTATvalue = 0b01111000;

#define PPU_READ_OUT_OF_RANGE "PPU: Trying to read from an address that is not within range"
#define PPU_READ_IN_MODE_3 "PPU: Trying to read from a register inaccessible during PPU mode 3 (drawing)"
#define PPU_READ_IN_CGB_MODE "PPU: Trying to perform a read from a register that is not accessible when the CgbMode is enabled"
#define PPU_READ_IN_NON_CGB_MODE "PPU: Trying to perform a read from a register that is not accessible when the CgbMode is disabled"

#define PPU_WRITE_OUT_OF_RANGE "PPU: Trying to write to an address that is not within range"
#define PPU_WRITE_IN_MODE_3 "PPU: Trying to write to a register inaccessible during PPU mode 3 (drawing)"
#define PPU_WRITE_IN_CGB_MODE "PPU: Trying to perform a write to a register that is not accessible when the CgbMode is enabled"
#define PPU_WRITE_IN_NON_CGB_MODE "PPU: Trying to perform a write to a register that is not accessible when the CgbMode is disabled"

}
