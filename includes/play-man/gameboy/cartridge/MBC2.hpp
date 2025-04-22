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
#include <play-man/gameboy/cartridge/CartridgeDefines.hpp>

namespace GameBoy {

class MBC2Cartridge : public ACartridge
{
private:
    /* Control Registers */

    /**
     * @brief The MCB2 Cartridge does not contain external RAM,
     * rather is has 512 half-bytes of ram baked into the MCB2 chip.
     * 
     * @note the upper 4 bits of this RAM is undefined and should not
     * be relied upon.
     */
    std::array<uint8_t, MCB2RamSize>    ram;

    /**
     * @brief Bit 8 (least significant bit of the upper address byte) controls
     * if his register is being written to or the ramEnabled register.
     * 
     * @note 
     */
    uint8_t romBankNumber;

    /**
     * @brief Bit 8 (least significant bit of the upper address byte) controls
     * if his register is being written to or the romBankNumber register.
     * 
     * 
     */
    bool ramEnabled;

public:
    MBC2Cartridge() = delete;
    MBC2Cartridge(std::unique_ptr<Rom> rom);
    ~MBC2Cartridge() = default;

    virtual uint8_t ReadByte(const uint16_t address) override;
    virtual void    WriteByte(const uint16_t address, const uint8_t value) override;
};

}
