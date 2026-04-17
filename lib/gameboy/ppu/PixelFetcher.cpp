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
#include <play-man/graphics/UserInterface.hpp>

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

uint32_t PixelFetcher::ResolveObjectColor(const FiFoEntry& objectEntry)
{
    if (!ppu->CgbMode)
    {
        // These should be filtered out through the mixer
        assert(objectEntry.colorIndex != TransparentColorIndexDMG);
        assert(ppu->LCDCregister & ObjectEnableMask);

        uint8_t pallette;
        if (objectEntry.palette == PalletteValueOBP1)
            pallette = ppu->OBP1register;
        else
            pallette = ppu->OBP0register;

        const uint8_t colorIndex = objectEntry.colorIndex;
        const uint8_t shade = (pallette >> (colorIndex * PaletteIndexShiftSizeDMG)) & PalletteShadeMaskDMG;

        assert(ppu->colorModeDMG == BlackAndWhitePixels || ppu->colorModeDMG == GreenPixels);

        return ColorsDMG[ppu->colorModeDMG][shade];
    }
    else
    {
        assert(false && "CGB colors are not supported yet!");
        return ColorsDMG[ppu->colorModeDMG][WhitePixelIndexDMG];
    }
}

uint32_t PixelFetcher::ResolveBackgroundColor(const FiFoEntry& backgroundEntry)
{
    if (!ppu->CgbMode)
    {
        // In DMG mode, if the LCDC has the window and backgrounds turned off
        // a white pixel must be rendered.
        if (!(ppu->LCDCregister & BackgroundWindowEnablePriorityMask))
            return ColorsDMG[ppu->colorModeDMG][WhitePixelIndexDMG];

        // To get the correct shade from the palette we use the retrieved color ID from
        // the FiFo entry and 'index' into the background palette to get the right shade.
        // Check the BGPregister's comment for more info.
        const uint8_t colorIndex = backgroundEntry.colorIndex;
        const uint8_t shade = (ppu->BGPregister >> (colorIndex * PaletteIndexShiftSizeDMG)) & PalletteShadeMaskDMG;

        assert(ppu->colorModeDMG == BlackAndWhitePixels || ppu->colorModeDMG == GreenPixels);

        return ColorsDMG[ppu->colorModeDMG][shade];
    }
    else
    {
        assert(false && "CGB colors not supported yet!");
        return ColorsDMG[ppu->colorModeDMG][WhitePixelIndexDMG];
    }
}

void PixelFetcher::PushBackgroundPixel(const FiFoEntry& backgroundEntry)
{
    const size_t         scale = ppu->cpu->settings->screenScaleGameBoy;
    Graphics::Rectangle  pixel;

    pixel.height = scale;
    pixel.height = scale;
    pixel.x = mixerX * scale;
    pixel.y = ppu->LYregister * scale;

    pixel.color = ResolveBackgroundColor(backgroundEntry);

    Graphics::UserInterface::PushRectangle(pixel);
}

PixelFetcher::EntryPriority PixelFetcher::GetEntryPriority(const FiFoEntry& backgroundEntry, const FiFoEntry& objectEntry)
{
    if (!ppu->CgbMode)
    {
        if (objectEntry.colorIndex == TransparentColorIndexDMG)
            return EntryPriority::Background;

        if (!(ppu->LCDCregister & ObjectEnableMask))
            return EntryPriority::Background;

        return EntryPriority::Object;
    }
    else
    {
        // https://gbdev.io/pandocs/Tile_Maps.html#bg-to-obj-priority-in-cgb-mode
        assert(false && "CGB mode not yet supported!");
        return EntryPriority::Background;
    }
}

void PixelFetcher::MixPixel(const FiFoEntry& backgroundEntry, const FiFoEntry& objectEntry)
{
    const size_t         scale = ppu->cpu->settings->screenScaleGameBoy;
    Graphics::Rectangle  pixel;

    pixel.height = scale;
    pixel.height = scale;
    pixel.x = mixerX * scale;
    pixel.y = ppu->LYregister * scale;

    if (GetEntryPriority(backgroundEntry, objectEntry) == EntryPriority::Object)
        pixel.color = ResolveObjectColor(objectEntry);
    else
        pixel.color = ResolveBackgroundColor(backgroundEntry);

    Graphics::UserInterface::PushRectangle(pixel);
}

void PixelFetcher::PixelMixerTick()
{
    assert(mixerX <= PixelsPerScanline);

    // If the mixer is paused due to an object fetch not being completed yet, wait
    if (mixerPaused)
        return ;

    // We cannot shift a pixel out to the screen if the background FiFo is empty
    if (backgroundFiFo.Size() == 0)
        return ;

    // Shift a pixel out from the background FiFo, creating a draw delay
    if (backgroundShift != 0)
    {
        assert(mixerX == 0);
        backgroundFiFo.GetFiFo().pop();
        backgroundShift--;
        return;
    }

    // If there is not a pixel in the object fifo, just render the background right away
    // else mix the pixels with their priority/color.
    if (!objectFiFo.Size() == 0)
        PushBackgroundPixel(backgroundFiFo.GetFrontAndPop());
    else
        MixPixel(backgroundFiFo.GetFrontAndPop(), objectFiFo.GetFrontAndPop());
    mixerX++;

    // TODO:
    // Check window fetching
    // if (WX >= backgroundFiFo.xPosition && LCDC & windowEnableBit)
    // backgroundFiFo.startWindowFetching(); // this can happen more than once per scanline: https://gbdev.io/pandocs/Window.html
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
