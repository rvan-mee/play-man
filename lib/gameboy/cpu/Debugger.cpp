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

#define PROMPT "Debugger >"

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

    bool Debugger::StepInstruction()
    {

    }

    bool Debugger::StepOver()
    {

    }

    bool Debugger::StepIn()
    {

    }

    bool Debugger::StepOut(std::string& userInput)
    {

    }

    bool Debugger::SetBreakpoint(std::string& userInput)
    {

    }

    bool Debugger::PrintHelp()
    {
        std::cout << "Available options: ";
        std::cout << "    h/help           : You just ran this command!\n";
        std::cout << "    p/print          : Prints the state of the CPU.\n";
        std::cout << "    s/step           : Single step, executes the current instruction.\n";
        std::cout << "    c/continue       : Continue to the next breakpoint, or till the end.\n";
        std::cout << "    b/breakpoint [n] : Set a breakpoint.\n";
        std::cout << "    up [n]           : Go up one or [n] stack levels, to the calling function.\n";
        std::cout << "    do/down          : Go down one stack level, to the next called function.\n";
        std::cout << "    o/over           : Jump over the current function call.\n";
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

    static std::vector<std::string> splitString(const std::string& input, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(input);
        std::string token;

        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }

    DebuggerCommands Debugger::GetUserCommandType(std::string& userInput)
    {
        if (!userInput.size())
            return DebuggerCommands::INVALID;

        std::vector<std::string> splitInput = splitString(userInput, ' ');

        if (!splitInput[0].size())
            return DebuggerCommands::INVALID;
        if (splitInput[0] == "h" || splitInput[0] == "help")
            return DebuggerCommands::HELP;
        if (splitInput[0] == "p" || splitInput[0] == "print")
            return DebuggerCommands::PRINT;
        if (splitInput[0] == "s" || splitInput[0] == "step")
            return DebuggerCommands::STEP;
        if (splitInput[0] == "c" || splitInput[0] == "continue")
            return DebuggerCommands::CONTINUE;
        if (splitInput[0] == "b" || splitInput[0] == "breakpoint")
            return DebuggerCommands::BREAKPOINT;
        if (splitInput[0] == "up")
            return DebuggerCommands::UP;
        if (splitInput[0] == "do" || splitInput[0] == "down")
            return DebuggerCommands::DOWN;
        if (splitInput[0] == "0" || splitInput[0] == "over")
            return DebuggerCommands::STEP_OVER;

        return DebuggerCommands::INVALID;
    }

    bool Debugger::HandleUserInput(std::string userInput)
    {
        DebuggerCommands commandType = GetUserCommandType(userInput);

        switch(commandType)
        {
            case DebuggerCommands::HELP:
                return PrintHelp();
            case DebuggerCommands::PRINT:
                return PrintState();
            case DebuggerCommands::STEP:
                return StepInstruction();
            case DebuggerCommands::CONTINUE:
               return Continue();
            case DebuggerCommands::BREAKPOINT:
                return SetBreakpoint(userInput);
            case DebuggerCommands::UP:
                return StepOut(userInput);
            case DebuggerCommands::DOWN:
                return StepIn();
            case DebuggerCommands::STEP_OVER:
                return StepOver();
            case DebuggerCommands::INVALID:
            default:
            {
                std::cout << PROMPT << " Unknown command: " << userInput << std::endl;
                return true;
            }
        }
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
        bool      shouldPrompt = (state == DebuggerState::STEPPING);

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
                calledFunctions.push(currentPC);
                if (state == )
                returnableAddresses
            }
        }
        else
            currentPC -= 2;

        PrintCurrentPC(cpu->currentInstruction, currentPC);

        while (shouldPrompt)
        {
            char*   userInput = readline(PROMPT);

            shouldPrompt = HandleUserInput(userInput);

            free(userInput);
        }
    }

}
