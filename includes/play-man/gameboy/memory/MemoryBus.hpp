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
#include <play-man/ROM/Rom.hpp> // TODO: move ROM to /play-man/gameboy/ROM/?
#include <stdint.h>

namespace GameBoy {

    class Cpu; // pre-declare the CPU class

    class MemoryBus {
        private:
            Rom* rom;
            Cpu* cpu;
            // TODO:
            // video module
            // io module
            // other modules

        public:
            // TODO:
            // void   init(Rom* rom, Cpu* cpu);

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

    };

}
