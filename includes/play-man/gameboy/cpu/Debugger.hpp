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
#include <play-man/gameboy/opcodes/Opcodes.hpp>

namespace GameBoy {

    class Cpu;


    #define DEBUGGER_STATE_SEQ(x, n) \
        x(n, BREAKPOINT,  0)  \
        x(n, STEPPING,    1)  \
        x(n, RETURNING,   2)  \
        x(n, STEP_OVER,   3)

    CREATE_ENUM_WITH_UTILS(DEBUGGER_STATE_SEQ, DebuggerState)
    #undef DEBUGGER_STATE_SEQ

    #define DEBUGGER_COMMANDS_SEQ(x, n) \
        x(n, HELP,       0)  \
        x(n, PRINT,      1)  \
        x(n, STEP,       2)  \
        x(n, CONTINUE,   3)  \
        x(n, BREAKPOINT, 4)  \
        x(n, UP,         5)  \
        x(n, DOWN,       6)  \
        x(n, STEP_OVER,  7)  \
        x(n, INVALID,    8)

    CREATE_ENUM_WITH_UTILS(DEBUGGER_COMMANDS_SEQ, DebuggerCommands)
    #undef DEBUGGER_COMMANDS_SEQ

    class Debugger
    {
    private:

        /**
         * @brief Execute and step over the current instructions.
         * 
         * @return True if the user should be prompted again.
         */
        bool StepInstruction();

        /**
         * @brief Steps over a 'call' instruction.
         * 
         * @return True if the user should be prompted again.
         */
        bool StepOver();

        /**
         * @brief Goes into the next 'call' instruction.
         * 
         * @return True if the user should be prompted again.
         */
        bool StepIn();

        /**
         * @brief Completes the current call, returns to the calling function.
         * Optionally can step out of multiple functions at the same time.
         * 
         * @return True if the user should be prompted again.
         */
        bool StepOut(std::string& userInput);

        /**
         * @brief Sets a breakpoint for when the program counter hits a specific value.
         * 
         * @return True if the user should be prompted again.
         */
        bool SetBreakpoint(std::string& userInput);

        /**
         * @brief Prints helpful information of the debugger to the stdout.
         * 
         * @return True if the user should be prompted again.
         */
        bool PrintHelp();

        /**
         * @brief Prints the state of the CPU to the stdout.
         * 
         * @return True if the user should be prompted again.
         */
        bool PrintState();

        /**
         * @brief Sets the debugger in a state that it will continue to the next breakpoint,
         * or the end of the program.
         * 
         * @return True if the user should be prompted again.
         */
        bool Continue();

        /**
         * @brief If the given opcode is a return instruction.
         */
        bool IsReturn(OpCode opCode);

        /**
         * @brief If the given opcode is a call instruction.
         */
        bool IsCall(OpCode opCode);

        /**
         * @brief Returns the command the user has requested.
         */
        DebuggerCommands GetUserCommandType(std::string& userInput);

        /**
         * @brief Parses and handles the given user input string.
         * 
         * @return True if the user should be prompted again.
         */
        bool HandleUserInput(std::string userInput);

        /**
         * @brief Stores the return addresses of called functions to support stepping out.
         */
        std::stack<uint16_t> calledFunctions;

        /**
         * @brief Stores the possible return addresses a user can 'StepOut/d/down' to.
         */
        std::stack<uint16_t> returnableAddresses;

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
