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

    class MBC5Cartridge : public ACartridge
    {
    private:
        /**
         * @brief The lower 8 bits of the ROM bank number, selecting 00 will actually
         * result in selecting bank 0, unlike in the MBC's before this.
         */
        uint8_t romBankNumberLowerbits;

        /**
         * @brief The upper 9th bit of the ROM bank number. The MBC5 supports up to 8 MiB of ROM.
         */
        uint8_t romBankNumberUpperbit;

        /**
         * @brief The number to select the correct RAM bank.
         * On cartridges with a rumble motor the 3rd bit is used to enable the motor.
         */
        uint8_t ramBankNumber;

        /**
         * @brief Wether reading and writing to ram is enabled. 
         */
        bool    ramEnabled;

        /**
         * @brief If the current cartridge included a rumble motor.
         */
        bool    hasRumbleMotor;

        // TODO: Once SDL is implemented and controller support is added support this:
        /**
         * @brief Activates the rumble motor on a controller.
         */
        void    ActivateRumbleMotor();

        /**
         * @brief Activates the rumble motor on a controller.
         */
        void    DeactivateRumbleMotor();

    public:
        MBC5Cartridge() = delete;
        MBC5Cartridge(std::unique_ptr<Rom> rom);
        ~MBC5Cartridge() = default;

        virtual uint8_t ReadByte(const uint16_t address) override;
        virtual void    WriteByte(const uint16_t address, const uint8_t value) override;
    };

}
