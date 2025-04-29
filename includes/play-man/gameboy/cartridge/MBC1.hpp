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

class MBC1Cartridge : public ACartridge
{
private:
    /* Control Registers*/

    /**
     * @brief Ram is disabled on startup, it can be enabled by writing 0x0A to
     * the address space used to enable it. Any value with $A in the lower 4 bits
     * enables the RAM, any other value disables it.
     */
    bool    ramEnabled;

    /**
     * @brief A 5 bit register (write only) that is used to select the ROM bank number,
     * writing 0xE1 (binary 11100001) to this register would select bank 0x01.
     * 
     * Additionally if the number is set to a higher value then the amount of available
     * banks it will be masked to the required number of bits. e.g. a 256 KiB cart only
     * needs a 4-bit bank number to address all of its 16 banks, so this register
     * is masked to 4 bits. Ignoring the top bits.
     * 
     * On larger carts which need a >5 bit bank number,
     * the secondary banking register at 4000–5FFF is used to supply an additional 2 bits.
     * 
     * @note For MBC1M cartridges this register is instead 4 bits long.
     */
    uint8_t    selectedBankRegister;

    /**
     * @brief Depending on the selected banking mode this register is used to switch between
     * the RAM banks in the range 0x00 to 0x03 or it is used to extend the ROM bank selection
     * for carts that are 1MiB or larger. If neither the ROM or the RAM is large enough,
     * setting these bits will result in nothing.
     */
    uint8_t    secondarySelectedBankRegister;

    /**
     * @brief A 1 bit register that selects the banking mode used for the secondary selected bank register.
     * If the cart is not large enough to use the 2-bit register (≤ 8 KiB RAM and ≤ 512 KiB ROM)
     * this mode select has no observable effect.
     * 
     * The two possible modes are:
     * 
     * - simple: ranges 0x0000–0x3FFF and 0xA000–0xBFFF are locked to bank 0 of ROM and RAM respectively.
     * - advanced: ranges 0x0000–0x3FFF and 0xA000-0xBFFF can be bank-switched via the secondary register.
     */
    uint8_t  bankingModeSelect;

    /**
     * @brief MBC1M carts, otherwise known as Multi-Game compilation carts
     * contain multiple games on the same cartridge. To accomplish this without
     * chaning their code, these cards have alternative wiring compared to a regular MBC1.
     * 
     * A MBC1M cart can be identified by a second nintendo copyright header in the 0x10 rom bank.
     */
    bool    cartIsMBC1M;

    /**
     * @brief The amount of bits used to represent the size of the selectedBankRegister.
     * 
     * @note For regular MBC1 cartridges the register is 5 bits long, however MBC1M cartridges
     * are wired differently, making the register only 4 bits long.
     */
    uint8_t bankRegisterBitCount;

    /**
     * @brief Returns the mask that is used to get the right selected rom bank.
     */
    uint8_t RomBankMask();

    /**
     * @brief Checks if the current MBC1 cartridge contains a second nintendo logo,
     * signifying that it is a MBC1M cartridge.
     */
    bool CheckMBC1M();

public:
    MBC1Cartridge() = delete;
    MBC1Cartridge(std::unique_ptr<Rom> rom); 
    ~MBC1Cartridge() = default;

    virtual uint8_t ReadByte(const uint16_t address) override;
    virtual void    WriteByte(const uint16_t address, const uint8_t value) override;
};

}
