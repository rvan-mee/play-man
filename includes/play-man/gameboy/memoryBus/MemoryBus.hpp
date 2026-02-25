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
#include <play-man/gameboy/memoryBus/MemoryDefines.hpp>
#include <stdint.h>

namespace GameBoy {

    // Forward declaration of the CPU class used to access all modules.
    class Cpu;

    class MemoryBus {
        private:
            Cpu* cpu;

            std::array<WorkRamBank, 8> workRam;

        public:
            MemoryBus() = delete;
            MemoryBus(Cpu* _cpu);

            /**
             * @brief Passthrough function to call the regular Readbyte,
             * converting the register into an u16.
             * @param reg The register to get the address from.
             */
            uint8_t ReadByte(const Register reg);

            /**
             * @brief Handles the correct mapping of an address to a location,
             * be it on the ROM, inside the CPU or any other module.
             * @param address The address to fetch the data from.
             */
            uint8_t ReadByte(const uint16_t address);


            /**
             * @brief Passthrough function to call the regular Writebyte,
             * converting the register into an u16.
             * @param reg The register to get the address from.
             * @param value The value to be set.
             */
            void WriteByte(const Register reg, const uint8_t value);

            /**
             * @brief Handles the correct mapping of an address to a location,
             * be it on the ROM, inside the CPU or any other module.
             * @param address The address to set the data in.
             * @param value The value to be set.
             */
            void WriteByte(const uint16_t address, const uint8_t value);
             
            /**
             * @brief Pops 2 bytes from the address pointed to by SP, which then
             * gets incremented by 2.
             * 
             * @note The bytes are popped in order of LowByte then HighByte.
             */
            uint16_t PopStack();

            /**
             * @brief Pushes 2 bytes to the address pointed to by SP, which then
             * gets decremented by 2.
             * 
             * @note The bytes are pushed in oder of HighByte then LowByte.
             */
            void PushStack(uint16_t value);

    };

}

#include <play-man/gameboy/cpu/Cpu.hpp>
