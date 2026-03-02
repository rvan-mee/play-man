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
#include <stdint.h>

namespace GameBoy {

    #define SERIAL_TRANSFER_CONTROL_FLAGS_SEQ(x, n) \
        x(n, TRANSFER_ENABLE, 0b1000'0000)  \
        x(n, CLOCK_SPEED,     0b0000'0010)  \
        x(n, CLOCK_SELECT,    0b0000'0001)

    CREATE_ENUM_WITH_UTILS(SERIAL_TRANSFER_CONTROL_FLAGS_SEQ, SerialTransferControlFlag)
    #undef SERIAL_TRANSFER_CONTROL_FLAGS_SEQ

    class SerialTransfer
    {
    private:

        /**
         * @brief The Serial Transfer Control register.
         * 
         * The bits represent the following:
         * - Bit 7 : Transfer enable : 1 = Requested or in progress
         * - Bit 1 : Clock speed (CGB only) : 1 = enable high speed serial clock (~256 kHz in normal-speed mode)
         * - Bit 0 : Clock select : 0 = external clock | 1 = internal clock
         */
        uint8_t SC;

        /**
         * @brief 
         */
        uint8_t serialData;

        // TODO:
        // const bool& cgbMode;

    public:
        SerialTransfer();
        // SerialTransfer(const bool& _cgbMode);
        ~SerialTransfer();

        // TODO:
        // void TickSerialTransfer();

        void WriteByte(const uint16_t address, uint8_t value);

        uint8_t ReadByte(const uint16_t address);
    };

}
