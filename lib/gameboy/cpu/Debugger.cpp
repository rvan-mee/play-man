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

#include <readline/history.h>
#include <readline/readline.h>

#include <iostream>
#include <stdlib.h>

#include <play-man/gameboy/cpu/Debugger.hpp>
#include <play-man/utility/UtilFunc.hpp>

namespace GameBoy {

    const char* options[] = {"help", "h", "p", "print" "step", "s", "c", "continue", "b", "break", "c", "up", "do", "down", NULL};

    static char* CompleterGenerator(const char* text, int state)
    {
        static int  list_index;
        static int  len;
        const char* name;

        // First call to the generator for a new prompt will have the state as zero
        if (state == 0) {
            list_index = 0;
            len        = strlen(text);
        }

        name = options[list_index];
        while (name != NULL) {
            list_index++;
            if (strncmp(name, text, len) == 0) {
                return strdup(name);
            }
            name = options[list_index];
        }

        return NULL;
    }

    /**
     * Supposed to return NULL on no match found, or an array of options following a NULL.
     * rl_completion_matches creates this array for us.
     */
    static char** Completer(const char* text, int start, int end)
    {
        (void)start;
        (void)end;

        // Don't fall back on default completion if no match is found
        rl_attempted_completion_over = 1;

        return rl_completion_matches(text, CompleterGenerator);
    }

    Debugger::Debugger(Cpu* _cpu, std::shared_ptr<PlayManSettings> _settings) : cpu(_cpu), settings(_settings), enabled(settings.get()->enableDebuggingGameBoy)
    {
        breakpointEnabled = settings.get()->enableBreakpointGameBoy;
        breakpoints = {};

        
        if (breakpointEnabled)
        {
            breakpoints.push_back(settings.get()->breakpointGameBoy);
            state = DebuggerState::BREAKPOINT;
        }
        else
            state = DebuggerState::STEPPING;

        rl_attempted_completion_function = &Completer;
    }

    Debugger::~Debugger()
    {
    }

    void Debugger::StepInstruction()
    {

    }

    void Debugger::StepOver()
    {

    }

    void Debugger::StepIn()
    {

    }

    void Debugger::StepOut()
    {

    }

    void Debugger::SetBreakpoint()
    {

    }

    void Debugger::PrintHelp()
    {
        std::cout << "Available options: ";
        std::cout << "    h/help       : You just ran this command!\n";
        std::cout << "    p/print      : Prints the state of the CPU.\n";
        std::cout << "    s/step       : Single step, executes the current instruction.\n";
        std::cout << "    c/continue   : Continue to the next breakpoint, or till the end.\n";
        std::cout << "    b/breakpoint : Set a breakpoint.\n";
        std::cout << "    up           : .\n";
        std::cout << "    do/down      : .\n";
        std::cout << "    o/over       : .\n";
    }

    bool Debugger::IsReturn(OpCode opCode)
    {
        if (opCode == OpCode::RET    || \
            opCode == OpCode::RET_C  || \
            opCode == OpCode::RET_NC || \
            opCode == OpCode::RET_NZ || \
            opCode == OpCode::RET_Z  || \
            opCode == OpCode::RETI)
            return true;
        return false;
    }

    bool Debugger::IsCall(OpCode opCode)
    {
        if (opCode == OpCode::CALL_a16    || \
            opCode == OpCode::CALL_C_a16  || \
            opCode == OpCode::CALL_NC_a16 || \
            opCode == OpCode::CALL_NZ_a16 || \
            opCode == OpCode::CALL_Z_a16)
            return true;
        return false;
    }

    bool Debugger::HandleUserInput(std::string userInput, bool currentInstIsCall)
    {
        (void) userInput;
        (void) currentInstIsCall;
        return false;
    }

    static void PrintCurrentPC(Instruction currentInst, uint16_t pc)
    {
        std::cout << Utility::IntAsHexString(pc) << ": " << currentInst.OpCodeAsHexString() << "> ";
        if (currentInst.IsPrefixed())
            std::cout << currentInst.GetPrefixedOpCode() << "\n";
        else
            std::cout << currentInst.GetOpCode() << "\n";
    }

    void Debugger::HandleDebug()
    {   
        if (!enabled)
            return ;

        uint16_t  currentPC = cpu->GetCpuCore().PC.Value();
        bool      currentInstIsCall = false;
        bool      shouldPrompt;

        shouldPrompt = state == DebuggerState::STEPPING;

        // Check if the current instruction is a call or a return:
        if (!cpu->currentInstruction.IsPrefixed())
        {
            currentPC -= 1;

            if (IsReturn(cpu->currentInstruction.GetOpCode()))
            {
                uint16_t currentReturnAddress = calledFunctions.top();
                calledFunctions.pop();

                (void) currentReturnAddress;

                if (state == DebuggerState::RETURNING)
                {
                    shouldPrompt = true;
                    state = DebuggerState::STEPPING;
                }
            }
            else if (IsCall(cpu->currentInstruction.GetOpCode()))
            {
                if (state != DebuggerState::RETURNING && state != DebuggerState::STEP_OVER)
                    calledFunctions.push(currentPC);
                currentInstIsCall = true;
            }
        }
        else
            currentPC -= 2;

        PrintCurrentPC(cpu->currentInstruction, currentPC);

        while (shouldPrompt)
        {
            char*   userInput = readline("Debugger >");

            shouldPrompt = HandleUserInput(userInput, currentInstIsCall);

            free(userInput);
        }
    }

}
