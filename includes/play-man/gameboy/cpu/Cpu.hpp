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
#include <play-man/gameboy/cpu/Instruction.hpp>
#include <play-man/containers/EnumIndexableArray.hpp>

#include <functional>
#include <stdint.h>

namespace GameBoy
{	
    class Cpu
    {
        friend struct TestFixtures::GameBoyCpuFixture;

        using InstructionPrototype = Instruction::InstructionPrototype; /*!< -. */
    
    private:
        std::shared_ptr<ACartridge>     cartridge;
        CpuCore                         core;
        MemoryBus                       memoryBus;

        size_t cycles = 0;
        Instruction currentInstruction; /*< The current instruction to execute/is being executed. */

        static constexpr size_t numberOfInstructions = 256;
        static constexpr size_t numberOfPrefixedInstructions = 256;

        EnumIndexableArray<OpCode, InstructionPrototype, numberOfInstructions> instructions;
        EnumIndexableArray<PrefixedOpCode, InstructionPrototype, numberOfPrefixedInstructions> prefixedInstructions;

    public:

        Cpu() = delete;
        Cpu(std::shared_ptr<ACartridge> _cartridge) : cartridge(_cartridge), memoryBus(cartridge, core)
        {
            InitInstructionTable();
        };

        /**
         * @brief Used for testing, overwrites the current ROM data with the data
         *        found inside the file at filePath. Also resets all the internal registers.
         * @param filePath The location of the test rom.
         */
        void LoadTestRom(const char *filePath);

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

        /*!
         * @brief Reads byte from memory bus at address.
         * @brief address
         * @return
        */
        uint8_t Fetch(uint16_t address);

        /**
         * @brief Reads byte from memory bus at address contained in PC; increments PC;
         * @return 
         */
        uint8_t FetchPcAddress();

        /**
         * @brief Initializes the instruction array.
         */
        void InitInstructionTable();

//////////////////
// Instructions //
//////////////////
private:

        /**
         * @brief
         * @return number of cycles.
         */
        size_t NOP();

        /**
         * @brief Loads two bytes of immediate data into reg.
         * 		  First byte of immediate data is low byte.
         * @param reg 
         * @return size_t 
         */
        size_t Load_16bit_ImmediateData(Register CpuCore::* reg);

        /**
         * @brief Loads two bytes of immediate date into the PC reg.
         *        First byte of immediate data is low byte.
         * @return number of cycles.
         */
        size_t Jump_16bit_ImmediateData();
    };

}