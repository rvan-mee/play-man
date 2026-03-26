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

#define PROMPT "Debugger> "

#define PROMPT_AGAIN true
#define STOP_PROMPT false

namespace GameBoy {

    const char* options[] = {"help", "h", "p", "print" "step", "s", "c", "continue", \
                             "b", "breakpoint", "lb", "list", "up", "do", "down", NULL};

    static char* CompleterGenerator(const char* text, int state)
    {
        static int  list_index;
        static int  len;
        const char* name;

        // First call to the generator for a new prompt will have the state as zero
        if (state == 0)
        {
            list_index = 0;
            len = strlen(text);
        }

        name = options[list_index];
        while (name != NULL)
        {
            list_index++;
            if (strncmp(name, text, len) == 0)
                return strdup(name);
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
        breakpoints = {};
        
        if (settings.get()->breakpointsGameBoy.size() != 0)
            breakpoints = settings.get()->breakpointsGameBoy;

        state = DebuggerState::STEPPING;
        rl_attempted_completion_function = &Completer;
    }

    Debugger::~Debugger()
    {
    }

    bool Debugger::StepInstruction()
    {
        state = DebuggerState::STEPPING;
        return STOP_PROMPT;
    }

    bool Debugger::StepOver()
    {
        state = DebuggerState::STEP_OVER;
        return STOP_PROMPT;
    }

    bool Debugger::StepDown()
    {
        state = DebuggerState::STEP_DOWN;
        return STOP_PROMPT;
    }

    bool Debugger::StepOut(std::string& userInput)
    {
        try 
        {
            size_t timesToReturn = 1;

            if (userInput.size() != 0)
                timesToReturn = std::stoi(userInput.substr(2, userInput.size()));
                
            if (returnableAddresses.size() < timesToReturn)
                throw std::exception();   

            currentReturnCount = timesToReturn;
            state = DebuggerState::RETURNING;
            return STOP_PROMPT;
        }
        catch (const std::exception& e)
        {
            std::cout << "Invalid input: `" << userInput << "`\nCurrent maximum return availability: " << returnableAddresses.size() << " times\n";
            state = DebuggerState::STEPPING;
            return PROMPT_AGAIN;
        }
    }

    bool Debugger::SetBreakpoint(std::string& userInput)
    {
        size_t startIndex = 1;

        // If breakpoint is given as opposed to just 'b'
        if (userInput.starts_with("breakpoint"))
            startIndex = 11;

        // Skip any spaces after the command
        startIndex = userInput.find_first_not_of(" ", startIndex);

        try
        {
            int32_t breakpoint = 0;

            if (std::string_view(userInput.begin() + startIndex, userInput.end()).starts_with("0x"))
                breakpoint = std::stoi(userInput.substr(startIndex, userInput.size()), nullptr, 16);
            else
                breakpoint = std::stoi(userInput.substr(startIndex, userInput.size()), nullptr, 10);

            // Check for the ROM address range:
            if (breakpoint < 0 || breakpoint > 0x7FFF)
                throw std::exception();

            for (const auto& storedBreakpoint : breakpoints)
            {
                if (breakpoint == storedBreakpoint)
                {
                    std::cout << "Breakpoint already saved!\nUse lb or list to show current breakpoints.\n";
                    return PROMPT_AGAIN;
                }
            }

            breakpoints.push_back(breakpoint);
            return PROMPT_AGAIN;
        }
        catch(const std::exception& e)
        {
            std::cout << "Invalid input: `" << userInput << "`\n";
            return PROMPT_AGAIN;
        }
    }

    bool Debugger::ListBreakpoints()
    {
        if (breakpoints.size() == 0)
            std::cout << "No breakpoints saved!\n";

        for (size_t i = 0; i < breakpoints.size(); i++)
            std::cout << "breakpoint[" << i + 1 << "]: " << Utility::IntAsHexString(breakpoints[i]) << "\n";
        return PROMPT_AGAIN;
    }

    bool Debugger::Continue()
    {
        state = DebuggerState::BREAKPOINT;
        return STOP_PROMPT;
    }

    bool Debugger::PrintState()
    {
        std::cout << cpu->GetCpuCore();
        return PROMPT_AGAIN;
    }

    bool Debugger::PrintHelp()
    {
        std::cout << "Available options:\n";
        std::cout << "    h/help           : You just ran this command!\n";
        std::cout << "    p/print          : Prints the state of the CPU.\n";
        std::cout << "    s/step           : Single step, executes the current instruction.\n";
        std::cout << "    c/continue       : Continue to the next breakpoint, or till the end.\n";
        std::cout << "    b/breakpoint [n] : Set a breakpoint.\n";
        std::cout << "    lb/list          : Lists the current breakpoints.\n";
        std::cout << "    up [n]           : Go up one or [n] stack levels, to the calling function.\n";
        std::cout << "    do/down          : Go down one stack level, to the next called function.\n";
        std::cout << "    o/over           : Jump over the current function call.\n";

        return PROMPT_AGAIN;
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

    static std::vector<std::string> splitString(const std::string& input, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(input);
        std::string token;

        while (std::getline(ss, token, delimiter))
            tokens.push_back(token);

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
        if (splitInput[0] == "lb" || splitInput[0] == "list")
            return DebuggerCommands::LIST_BREAKPOINTS;
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
            case DebuggerCommands::LIST_BREAKPOINTS:
                return ListBreakpoints();
            case DebuggerCommands::UP:
                return StepOut(userInput);
            case DebuggerCommands::DOWN:
                return StepDown();
            case DebuggerCommands::STEP_OVER:
                return StepOver();
            case DebuggerCommands::INVALID:
            default:
            {
                std::cout << "Unknown command: `" << userInput << "`" << std::endl;
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

        // Program counter is already incremented to be *after* the current instruction since
        // the debugger is called after the fetch, the decrement to get the right counter happens below
        uint16_t  currentPC = cpu->GetCpuCore().PC.Value();
        bool      shouldPrompt = (state == DebuggerState::STEPPING);

        // Check if the current instruction is a call or a return:
        if (!cpu->currentInstruction.IsPrefixed())
        {
            if (IsReturn(cpu->currentInstruction.GetOpCode()))
            {
                uint16_t currentReturnAddress = calledFunctions.top();

                if (state == DebuggerState::RETURNING)
                {
                    // We can only be sure that we are on the right stack level if
                    // the calledFunctions stack is of the same size as returnableAddresses
                    if (calledFunctions.size() == returnableAddresses.size())
                    {
                        assert(currentReturnAddress == returnableAddresses.top());
                        returnableAddresses.pop();

                        currentReturnCount -= 1;

                        if (currentReturnCount == 0)
                        {
                            shouldPrompt = true;
                            state = DebuggerState::STEPPING;
                        }
                    }
                }
                calledFunctions.pop();
            }
            else if (IsCall(cpu->currentInstruction.GetOpCode()))
            {
                calledFunctions.push(currentPC);
                if (state == DebuggerState::STEP_DOWN)
                {
                    state = DebuggerState::STEPPING;
                    shouldPrompt = true;
                }

                if (state != DebuggerState::RETURNING)
                    returnableAddresses.push(currentPC);
            }
            currentPC -= 1;
        }
        else
            currentPC -= 2;

        if (state == DebuggerState::BREAKPOINT)
        {
            for (size_t i = 0; i < breakpoints.size(); i++)
            {
                if (currentPC == breakpoints[i])
                {
                    shouldPrompt = true;
                    state = DebuggerState::STEPPING;
                }
            }
        }

        PrintCurrentPC(cpu->currentInstruction, currentPC);

        char *lastInput = NULL;

        while (shouldPrompt)
        {
            char*   userInput = readline(PROMPT);

            if (!userInput)
            {
                free(lastInput);
                return ;
            }

            // Skip adding the same input
            if (userInput[0] != '\0' && (lastInput == NULL || strcmp(userInput, lastInput) != 0))
                add_history(userInput);

            shouldPrompt = HandleUserInput(userInput);

            free(lastInput);
            lastInput = userInput;
        }
    }

}
