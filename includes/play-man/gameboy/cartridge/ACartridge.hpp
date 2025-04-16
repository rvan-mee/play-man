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

#include <play-man/gameboy/cartridge/Rom.hpp>
#include <memory>
#include <stdint.h>

namespace GameBoy {

using MemoryBanks = std::vector<std::vector<uint8_t>>;
constexpr uint32_t KiB = 1024;
constexpr uint32_t RamBankSize = KiB * 8;
constexpr uint32_t RomBankSize = KiB * 16;

/**
 * @note Banking: Banking is a way to use the same address space for much more data
 *       then it should be able to fit. A cartridge contains registers that keep track of
 *       which bank is activated. Changing the activated bank will change where
 *       the address will point to, allowing for a lot more memory to be addressed.
 *       
 *       Since ROMs are read only (as the name suggest) trying to write to the address space
 *       connected to it seem counter productive but it will not attempt to write
 *       into ROM memory, instead it will change register values inside the cartridge.
 *       This allows games to change which bank is activated.
 */
class ACartridge
{
protected:
    std::unique_ptr<Rom> rom;

public:
    ACartridge() = delete;
    ACartridge(std::unique_ptr<Rom> _rom) : rom(std::move(_rom)) {};
    virtual ~ACartridge() = default;

    virtual uint8_t ReadByte(const uint16_t address) = 0;
    virtual void    WriteByte(const uint16_t address, const uint8_t value) = 0;

    CartridgeType   GetType() { return rom->GetCartridgeType(); }

};

}
