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
        x(n, STEP_OVER,   3)  \
        x(n, STEP_DOWN,   4)

    CREATE_ENUM_WITH_UTILS(DEBUGGER_STATE_SEQ, DebuggerState)
    #undef DEBUGGER_STATE_SEQ

    #define DEBUGGER_COMMANDS_SEQ(x, n) \
        x(n, HELP,             0)  \
        x(n, PRINT,            1)  \
        x(n, STEP,             2)  \
        x(n, CONTINUE,         3)  \
        x(n, BREAKPOINT,       4)  \
        x(n, LIST_BREAKPOINTS, 5)  \
        x(n, UP,               6)  \
        x(n, DOWN,             7)  \
        x(n, STEP_OVER,        8)  \
        x(n, INVALID,          9)

    CREATE_ENUM_WITH_UTILS(DEBUGGER_COMMANDS_SEQ, DebuggerCommands)
    #undef DEBUGGER_COMMANDS_SEQ

    /**
     * @brief A messy debugger class used to debug instructions and the CPU core
     *        after an instruction has been fetched inside the CPU.
     */
    class Debugger
    {
    private:

        /**
         * @brief Execute and step over the current instructions.
         * 
         * @return False.
         */
        bool StepInstruction();

        /**
         * @brief Steps over a 'call' instruction.
         * 
         * @return False.
         */
        bool StepOver();

        /**
         * @brief Continues till the next 'call' instruction.
         * 
         * @return False.
         */
        bool StepDown();

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
         * @return True.
         */
        bool SetBreakpoint(std::string& userInput);

        /**
         * @brief Lists the current breakpoints.
         * 
         * @return True.
         */
        bool ListBreakpoints();

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
         * @return False.
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
         * @brief Stores the return addresses of called functions.
         */
        std::stack<uint16_t> calledFunctions;

        /**
         * @brief Stores the possible return addresses a user can 'StepOut/d/down' to.
         * This is different from the 'calledFunctions', since a new function can be called
         * whilst the state is in 'RETURNING'.
         */
        std::stack<uint16_t> returnableAddresses;

        /**
         * @brief The amount of times the debugger will 'return' from a call.
         */
        size_t  currentReturnCount;

        /**
         * @brief The current set breakpoints.
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
