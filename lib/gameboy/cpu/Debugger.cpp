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

#include <play-man/gameboy/cpu/Debugger.hpp>
#include <play-man/utility/UtilFunc.hpp>

namespace GameBoy {

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
        userInput.split();
    }

    static void PrintCurrentPC(Instruction currentInst, uint16_t pc)
    {
        std::cout << Utility::IntAsHexString(pc) << ": " << currentInst.OpCodeAsHexString() << ": ";
        if (currentInst.IsPrefixed())
            std::cout << currentInst.GetPrefixedOpCode() << "\n";
        else
            std::cout << currentInst.GetOpCode() << "\n";
    }

    const char* optionsNoCall[] = {"help", "h", "step", "s", "r", "run", "b", "break", "c", NULL};
    const char* optionsCall[] = {"help", "h", "step", "s", "b", "break", "c", "terminate", NULL};

    static char* CompleterGeneratorNoCall(const char* text, int state)
    {
        static int  list_index;
        static int  len;
        const char* name;

        // First call to the generator for a new prompt will have the state as zero
        if (state == 0) {
            list_index = 0;
            len        = strlen(text);
        }

        name = optionsNoCall[list_index];
        while (name != NULL) {
            list_index++;
            if (strncmp(name, text, len) == 0) {
                return strdup(name);
            }
            name = optionsNoCall[list_index];
        }

        return NULL;
    }

    /**
     * Supposed to return NULL on no match found, or an array of options following a NULL.
     * rl_completion_matches creates this array for us.
     */
    static char** CompleterNoCall(const char* text, int start, int end)
    {
        (void)start;
        (void)end;

        // Don't fall back on default completion if no match is found
        rl_attempted_completion_over = 1;

        return rl_completion_matches(text, CompleterGeneratorNoCall);
    }

    static char* CompleterGeneratorCall(const char* text, int state)
    {
        static int  list_index;
        static int  len;
        const char* name;

        // First call to the generator for a new prompt will have the state as zero
        if (state == 0) {
            list_index = 0;
            len        = strlen(text);
        }

        name = optionsCall[list_index];
        while (name != NULL) {
            list_index++;
            if (strncmp(name, text, len) == 0) {
                return strdup(name);
            }
            name = optionsCall[list_index];
        }

        return NULL;
    }

    /**
     * Supposed to return NULL on no match found, or an array of options following a NULL.
     * rl_completion_matches creates this array for us.
     */
    static char** CompleterCall(const char* text, int start, int end)
    {
        (void)start;
        (void)end;

        // Don't fall back on default completion if no match is found
        rl_attempted_completion_over = 1;

        return rl_completion_matches(text, CompleterGeneratorCall);
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

        if (shouldPrompt)
        {
            if (currentInstIsCall)
                rl_attempted_completion_function = &CompleterCall;
            else
                rl_attempted_completion_function = &CompleterNoCall;

            while (true)
            {
                const char* userInput = readline("Debugger >");
                const shouldBreak = handleUserInput(userInput, currentInstIsCall);

                free(userInput);
                if (shouldBreak)
                    break ;
            }
        }
    }

}
