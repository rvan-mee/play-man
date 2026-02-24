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

#include <play-man/gameboy/ppu/PPU.hpp>

namespace GameBoy {

// *************** Shared Functions ***************

PPU::FiFoBase::FiFoBase(PPU* _ppu) : ppu(_ppu)
{
    assert(_ppu);
    this->Clear();
}

void    PPU::FiFoBase::Clear()
{
    fetcherX = 0;
    fetcherTileX = 0;
    fetcherTileY = 0;
    fetchData.clear();
    tileFetchAddress = 0;
    dataLowFetchAddress = 0;
    dataHighFetchAddress = 0;
    fetchState = PixelFetchState::TileFetch;
    innerFetchState = InnerPixelFetchState::Computing;
    fifo = {}; // clear the queue (no .clear member function)
}

void PPU::FiFoBase::TickFetcher()
{
    switch (fetchState)
    {
        case PixelFetchState::TileFetch: TickTileFetch(); break;
        case PixelFetchState::DataLowFetch: TickDataLowFetch(); break;
        case PixelFetchState::DataHighFetch: TickDataHighFetch(); break;
        case PixelFetchState::FiFoPush: TickFiFoPush(); break;
        default: break;
    }
}

PixelFiFo& PPU::FiFoBase::GetFiFo()
{
    return fifo;
}


// *************** Background FiFo Functions ***************

bool PPU::BackgroundFiFo::RenderingWindow()
{
    // The window's top left X coordinate starts at WX - 7.
    const uint8_t windowX = ppu->WXregister - WindowStartOffset;
    // To know if we are rendering a window a combination of things need to be checked.
    // If the LCDC's 5th bit is enabled and the LY is >= WY and
    // the fetcherX is >= WX we are rendering a window tile.
    const bool windowEnabled = ppu->LCDCregister & WindowEnableMask;
    const bool windowWithinY = ppu->LYregister >= ppu->WYregister;
    const bool windowWithinX = fetcherX >= windowX;
    // TODO: Some docs mention only the check of an X position, not Y, verify which is the case.
    const bool renderingWindowTile = windowEnabled && windowWithinY && windowWithinX;
    return renderingWindowTile;
}

void PPU::BackgroundFiFo::TickTileFetch()
{
    // TEMP to make things compile again, revisiting the PPU after completing the CPU.
    (void) windowHorizontalCondition;
    (void) windowVerticalCondition;

    const uint8_t windowX = ppu->WXregister - WindowStartOffset;
    (void) windowX;

    if (fetcherX >= PixelsPerScanline)
        return ;

    if (innerFetchState == InnerPixelFetchState::Computing)
    {
        // In DMG mode, if the LCDC has the window and backgrounds turned off
        // a white pixel must be rendered.
        if (!ppu->CgbMode && !(ppu->LCDCregister & BackgroundWindowEnablePriorityMask))
        {
            // TODO:
            // Push white pixel into FiFo.
            return ;
        }

        // The window's top left X coordinate starts at WX - 7.
        const bool renderingWindowTile = RenderingWindow();

        // The base address of the tile data map.
        uint16_t tileMapAddress = TileMapBaseAddress;

        // The base address of the map can be changed depending on the state of the LCDC register.
        // The LCDC register contains 2 bits that specify which tile map area to use for
        // rendering both the background and window tiles respectively.
        if (ppu->LCDCregister & BackgroundTilemapAreaMask && !renderingWindowTile)
            tileMapAddress += TileMapBaseAddressOffset;
        else if (ppu->LCDCregister & WindowTileMapAreaMask && renderingWindowTile)
            tileMapAddress += TileMapBaseAddressOffset;

        if (renderingWindowTile)
        {
            // TODO: Which window X&Y value to take here???
            fetcherTileX = fetcherX;
            fetcherTileY = ppu->WYregister;
            windowPixelRendered = true;
        }
        else
        {
            // Rendering a background tile requires some calculations of the X and Y positions,
            // since the scroll registers can change them.
            fetcherTileX = (fetcherX + (ppu->SCXregister / TileWidth)) & TilePositionLimiterX;
            fetcherTileY = (ppu->LYregister + ppu->SCYregister) & TilePositionLimiterY;
        }

        // To get the correct address we take the current base pointer, offset it by the tile's x position
        // now we have to offset using the y position, we divide it by the width of a tile and multiply
        // that value by 32 (since there are 32x32 tiles inside the tile map).
        tileFetchAddress = tileMapAddress + fetcherTileX + ((fetcherTileY / TileWidth) * 32);

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        assert(tileFetchAddress >= AddressTileMapStart && tileFetchAddress <= AddressTileMapEnd);
        fetchData.tileNumber = ppu->ReadByte(tileFetchAddress);

        assert(!ppu->CgbMode && "Not fetching tile attributes for CGB mode yet.");

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::DataLowFetch;
    }
}

void PPU::BackgroundFiFo::TickDataLowFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {
        // The base address can be changed through the LCDC register.
        uint16_t tileMapBaseAddress = PrimaryTileAddressingMethod;
        if (!(ppu->LCDCregister & BackgroundWindowTileDataAreaMask))
            tileMapBaseAddress = SecondaryTileAddressingMethod;

        // Since every pixel line in a sprite consists of 2 bytes (low and high data)
        // we can extract the Y offset by getting the Y of the tile and then multiplying that by 2.
        const uint8_t offsetY = (fetcherTileY % TileHeight) * BytesPerTileLine;
        // TODO: Is this offset correct? https://hacktix.github.io/GBEDG/ppu/#oam-scan-mode-2 mentions a different calculation.

        assert(!ppu->CgbMode && "Cannot perform Y flips since the CGB attributes are not fetched yet.");

        // The way the pixel data address is calculated is done by getting the correct base address for
        // the tile map first, then offsetting that by the tile size amount of times the tile number we have
        // fetched in the previous step and then offsetting that once more by adding the calculated Y offset.
        dataLowFetchAddress = tileMapBaseAddress + ((fetchData.tileNumber * TileSize) + offsetY);

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        assert(dataLowFetchAddress >= AddressTileDataStart && dataLowFetchAddress <= AddressTileDataEnd);
        fetchData.dataLow = ppu->ReadByte(dataLowFetchAddress);

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::DataHighFetch;
    }
}

