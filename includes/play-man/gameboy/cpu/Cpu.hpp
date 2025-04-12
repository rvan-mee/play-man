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

#include <play-man/gameboy/cartridge/Cartridge.hpp>
#include <play-man/gameboy/memory/MemoryBus.hpp>
#include <play-man/gameboy/opcodes/Opcodes.hpp>
<<<<<<< HEAD
#include <play-man/gameboy/cpu/CpuCore.hpp>
#include <memory>
#include <array>
=======
#include <play-man/containers/EnumIndexableArray.hpp>
#include <functional>
>>>>>>> 1edae86 (chore!: enumindexablearray)
#include <stdint.h>


namespace GameBoy
{
	
	class Cpu
    {
    private:
        std::shared_ptr<ACartridge>     cartridge;
        CpuCore                         core;
        MemoryBus                       memoryBus;

        bool    opcodeIsPrefixed;
        uint8_t currentOpcode;

		Instruction currentInstruction;

		static constexpr size_t numberOfInstructions = 256;
		static constexpr size_t numberOfPrefixedInstructions = 256;
		using InstructionPrototype = std::function<void()>;

		EnumIndexableArray<OpCode, InstructionPrototype, numberOfInstructions> instructions;
		EnumIndexableArray<PrefixedOpCode, InstructionPrototype, numberOfPrefixedInstructions> prefixedInstructions;

        /* GameBoy CPU instructions */
        void    NOP();

    public:

        Cpu() = delete;
        Cpu(std::shared_ptr<ACartridge> _cartridge) : cartridge(_cartridge), memoryBus(cartridge, core)
        {
            InitInstructionTable();
        };

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
         * @brief Logs the current instruction being executed in debug mode.
         */
        void LogInstruction();

        /**
         * @brief Executes the current instruction.
         */
        void ExecuteInstruction();

        /**
         * @brief Fetches the instruction located on the address stored inside the PC register.
         */
        void FetchInstruction();

        /**
         * @brief Initializes the instruction array.
         */
        void InitInstructionTable();

    };

}