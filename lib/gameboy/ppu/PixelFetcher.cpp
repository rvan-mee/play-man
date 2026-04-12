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

#include <play-man/gameboy/ppu/PixelFetcher.hpp>
#include <play-man/gameboy/ppu/PPU.hpp>

namespace GameBoy {

    
PixelFetcher::PixelFetcher(PPU* _ppu) : backgroundFiFo(_ppu), objectFiFo(_ppu, &backgroundFiFo), ppu(_ppu)
{
    mixerX = 0;
    backgroundShift = 0;
    mixerPaused = false;
}

void PixelFetcher::ResetForScanline()
{
    backgroundFiFo.Clear();
    objectFiFo.Clear();
    mixerPaused = false;
    mixerX = 0;

    backgroundShift = ppu->SCXregister % TileWidth;
}

void PixelFetcher::PixelMixerTick()
{
    assert(mixerX <= PixelsPerScanline);

    // If the mixer is paused due to an object fetch not being completed yet, wait
    if (mixerPaused)
        return ;

    // We cannot shift a pixel out to the screen if the background FiFo is empty
    if (backgroundFiFo.GetFiFo().size() == 0)
        return ;

    // Shift a pixel out from the background FiFo, creating a draw delay
    if (backgroundShift != 0)
    {
        assert(mixerX == 0);
        backgroundFiFo.GetFiFo().pop();
        backgroundShift--;
        return;
    }

    // TODO:
    // In DMG mode, if the LCDC has the window and backgrounds turned off
    // a white pixel must be rendered.
    // if (!ppu->CgbMode && !(ppu->LCDCregister & BackgroundWindowEnablePriorityMask))
    // {
        // Discard pixel in FiFo, use blank pixel instead.
        // Objects can still be rendered on top.
    // }

    // TODO:
    // Check priority of background / object
    // Get pixel color
    // Push color to screen
    mixerX++;

    // TODO:
    // Check window fetching
    // if (WX >= backgroundFiFo.xPosition)
    // backgroundFiFo.startWindowFetching();
}

void PixelFetcher::StartObjectFetch()
{
    mixerPaused = true;
    backgroundFiFo.ResetAndPause();
}

void PixelFetcher::ContinueMixing()
{
    mixerPaused = false;
    backgroundFiFo.Continue();
}

void PixelFetcher::Tick()
{
    backgroundFiFo.TickFetcher();
    objectFiFo.TickFetcher();
    PixelMixerTick();
}

bool PixelFetcher::DoneWithScanline()
{
    return mixerX >= PixelsPerScanline;
}

}
