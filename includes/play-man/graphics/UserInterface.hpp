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

#include <SDL3/SDL.h>
#include <memory>

namespace Graphics {

class UserInterface
{
private:
    /**
     * @brief The width of the main application window.
     */
    inline static int             AppWindowWidth;

    /**
     * @brief The height of the main application window.
     */
    inline static int             AppWindowHeight;

    /**
     * @brief The the main application window.
     */
    inline static SDL_Window*     AppWindow;

    /**
     * @brief The surface of the main application window.
     */
    inline static SDL_Surface*    AppSurface;

    /**
     * @brief Initializes the graphics library and starts the main application window.
     * 
     * @note throws an std::runtime_error on failure.
     */
    static void InitializeMainApplicationWindow();

public:
    UserInterface() {};
    ~UserInterface();

    static std::unique_ptr<UserInterface>& GetInstance();

    /**
     * @brief Initializes the main application window and all of its UI elements.
     */
    static void Initialize();

    /**
     * @brief Resizes the window.
     */
    static void Resize(uint32_t width, uint32_t height);

    /**
     * @brief Pushes a rectangle of a certain color to the window.
     */
    static void PushRectangle(SDL_Rect rect, uint32_t color);

    /**
     * @brief Sets the entire main application window to white.
     */
    static void ClearScreen();
};

}
