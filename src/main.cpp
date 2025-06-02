// // ****************************************************************************** //
// //   _______   __                              __       __                        //
// //  /       \ /  |                            /  \     /  |                       //
// //  $$$$$$$  |$$ |  ______   __    __         $$  \   /$$ |  ______   _______     //
// //  $$ |__$$ |$$ | /      \ /  |  /  | ______ $$$  \ /$$$ | /      \ /       \    //
// //  $$    $$/ $$ | $$$$$$  |$$ |  $$ |/      |$$$$  /$$$$ | $$$$$$  |$$$$$$$  |   //
// //  $$$$$$$/  $$ | /    $$ |$$ |  $$ |$$$$$$/ $$ $$ $$/$$ | /    $$ |$$ |  $$ |   //
// //  $$ |      $$ |/$$$$$$$ |$$ \__$$ |        $$ |$$$/ $$ |/$$$$$$$ |$$ |  $$ |   //
// //  $$ |      $$ |$$    $$ |$$    $$ |        $$ | $/  $$ |$$    $$ |$$ |  $$ |   //
// //  $$/       $$/  $$$$$$$/  $$$$$$$ |        $$/      $$/  $$$$$$$/ $$/   $$/    //
// //                          /  \__$$ |                                            //
// //                          $$    $$/                                             //
// //                           $$$$$$/                                              //
// //                                                                                //
// //                            By: K1ngmar and rvan-mee                            //
// // ****************************************************************************** //

#define SDL_MAIN_USE_CALLBACKS

#include <play-man/graphics/UserInterface.hpp>
#include <play-man/gameboy/cartridge/Cartridge.hpp>
#include <play-man/settings/PlayManSettings.hpp>
#include <play-man/gameboy/opcodes/Opcodes.hpp>
#include <play-man/gameboy/cpu/Cpu.hpp>
#include <play-man/logger/Logger.hpp>
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


SDL_AppResult SDL_Fail()
{
    LOG_ERROR(SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
	(void)argc;
	(void)argv;
    Logger::LogInterface::Initialize("Logging", Logger::LogLevel::Debug);
    Graphics::UserInterface::Initialize();

    if (argc != 2)
    {
        LOG_ERROR("Not enough arguments, please provide a ROM.");
        return SDL_APP_FAILURE;
    }

    try
    {
        auto cartridge = GameBoy::MakeCartridge(argv[1]);
        // For now we can store a single CPU, we can wrap this around an application class later on.
        *appstate = new GameBoy::Cpu(cartridge);
    }
    catch(const std::exception& e)
    {
        LOG_ERROR(e.what());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    (void) appstate;
    (void) event;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    GameBoy::Cpu *cpu = static_cast<GameBoy::Cpu *>(appstate);
    Graphics::UserInterface::ClearScreen();

    cpu->RenderFrame();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult)
{
    delete static_cast<GameBoy::Cpu*>(appstate);
    SDL_Quit();
}
