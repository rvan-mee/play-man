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

    #define COUNTER_CLOCK_SPEEDS_SEQ(x, n) \
        x(n, MCYCLES_256, 0b0000'0000)  \
        x(n, MCYCLES_4,   0b0000'0001)  \
        x(n, MCYCLES_16,  0b0000'0010)  \
        x(n, MCYCLES_64,  0b0000'0011)

    CREATE_ENUM_WITH_UTILS(COUNTER_CLOCK_SPEEDS_SEQ, CounterClockSpeeds)
    #undef COUNTER_CLOCK_SPEEDS_SEQ

    #define COUNTER_STATE_SEQ(x, n) \
        x(n, ENABLED,  0b0000'0100)  \
        x(n, DISABLED, 0b0000'0000)

    CREATE_ENUM_WITH_UTILS(COUNTER_STATE_SEQ, CounterState)
    #undef COUNTER_STATE_SEQ

    constexpr uint8_t CounterEnableBit = 0b0000'0100;
    constexpr uint8_t CounterClockBits = 0b0000'0011;

    constexpr uint16_t DividerRegisterAddress = 0xFF04;
    constexpr uint16_t CounterRegisterAddress = 0xFF05;
    constexpr uint16_t ModuloRegisterAddress  = 0xFF06;
    constexpr uint16_t ControlRegisterAddress = 0xFF07;

    constexpr uint8_t DividerDefaultValue = 0x00;
    constexpr uint8_t CounterDefaultValue = 0x00;
    constexpr uint8_t ModuloDefaultValue  = 0x00;
    constexpr uint8_t ControlDefaultValue = 0x00;
}
