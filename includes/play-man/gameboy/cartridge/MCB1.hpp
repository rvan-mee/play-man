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

#include <play-man/gameboy/cartridge/ACartridge.hpp>
#include <play-man/gameboy/cartridge/Rom.hpp>
// #include <>

namespace GameBoy {
    
class MCB1Cartridge : public ACartridge
{
private:
    /* Banks */
    MemoryBanks ramBanks;
    MemoryBanks romBanks;

    /* Control Registers*/
    bool        ramEnabled;
    uint8_t     selectedRamBank;
    uint8_t     selectedRomBank;
    uint8_t     secondarySelectedBankBits;

    void    InitRomBanks();
    void    InitRamBanks();

public:
    MCB1Cartridge() = delete;
    MCB1Cartridge(std::unique_ptr<Rom> rom); 
    ~MCB1Cartridge() = default;

    virtual uint8_t ReadByte(const uint16_t address) override;
    virtual void    WriteByte(const uint16_t address, const uint8_t value) override;
};

}
