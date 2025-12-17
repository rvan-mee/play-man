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
		x(n, ADD_SUB,    0b01000000)   \
		x(n, HALF_CARRY, 0b00100000)   \
		x(n, CARRY,      0b00010000)

	CREATE_ENUM_WITH_UTILS(F_REGISTER_FLAGS_SEQ, FlagRegisterFlag)
	#undef F_REGISTER_FLAGS_SEQ

	constexpr uint16_t programCounterAfterBootRom = 0x0100;

    class CpuCore
    {
        friend class Cpu;
		friend struct TestFixtures::GameBoyCpuFixture;

        // TODO: Set default values on construction
        Register	AF; /* Accumulator & flags */
        Register	BC; /* */
        Register	DE; /* */
        Register	HL; /* */
        Register	SP; /* Stack pointer */
        Register	PC = programCounterAfterBootRom; /* Program counter */
        uint8_t		IE; /* Interrupt Enable Register*/

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
         * @brief -.
         */
        void    SetInterruptRegister(const uint8_t value);

        /**
         * @brief Used to set a bit inside the flag register
         * @param flag The flag to be changed
         * @param enable Enable or disable the targeted flag
         */
        void SetFlag(FlagRegisterFlag flag, bool enable);

        friend std::ostream& operator << (std::ostream& lhs, const CpuCore& rom);
    };

} /* namespace Gameboy */
