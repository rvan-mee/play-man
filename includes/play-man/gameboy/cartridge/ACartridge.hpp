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

#include <play-man/gameboy/cartridge/CartridgeDefines.hpp>
#include <play-man/gameboy/cartridge/Rom.hpp>
#include <memory>
#include <stdint.h>

namespace GameBoy {

class ACartridge
{
private:
    void    InitRamBanks();

protected:
    std::unique_ptr<Rom>    rom;
    MemoryBanks             ramBanks;

public:
    ACartridge() = delete;
    ACartridge(std::unique_ptr<Rom> _rom);
    virtual ~ACartridge() = default;

    virtual uint8_t ReadByte(const uint16_t address) = 0;
    virtual void    WriteByte(const uint16_t address, const uint8_t value) = 0;

    CartridgeType   GetType() const;
    uint32_t        GetRamBankCount() const;
    uint32_t        GetRomBankCount() const;

};

}
