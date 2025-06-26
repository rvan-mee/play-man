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

#include <play-man/gameboy/cpu/CycleDefines.hpp>
#include <play-man/graphics/UserInterface.hpp>
#include <play-man/gameboy/ppu/PPU.hpp>
#include <play-man/logger/Logger.hpp>

namespace GameBoy {

PPU::PPU(bool cgbEnabled, Cpu* _cpu) :
        backgroundFiFo(this), objectFiFo(this), CgbMode(cgbEnabled), cpu(_cpu)
{
    // TODO:
    // Find the correct default values.
    DMAregister = DefaultDMAregisterValue;
    LCDCregister = DefaultLCDCregisterValue;
    LYregister = DefaultLYregisterValue;
    LYCregister = DefaultLYCregisterValue;
    STATregister = DefaultSTATregisterValue;
    SCYregister = DefaultSCYregisterValue;
    SCXregister = DefaultSCXregisterValue;
    WYregister = DefaultWYregisterValue;
    WXregister = DefaultWXregisterValue;
    vRamBankRegister = DefaultVRamBankRegisterValue;
    BGPregister = DefaultBGPregisterValue;
    OBP0register = DefaultOBP0registerValue;
    OBP1register = DefaultOBP1registerValue;
    BGPIregister = DefaultBGPIregisterValue;
    BCPDregister = DefaultBCPDregisterValue;
    OBPIregister = DefaultOBPIregisterValue;
    OBPDregister = DefaultOBPDregisterValue;

    // OAM related values
    amountOfSelectedObjects = DefaultAmountOfSelectedObjects;
    currentOamScanAddress = DefaultCurrentOamScanAddress;
    dotsPassedInScanline = DefaultDotsPassedInScanline;
    oamScanState = DefaultOamScanState;

    // Drawing related values
    drawDelay = DefaultDrawDelay;
    colorModeDMG = GreenPixelsDMG;

    state = DefaultStateValue;

    InitVram();
}

void    PPU::SetCgbMode(bool enabled)
{
    CgbMode = enabled;
}

void    PPU::InitVram()
{
    // Initializes 2 banks, incase we want to switch to CGB mode.
    vRam.resize(2);
    for (uint8_t bank = 0; bank < 2; bank++)
        vRam[bank].resize(VRamBankSize);
}

void PPU::CompareLYC()
{
    if (LYregister == LYCregister)
        STATregister |= LYCEqualsLYMask;
    else
        STATregister &= ~LYCEqualsLYMask;
}

void PPU::ResetLY()
{
    LYregister = 0;
    CompareLYC();
}

void PPU::IncrementLY()
{
    LYregister++;
    CompareLYC();
}

void PPU::UpdateStatMode()
{
    STATregister = (STATregister & ~PPUModeMask) | (GetEnumAsValue(state) & PPUModeMask);
}

void PPU::ResetForNextScanline()
{
    state = PixelProcessingState::ScanOAM;
    oamScanState = OamScanState::Reading;
    amountOfSelectedObjects = 0;
    dotsPassedInScanline = 0;
    currentOamScanAddress = 0;
}

void PPU::TickOamScan()
{
    switch (oamScanState)
    {
    case OamScanState::Reading:
    {
        currentScanSpriteY = oam[currentOamScanAddress];
        oamScanState = OamScanState::Comparing;
        break;
    }
    case OamScanState::Comparing:
    {
        uint8_t currentObjectAddress = currentOamScanAddress;
        // Increment the address and set the state to reading
        currentOamScanAddress += OamEntrySize;
        oamScanState = OamScanState::Reading;

        // Cannot select more than 10 object per scanline
        if (amountOfSelectedObjects == SelectedObjectsStoreSize)
            break ;

        // The first 16 Y values are considered off-screen, the OAM scan starts at Y 16 since this
        // height value is the first scanline's height.
        const uint8_t currentY = LYregister + 16;
        // If the Object Size bit is set inside the LCDC register objects are seen as 16 pixels high
        const uint8_t spriteHeight = (LCDCregister & ObjectSizeMask) ? 16 : 8;

        // Check if the current sprite is visible on this scanline
        if (currentY >= currentScanSpriteY && currentY < currentScanSpriteY + spriteHeight)
        {
            selectedObjects[amountOfSelectedObjects] = currentObjectAddress;
            amountOfSelectedObjects++;
        }
        break;
    }
    default:
    {
        LOG_FATAL("PPU: Invalid OAM Scan State?");
        assert(false);
    }
    }

    // If we have completed the OAM scan switch the PPU mode to drawing.
    if (dotsPassedInScanline == DotsInMode2)
    {
        state = PixelProcessingState::Drawing;

        // The start of Mode 3 has a draw delay depending on the value inside SCX
        // Both the Pixel FiFos are cleared as well.
        drawDelay = SCXregister % 8;
        backgroundFiFo.Clear();
        objectFiFo.Clear();
    }
}

uint32_t PPU::GetBackgroundPixelColor(FiFoEntry pixelData)
{
    if (!CgbMode)
    {
        // To get the correct shade from the palette we use the retrieved color ID from
        // the FiFo entry and 'index' into the background palette to get the right shade.
        // Check the BGPregister's comment for more info.
        const uint8_t colorIndex = pixelData.color;
        const uint8_t shade = (BGPregister >> (colorIndex * BackgroundPaletteIndexShiftSize)) & BackgroundShadeMaskDMG;

        assert(colorModeDMG == BlackAndWhitePixelsDMG || colorModeDMG == GreenPixelsDMG);
        assert(shade >= 0 && shade <= 4);

        return ColorsDMG[colorModeDMG][shade];
    }
    else
    {
        assert(false && "Cannot get CGB colors.");
    }
    return 0xFFFFFFFF;
}

uint32_t PPU::GetObjectPixelColor(FiFoEntry pixelData)
{
    (void) pixelData;
    if (!CgbMode)
    {
        assert(false && "Sprites are not implemented yet");
    }
    else
    {
        assert(false && "Cannot get CGB colors.");
    }
    return 0xFFFFFFFF;
}

uint32_t PPU::PixelMixer()
{
    FiFoEntry backgroundEntry = backgroundFiFo.GetFiFo().front();
    backgroundFiFo.GetFiFo().pop();

    if (objectFiFo.GetFiFo().empty())
        return GetBackgroundPixelColor(backgroundEntry);

    FiFoEntry objectEntry = objectFiFo.GetFiFo().front();
    objectFiFo.GetFiFo().pop();

    // Decide the priority:
    // TODO: Set the priority rules
    bool backgroundPriority = true;

    if (backgroundPriority)
        return GetBackgroundPixelColor(backgroundEntry);
    return GetObjectPixelColor(objectEntry);
}


void PPU::TickPixelTransferLCD()
{
    // If the background FiFo is empty no pixel is pushed
    // and we have to wait till there are pixels in it.
    if (backgroundFiFo.GetFiFo().empty())
        return ;

    const size_t scale = cpu->settings->screenScaleGameBoy;
    uint32_t pixelColor = PixelMixer();
    SDL_Rect pixel;

    pixel.h = scale;
    pixel.w = scale;
    pixel.x = scanlineX * scale;
    pixel.y = LYregister * scale;

    Graphics::UserInterface::PushRectangle(pixel, pixelColor);

    // Move to the next pixel.
    scanlineX++;
    if (scanlineX == PixelsPerScanline)
    {
        state = PixelProcessingState::hBlank;
        scanlineX = 0;
    }
}

void PPU::TickDrawingPixel()
{
    if (drawDelay)
    {
        drawDelay--;
        return;
    }

    // Start of the scanline
    if (scanlineX == 0)
    {
        // TODO: Shift pixels using SCX, add delay
    }

    backgroundFiFo.TickFetcher(scanlineX);
    objectFiFo.TickFetcher(scanlineX);
    TickPixelTransferLCD();
}

void PPU::TickHorizontalBlank()
{
    // Move to the next scanline if we have reached te end of the current one
    if (dotsPassedInScanline == DotsPerScanline)
    {
        IncrementLY();
        ResetForNextScanline();
        // If we have passed all the scanlines meant for drawing, move to the next PPU mode.
        if (LYregister == ScanlinesPassedTillVBlank)
            state = PixelProcessingState::vBlank;
    }
}

void PPU::TickVerticalBlank()
{
    // The LY register starts reporting 0 in the middle of the final scanline
    if (LYregister == ScanlinesPerFrame && dotsPassedInScanline == DotsResetLY)
        ResetLY();

    // Moves to the next scanline if we have reached te end of the current one
    if (dotsPassedInScanline == DotsPerScanline)
    {
        // If we are at the end of the final scanline for a frame, the LY register will
        // report back 0 meaning we can start working on the next frame again
        //
        // Else we will move on with the next VBlank scanline by incrementing LY 
        if (LYregister == 0)
            ResetForNextScanline();
        else
            IncrementLY();
        dotsPassedInScanline = 0;
    }
}

void PPU::TickPPU()
{
    // TODO: Handle PPU disabling through the LCDC register.
    // After enabling the register it should wait for a frame.
    if (!(LCDCregister & LCDandPPUenableMask))
    {
        assert(false && "PPU: Trying to tick the PPU whilst it should be disabled");
    }

    // Increase the cycles done for this scanline.
    dotsPassedInScanline += Dot;

    switch (state)
    {
    case PixelProcessingState::hBlank:
        TickHorizontalBlank();
        break;
    case PixelProcessingState::vBlank:
        TickVerticalBlank();
        break;    
    case PixelProcessingState::ScanOAM:
        TickOamScan();
        break;
    case PixelProcessingState::Drawing:
        TickDrawingPixel();
        break;
    default:
        LOG_ERROR("PPU: Encountered an unknown Pixel Processing State? whilst trying to tick the PPU");
        assert(false);
    }

    // Update the STAT register's PPU mode bits with the current mode
    UpdateStatMode();
}

}
