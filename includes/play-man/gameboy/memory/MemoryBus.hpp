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
#include <play-man/gameboy/cpu/CpuCore.hpp>
#include <play-man/gameboy/cartridge/Cartridge.hpp>
#include <stdint.h>

namespace GameBoy {

    class MemoryBus {
        private:
            std::shared_ptr<ACartridge> cartridge;
            CpuCore&                    core;
            // TODO:
            // video module
            // io module
            // other modules

        public:
            MemoryBus() = delete;
            MemoryBus(std::shared_ptr<ACartridge> _cartridge, CpuCore& _core) : cartridge(_cartridge), core(_core) {};

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
    };

}
