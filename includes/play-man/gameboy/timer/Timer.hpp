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

#include <stdint.h>

namespace GameBoy {

    class Cpu;

    class Timer
    {
    private:

        /** Registers **/

        /**
         * @brief The divider register
         * 
         * Is incremented at a rate of 16384Hz (in CGB double speed 32768Hz).
         * Writing any value to this register resets it back to 0x00.
         */
        uint8_t divider;

        /**
         * @brief The timer counter register.
         * 
         * This timer is incremented by the clock frequency specified by the control register.
         * When the value overflows (> 0xFF) it is reset to the value specified by the modulo register
         * and an interrupt is requested.
         */
        uint8_t counter;

        /**
         * @brief The timer modulo register.
         * 
         * This register holds the value that the counter will assume once it overflows.
         * 
         * @note If a write to this register happens in the same M-tick as the counter overflows,
         * the counter will assume the old modulo register's value. 
         */
        uint8_t modulo;

        /**
         * @brief The timer control register.
         * 
         * The bits have the following meaning:
         * bit 2: Enable - 1 Enables increments of the counter register
         * bit 1 and 0: The frequency at which the counter register gets updated
         * 
         * @note Disabling bit 2 will still allow the divider register to be incremented.
         */
        uint8_t control;

        /**
         * @brief Pointer to the cpu so we can request interrupts when needed.
         */
        Cpu* cpu;

        bool isStopped;

    public:
        Timer() = delete;
        Timer(Cpu* _cpu);
        ~Timer();

        // TODO:
        // void TickTimer();

        /**
         * @brief Resets the divider register back to 0x00.
         * 
         * To be called when a 'stop' instruction is performed or a CGB speed switch is done.
         */
        void ResetDivider();

        /**
         * @brief Sets the state of the timer, stopped or not.
         * 
         * The 'stop' instruction prevents the timer from incrementing its registers.
         */
        void SetStopped(bool stopped);

        /**
         * @brief Writes to the internal registers, address range of (0xFF04 - 0xFF07).
         */
        void WriteByte(uint16_t address, uint8_t value);

        /**
         * @brief Reads from the internal registers, address range of (0xFF04 - 0xFF07).
         */
        uint8_t ReadByte(uint16_t address);
    }; 

}
