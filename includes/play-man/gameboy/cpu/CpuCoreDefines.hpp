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

namespace GameBoy
{
    /**
     * @brief The bits inside the lower nibble of the JoyPad register (P1) and what they
     * represent if the Action bit is cleared.
     */
    #define JOYPAD_ACTION_BUTTONS_SEQ(x, n) \
        x(n, START,    0b0000'1000)  \
        x(n, SELECT,   0b0000'0100)  \
        x(n, B,        0b0000'0010)  \
        x(n, A,        0b0000'0001)

    CREATE_ENUM_WITH_UTILS(JOYPAD_ACTION_BUTTONS_SEQ, JoyPadActionButtons)
    #undef JOYPAD_ACTION_BUTTONS_SEQ

    /**
     * @brief The bits inside the lower nibble of the JoyPad register (P1) and what they
     * represent if the Action bit is cleared.
     */
    #define JOYPAD_DIRECTION_BUTTONS_SEQ(x, n) \
        x(n, DOWN,  0b0000'1000)  \
        x(n, UP,    0b0000'0100)  \
        x(n, LEFT,  0b0000'0010)  \
        x(n, RIGHT, 0b0000'0001)

    CREATE_ENUM_WITH_UTILS(JOYPAD_DIRECTION_BUTTONS_SEQ, JoyPadDirectionButtons)
    #undef JOYPAD_DIRECTION_BUTTONS_SEQ

    /**
     * @brief The bits inside the P1 register that decide which group of buttons are pressed.
     */
    #define JOYPAD_BUTTON_GROUP_SEQ(x, n) \
        x(n, ACTION,    0b0010'0000)    \
        x(n, DIRECTION, 0b0001'0000)

    CREATE_ENUM_WITH_UTILS(JOYPAD_BUTTON_GROUP_SEQ, JoyPadButtonGroup)
    #undef JOYPAD_BUTTON_GROUP_SEQ

    template<typename EnumTypeT>
    /**
     * @brief Helper function to print state changes in the JoyPad (P1) register.
     * 
     * @param ss String stream where the output will be sent to.
     * @param enumName The button/bit to check for.
     * @param value The value being set.
     * @param P1 The state of the JoyPad register before the value change.
     * 
     * @note A pressed button will read as 0 instead of 1.
     */
    void AddButtonChangeToStringStream(std::stringstream& ss, EnumTypeT enumName, const uint8_t value, const uint8_t P1)
    {
        const uint8_t enumVal = GetEnumAsValue(enumName);
        if ((value & enumVal) != (P1 & enumVal))
        {
            ss << "\t\t" << enumName << " ";
            if ((value & enumVal) != 0)
                ss << "unpressed\n";
            else
                ss << "pressed\n";
        }
    }

    /**
     * @brief When the interrupts get enabled through the EI instruction
     *        it takes an additional instruction fot the effect to take place.
     *        This enum is there to keep track of the state of the IME flag after
     *        an EI instruction is called.
     */
    #define INTERRUPT_STATE_SEQ(x, n) \
        x(n, NONE,             0)   \
        x(n, DELAY_ENABLE_IME, 1)   \
        x(n, ENABLE_IME,       2)

    CREATE_ENUM_WITH_UTILS(INTERRUPT_STATE_SEQ, InterruptState)
    #undef INTERRUPT_STATE_SEQ

    /**
     * @brief the flags to set the bits inside the F register
     */
    #define INTERRUPT_FLAGS_SEQ(x, n) \
        x(n, VBLANK, 0b0000'0001)   \
        x(n, LCD,    0b0000'0010)   \
        x(n, TIMER,  0b0000'0100)   \
        x(n, SERIAL, 0b0000'1000)   \
        x(n, JOYPAD, 0b0001'0000)

    CREATE_ENUM_WITH_UTILS(INTERRUPT_FLAGS_SEQ, InterruptFlags)
    #undef INTERRUPT_FLAGS_SEQ

    /**
     * @brief the flags to set the bits inside the F register
     */
    #define F_REGISTER_FLAGS_SEQ(x, n) \
        x(n, ZERO,       0b10000000)   \
        x(n, SUB,        0b01000000)   \
        x(n, HALF_CARRY, 0b00100000)   \
        x(n, CARRY,      0b00010000)

    CREATE_ENUM_WITH_UTILS(F_REGISTER_FLAGS_SEQ, FlagRegisterFlag)
    #undef F_REGISTER_FLAGS_SEQ

    constexpr uint16_t ProgramCounterAfterBootRom = 0x0100;
    constexpr uint16_t StackPointerAfterStartup = 0xFFFE; // HRAM end address
    constexpr uint8_t  JoyPadNoButtonsPressed = 0x3F;
}
