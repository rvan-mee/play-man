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

#include <play-man/gameboy/opcodes/Opcodes.hpp>

#include <optional>
#include <functional>
#include <iostream>

namespace GameBoy
{	
	/*!
	 * @brief Holds information about a instruction.
	*/
	struct Instruction
	{
		friend void to_json(nlohmann::json& j, const Instruction& instruction);
		friend std::ostream& operator << (std::ostream& os, Instruction& i);

	private:

		using InstructionPrototype = std::function<size_t()>;

		OpCode opCode; /*! <-. */
		std::optional<PrefixedOpCode> prefixedOpCode; /*!< -. */
		InstructionPrototype instructionToExecute; /*!< Bound function from the InstructionTable. */

	public:

		/**
		 * @brief Non prefixed instruction.
		 * @param opCode 
		 * @param instruction 
		 */
		Instruction(OpCode opCode, InstructionPrototype instruction);

		/**
		 * @brief -.
		 * @param opCode 
		 * @param instruction 
		 */
		Instruction(PrefixedOpCode opCode, InstructionPrototype instruction);

		/**
		 * @brief -.
		 * @return the number of cycles the instruction took to execute.
		 */
		size_t Execute();

		/**
		 * @brief -.
		 * @return
		 */
		bool IsPrefixed();
	};

	/**
	 * @brief -.
	 * @param os 
	 * @param i 
	 * @return
	 */
	std::ostream& operator << (std::ostream& os, Instruction& i);

	/**
	 * @brief Serializes a instruction to json format.
	 * @param j 
	 * @param instruction 
	 */
	void to_json(nlohmann::json& j, const Instruction& instruction);

} /* namespace Gameboy */
