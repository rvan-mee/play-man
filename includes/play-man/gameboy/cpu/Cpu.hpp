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

#include <play-man/gameboy/memory/MemoryBus.hpp>
#include <play-man/gameboy/cpu/Register.hpp>
#include <play-man/gameboy/cpu/Opcodes.hpp>
#include <inttypes.h>

namespace GameBoy
{
	class Cpu
	{
		Register	AF; /* Accumulator & flags */
		Register	BC; /* */
		Register	DE; /* */
		Register	HL; /* */
		Register	SP; /* Stack pointer */
		Register	PC; /* Program counter */
		uint8_t		IE; /* Interrupt Enable Register*/

		uint16_t	cyclesPassed = 0; /* */

		bool		opcodeIsPrefixed = false;
		uint8_t		currentOpcode;

		MemoryBus	memoryBus;

	public:

		/**
		 * @brief -.
		 * @param opCode 
		 */
		void ExecuteInstruction(OpCode opCode);

		/**
		 * @brief -.
		 * @param opCode 
		 */
		void ExecuteInstruction(PrefixedOpCode opCode);

		/**
		 * @brief Executes the current instruction.
		 */
		void ExecuteInstruction();

		/**
		 * @brief Fetches the instruction located on the address stored inside the PC register.
		 */
		void FetchInstruction();

		/**
		 * @brief -.
		 */
		uint8_t GetInterruptRegister();

	};

} /* namespace Gameboy */
