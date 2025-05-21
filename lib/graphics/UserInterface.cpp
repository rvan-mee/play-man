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

#include <play-man/graphics/UserInterface.hpp>
#include <play-man/graphics/UserInterfaceDefines.hpp>
#include <stdexcept>

namespace Graphics {

UserInterface::~UserInterface()
{
    SDL_DestroyWindow(AppWindow);
    AppWindow = nullptr;

    SDL_Quit();
}

void UserInterface::InitializeMainApplicationWindow()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        throw std::runtime_error("Failed to initialize SDL");

    AppWindow = SDL_CreateWindow(AppWindowName, DefaultAppWindowWidth, DefaultAppWindowHeight, SDL_WINDOW_RESIZABLE);
    if (!AppWindow)
        throw std::runtime_error("Failed to create a window.");

    AppSurface = SDL_GetWindowSurface(AppWindow);
    if (!AppSurface)
        throw std::runtime_error("Failed to get the surface of the window");

    if (!SDL_GetWindowSize(AppWindow, &AppWindowWidth, &AppWindowHeight))
        throw std::runtime_error("Failed to get the window size");
}

std::unique_ptr<UserInterface>& UserInterface::GetInstance()
{
    static std::unique_ptr<UserInterface> ui;
    return ui;
}

void UserInterface::Initialize()
{
    auto& ui = GetInstance();

    if  (ui != nullptr)
    {
        throw std::runtime_error("UserInterface is already initialized.");
    }

    ui = std::make_unique<UserInterface>();

    InitializeMainApplicationWindow();
}

}