void PPU::BackgroundFiFo::TickDataHighFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {
        // The second pixel data values are found right after the first ones.
        dataHighFetchAddress = dataLowFetchAddress + 1;

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        assert(dataHighFetchAddress >= AddressTileDataStart && dataHighFetchAddress <= AddressTileDataEnd);
        fetchData.dataHigh = ppu->ReadByte(dataHighFetchAddress);

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::FiFoPush;
    }
}

void PPU::BackgroundFiFo::PushBackgroundPixels(uint8_t lowPixelData, uint8_t highPixelData)
{
    for (uint8_t i = 0; i < FiFoEntriesPerPush; i++)
    {
        // The color value (0-3) is calculated by taking 1 bit from the low and 1 bit from the high data.
        // We start with the left-most bit and take the bits on the right from there.
        const uint8_t shiftAmount = 7 - i;
        const uint8_t lowBit = (lowPixelData >> shiftAmount) & 1;
        const uint8_t highBit = (highPixelData >> shiftAmount) & 1;
        const uint8_t colorIndex = lowBit | (highBit << 1);
        fifo.push({colorIndex, UnusedFiFoEntryValue, UnusedFiFoEntryValue, UnusedFiFoEntryValue}); 
    }
}

void PPU::BackgroundFiFo::TickFiFoPush()
{
    // Pixels are only pushed to the background FiFo if there is space.
    // The FiFo has a size of 16 and attempts to push 8 at a time, meaning the current
    // FiFo size has to be smaller than 8 for this many pixels to fit.
    if (fifo.size() > 8)
        return ;

    if (!ppu->CgbMode)
        PushBackgroundPixels(fetchData.dataLow, fetchData.dataHigh);
    else
        assert(false && "Unable to push CGB pixels to the FiFo at the moment.");

    innerFetchState = InnerPixelFetchState::Computing;
    fetchState = PixelFetchState::TileFetch;
    fetcherX += TileWidth;
}

void PPU::BackgroundFiFo::UpdateWindow()
{
    // const bool windowEnabled = ppu->LCDCregister & WindowEnableMask;
    // const bool windowOnScanline = fetcherX >= ppu->WXregister;
}

void PPU::BackgroundFiFo::UpdateWindowLineCounter()
{
    if (windowPixelRendered)
        windowLineCounter++;
}

void PPU::BackgroundFiFo::ResetWindowLineCounter()
{
    windowLineCounter = 0;
    windowPixelRendered = false;
    windowRenderingActivated = false;
}

// *************** Object FiFo Functions ***************

void PPU::ObjectFiFo::TickTileFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::DataLowFetch;
    }
}

void PPU::ObjectFiFo::TickDataLowFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::DataHighFetch;
    }
}

void PPU::ObjectFiFo::TickDataHighFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::FiFoPush;
    }
}

void PPU::ObjectFiFo::TickFiFoPush()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::TileFetch;
    }
}

}
