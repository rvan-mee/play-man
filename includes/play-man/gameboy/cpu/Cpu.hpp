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
#include <inttypes.h>
#include <play-man/gameboy/cpu/Opcodes.hpp>

namespace Gameboy
{
	class Cpu
	{
		Register AF; /* Accumulator & flags */
		Register BC; /* */
		Register DE; /* */
		Register HL; /* */
		Register SP; /* Stack pionter */
		Register PC; /* Program counter */

		uint16_t cyclesPassed = 0; /* */

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
	};

} /* namespace Gameboy */
