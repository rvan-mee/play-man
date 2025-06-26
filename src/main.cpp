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
#include <play-man/graphics/UserInterfaceDefines.hpp>
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

    std::shared_ptr<PlayManSettings> settings = PlayManSettings::ReadFromFile("settings.json");
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

        std::cout << *cartridge;
        // For now we can store a single CPU, we can wrap this around an application class later on.
        *appstate = new GameBoy::Cpu(cartridge, settings);

        const size_t scale = settings->screenScaleGameBoy;
        Graphics::UserInterface::Resize(GameBoyWindowWidth * scale, GameBoyWindowWidth * scale);
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
    GameBoy::Cpu *cpu = static_cast<GameBoy::Cpu *>(appstate);

    (void) cpu;
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE)
        return SDL_APP_SUCCESS;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    GameBoy::Cpu *cpu = static_cast<GameBoy::Cpu *>(appstate);
    Graphics::UserInterface::ClearScreen();

    cpu->RenderFrame();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    delete static_cast<GameBoy::Cpu*>(appstate);

    if (result == SDL_AppResult::SDL_APP_SUCCESS)
        LOG_DEBUG("App closing after success")
    else if (result == SDL_AppResult::SDL_APP_FAILURE)
        LOG_DEBUG("App closing after failure")
    else if (result == SDL_AppResult::SDL_APP_CONTINUE)
        LOG_DEBUG("App closing, result set to continue?")
    else
        LOG_DEBUG("App closing with an undefined result")
}
