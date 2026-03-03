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

#include <stack>
#include <vector>
#include <stdint.h>

#include <play-man/settings/PlayManSettings.hpp>
#include <play-man/utility/EnumMacro.hpp>

namespace GameBoy {

    class Cpu;


    #define DEBUGGER_STATE_SEQ(x, n) \
        x(n, BREAKPOINT,  0)  \
        x(n, STEPPING,    1)  \
        x(n, RETURNING,   2)  \
        x(n, STEP_OVER,   3)

    CREATE_ENUM_WITH_UTILS(DEBUGGER_STATE_SEQ, DebuggerState)
    #undef DEBUGGER_STATE_SEQ

    class Debugger
    {
    private:

        /**
         * @brief Execute and step over the current instructions.
         */
        void StepInstruction();

        /**
         * @brief Steps over a 'call' instruction.
         */
        void StepOver();

        /**
         * @brief Goes into the current 'call' instruction.
         */
        void StepIn();

        /**
         * @brief Completes the current call, returns to the calling function.
         */
        void StepOut();

        /**
         * @brief Sets a breakpoint for when the program counter hits a specific value.
         */
        void SetBreakpoint();

        /**
         * @brief If the given opcode is a return instruction.
         */
        bool IsReturn(OpCode opCode);

        /**
         * @brief If the given opcode is a call instruction.
         */
        bool IsCall(OpCode opCode);

        /**
         * @brief Parses and handles the given user input string.
         * 
         * @return True if the input was handled and no new prompt needs to be given to the user.
         */
        bool HandleUserInput(std::string userInput, bool currentInstIsCall);

        /**
         * @brief Stores the return addresses of called functions to support stepping out.
         */
        std::stack<uint16_t> calledFunctions;

        /**
         * @brief If we need to look for a breakpoint.
         */
        bool   breakpointEnabled;

        /**
         * @brief The current set breakpoint.
         */
        std::vector<uint16_t> breakpoints;

        /**
         * @brief Pointer to the GameBoy instance.
         */
        Cpu* cpu;

        /**
         * @brief The settings of the emulator.
         */
        std::shared_ptr<PlayManSettings> settings;

        /**
         * @brief Whether or not the debugger is enabled.
         */
        const bool enabled;

        /**
         * @brief The current state of the debugger.
         */
        DebuggerState state;

    public:
        Debugger() = delete;
        Debugger(Cpu* _cpu, std::shared_ptr<PlayManSettings> _settings);
        ~Debugger();

        /**
         * @brief Enters the debugger to manually step into calls and instructions.
         * 
         * @note TODO: figure out what to do with interrupts.
         */
        void HandleDebug();
    };

}

#include <play-man/gameboy/cpu/Cpu.hpp>
