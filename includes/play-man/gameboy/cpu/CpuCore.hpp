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

	constexpr uint16_t programCounterAfterBootRom = 0x0100;
    constexpr uint16_t stackPointerAfterStartup = 0xFFFE; // HRAM end address

    class CpuCore
    {
        friend class Cpu;
		friend struct TestFixtures::GameBoyCpuFixture;

        // TODO: Set default values on construction
        Register	AF; /* Accumulator & flags */
        Register	BC; /* */
        Register	DE; /* */
        Register	HL; /* */
        Register	SP = stackPointerAfterStartup; /* Stack pointer */
        Register	PC = programCounterAfterBootRom; /* Program counter */
        uint8_t		IE; /* Interrupt Enable Register*/

        /**
         * @brief Whether the emulator is set to DMG or CGB mode.
         * 
         * Set to true if CGB mode is enabled.
         */
        bool        cgbMode = false;

        // uint16_t	cyclesPassed = 0; /* */

        /**
         * @brief Sets all registers to the value 0x00'00
         */
        void ClearRegisters();

    public:
        /**
         * @brief -.
         */
        uint8_t GetInterruptRegister();

        /**
         * @brief Sets the interrupt register.
         */
        void    SetInterruptRegister(const uint8_t value);

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
