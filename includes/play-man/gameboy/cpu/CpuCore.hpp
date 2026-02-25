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

#include <play-man/gameboy/cpu/CpuCoreDefines.hpp>
#include <play-man/gameboy/cpu/Register.hpp>
#include <play-man/utility/EnumMacro.hpp>
#include <iostream>
#include <stdint.h>

namespace TestFixtures
{
	struct GameBoyCpuFixture;
}

namespace GameBoy
{
    class CpuCore
    {
        friend class Cpu;
		friend struct TestFixtures::GameBoyCpuFixture;

        // TODO: Set default values on construction
        // https://gbdev.io/pandocs/Power_Up_Sequence.html#power-up-sequence
        Register	AF; /* Accumulator & flags */
        Register	BC; /* */
        Register	DE; /* */
        Register	HL; /* */
        Register	SP = StackPointerAfterStartup; /* Stack pointer */
        Register	PC = ProgramCounterAfterBootRom; /* Program counter */
        uint8_t		IE; /* Interrupt Enable Register*/
        
        uint8_t         IF = 0x00;  /* Interrupt Request Flag Register */
        bool            IME = false; /* Interrupt Master Enable */
        InterruptState  stateIME = InterruptState::NONE; /* Helper variable to update the IME at the correct time */

        /**
         * @brief The JoyPad register.
         * 
         * Only the lower 6 bits of this register are used, for 8 different buttons:
         * 
         * Bits:      5      |      4      |      3      |      2      |      1      |      0      |    
         *         Select    |    Select   |    Start\   |    Select\  |      B\     |      A\     |
         *         Buttons   |    D-Pad    |    Down     |    Pp       |     Left    |     Right   |
         * 
         * Bits 5 & 4 are set to 0 to select which set of buttons are pressed.
         * A pressed button is read as 0 rather than 1. 
         * 
         * @note The lower nibble is Read Only.
         */
        uint8_t     P1 = JoyPadNoButtonsPressed;  

        /**
         * @brief Whether the emulator is set to DMG or CGB mode.
         * 
         * Set to true if CGB mode is enabled.
         */
        bool        cgbMode = false;

        /**
         * @brief Sets all registers to the value 0x00'00
         */
        void ClearRegisters();

    public:
        /**
         * @brief Retrieves the value inside the interrupt enable (IE) register.
         */
        uint8_t GetInterruptEnableRegisterValue();
        
        /**
         * @brief Retrieves the value inside the interrupt request (IF) register.
         */
        uint8_t GetInterruptRequestRegisterValue();

        /**
         * @brief Retrieves the value of the JoyPad (P1) register.
         */
        uint8_t GetJoyPadRegisterValue();

        /**
         * @brief Sets the interrupt enable (IE) register.
         */
        void    SetInterruptEnableRegister(const uint8_t value);

        /**
         * @brief Sets the interrupt request (IF) register.
         */
        void    SetInterruptRequestRegister(const uint8_t value);

        /**
         * @brief Sets the JoyPad (P1) register.
         * 
         * @note The lower nibble of this register should be Read Only,
         * this is not enforced by this function so internal writes can be
         * done using this function as well.
         */
        void    SetJoyPadRegister(const uint8_t value);

        /**
         * @brief Used to set a bit inside the flag register
         * @param flag The flag to be changed
         * @param enable Enable or disable the targeted flag
         */
        void SetFlag(FlagRegisterFlag flag, bool enable);

        /**
         * @brief Used to retrieve the status of a flag inside teh flag register.
         * @param flag The flag to be returned.
         * @return true/false depending if the flag is set or not.
         */
        bool GetFlag(FlagRegisterFlag flag);

        /**
         * @brief Returns the state of the emulator, DMG or CGB mode.
         * 
         * @return True if set to CGB mode, false for DMG.
         */
        bool GetCgbMode() const;

        /**
         * @brief Returns the value inside the stack pointer, after which it gets incremented. 
         */
        uint16_t GetStackPointerInc();

        /**
         * @brief Decrements the value inside the stack pointer, after which it gets returned.
         */
        uint16_t GetStackPointerDec();

        friend std::ostream& operator << (std::ostream& lhs, const CpuCore& rom);
    };

    /**
     *  @brief Pointer to a full register (16 bits) within a CpuCore.
     */
    using RegisterPointer = Register CpuCore::*;

} /* namespace Gameboy */
