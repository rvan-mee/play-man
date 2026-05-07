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

PixelFetcher::FiFoBase::FiFoBase(PPU* _ppu) : ppu(_ppu)
{
    assert(_ppu);
    this->Clear();
}

void PixelFetcher::FiFoBase::AdvanceFetcherState()
{
    switch (innerFetchState)
    {
        case (InnerPixelFetchState::IO):
        {
            innerFetchState = InnerPixelFetchState::Computing;
            switch (fetchState)
            {
            case (PixelFetchState::TileFetch):
                fetchState = PixelFetchState::DataLowFetch;
                break;
            case (PixelFetchState::DataLowFetch):
                fetchState = PixelFetchState::DataHighFetch;
                break;
            case (PixelFetchState::DataHighFetch):
                fetchState = PixelFetchState::FiFoPush;
                break;
            case (PixelFetchState::FiFoPush):
                fetchState = PixelFetchState::TileFetch;
                break;
            default:
                assert(false && "Invalid fetch state!");
                break;
            }
            break ;
        }
        case (InnerPixelFetchState::Computing):
        {
            innerFetchState = InnerPixelFetchState::Computing;
            break;
        }
        default:
        {
            assert(false && "Invalid inner fetch state!");
            break;
        }
    }
}

void    PixelFetcher::FiFoBase::Clear()
{
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

void PixelFetcher::FiFoBase::TickFetcher()
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

PixelFiFo& PixelFetcher::FiFoBase::GetFiFo()
{
    return fifo;
}

FiFoEntry PixelFetcher::FiFoBase::GetFrontAndPop()
{
    FiFoEntry front = fifo.front();

    fifo.pop();
    return front;
}

size_t PixelFetcher::FiFoBase::Size()
{
    return fifo.size();
}


// *************** Background FiFo Functions ***************

void PixelFetcher::BackgroundFiFo::Clear()
{
    FiFoBase::Clear();
    fetchingType = FetchingTileType::Background;
    paused = false;
    fetcherTileColumn = 0;
}

void PixelFetcher::BackgroundFiFo::StartWindowFetch()
{
    // Update the internal fetching type, reset the fetcher and start fetching window tiles.
    innerFetchState = InnerPixelFetchState::Computing;
    fetchState = PixelFetchState::TileFetch;
    fetchingType = FetchingTileType::Window;
    fetcherTileX = 0;
    windowLineCounter++;
    fifo = {}; // clear the FiFo
}

void PixelFetcher::BackgroundFiFo::TickTileFetch()
{
    // If an object FiFo fetch is started the background FiFo is paused till that fetch is completed.
    if (paused)
        return ;

    if (innerFetchState == InnerPixelFetchState::Computing)
    {
        // If the window gets disabled, the tile fetch gets completed with the next one being 
        // back to a background tile fetch. To re-enable the window, the WX trigger needs to happen
        // again within the PixelFetcher.
        if (fetchingType == FetchingTileType::Window && !(ppu->LCDCregister & WindowEnableMask))
            fetchingType = FetchingTileType::Background;

        // The base address of the tile data map.
        uint16_t tileMapAddress = TileMapBaseAddress;

        // The base address of the map can be changed depending on the state of the LCDC register.
        // The LCDC register contains 2 bits that specify which tile map area to use for
        // rendering both the background and window tiles respectively.
        if (fetchingType == FetchingTileType::Background && ppu->LCDCregister & BackgroundTilemapAreaMask)
            tileMapAddress += TileMapBaseAddressOffset;
        else if (fetchingType == FetchingTileType::Window && ppu->LCDCregister & WindowTileMapAreaMask)
            tileMapAddress += TileMapBaseAddressOffset;

        if (fetchingType == FetchingTileType::Window)
        {
            // fetcherTileColumn gets reset to 0 when the window gets enabled.
            // LineCounter is incremented every time the window gets enabled.
            fetcherTileX = fetcherTileColumn & TilePositionLimiterX;
            fetcherTileY = windowLineCounter & TilePositionLimiterY;
        }
        else
        {
            // Rendering a background tile requires some calculations of the X and Y positions,
            // since the scroll registers can change them.
            fetcherTileX = (fetcherTileColumn + (ppu->SCXregister / TileWidth)) & TilePositionLimiterX;
            fetcherTileY = (ppu->LYregister + ppu->SCYregister) & TilePositionLimiterY;
        }

        // To get the correct address we take the current base pointer, offset it by the tile's x position
        // now we have to offset using the y position, we divide it by the width of a tile and multiply
        // that value by 32 (since there are 32x32 tiles inside the tile map).
        tileFetchAddress = tileMapAddress + fetcherTileX + ((fetcherTileY / SingleTileHeight) * TilesPerMapLine);

        AdvanceFetcherState();
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        assert(tileFetchAddress >= AddressTileMapStart && tileFetchAddress <= AddressTileMapEnd);
        fetchData.tileNumber = ppu->InternalReadByte(tileFetchAddress);

        assert(!ppu->CgbMode && "Not fetching tile attributes for CGB mode yet.");

        AdvanceFetcherState();
    }
}

void PixelFetcher::BackgroundFiFo::TickDataLowFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {
        // The base address can be changed through the LCDC register.
        uint16_t tileMapBaseAddress = PrimaryTileAddressingMethod;
        if (!(ppu->LCDCregister & BackgroundWindowTileDataAreaMask))
            tileMapBaseAddress = SecondaryTileAddressingMethod;

        // Since every pixel line in a sprite consists of 2 bytes (low and high data)
        // we can extract the Y offset by getting the Y of the tile and then multiplying that by 2.
        const uint8_t offsetY = (fetcherTileY % SingleTileHeight) * BytesPerTileLine;

        assert(!ppu->CgbMode && "Cannot perform Y flips since the CGB attributes are not fetched yet.");

        // The way the pixel data address is calculated is done by getting the correct base address for
        // the tile map first, then offsetting that by the tile size amount of times the tile number we have
        // fetched in the previous step and then offsetting that once more by adding the calculated Y offset.
        dataLowFetchAddress = tileMapBaseAddress + ((fetchData.tileNumber * TileSize) + offsetY);

        AdvanceFetcherState();
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        assert(dataLowFetchAddress >= AddressTileDataStart && dataLowFetchAddress <= AddressTileDataEnd);
        fetchData.dataLow = ppu->InternalReadByte(dataLowFetchAddress);

        AdvanceFetcherState();
    }
}

void PixelFetcher::BackgroundFiFo::TickDataHighFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {
        // The second pixel data values are found right after the first ones.
        dataHighFetchAddress = dataLowFetchAddress + 1;

        AdvanceFetcherState();
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        assert(dataHighFetchAddress >= AddressTileDataStart && dataHighFetchAddress <= AddressTileDataEnd);
        fetchData.dataHigh = ppu->InternalReadByte(dataHighFetchAddress);

        AdvanceFetcherState();
    }
}

void PixelFetcher::BackgroundFiFo::PushBackgroundPixelsDMG(uint8_t lowPixelData, uint8_t highPixelData)
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

void PixelFetcher::BackgroundFiFo::TickFiFoPush()
{
    // Pixels are only pushed to the background FiFo if there is space.
    if (fifo.size() != 0)
        return ;

    if (!ppu->CgbMode)
        PushBackgroundPixelsDMG(fetchData.dataLow, fetchData.dataHigh);
    else
        assert(false && "Unable to push CGB pixels to the FiFo at the moment.");

    AdvanceFetcherState();
    fetcherTileColumn++;
}

void PixelFetcher::BackgroundFiFo::ResetAndPause()
{
    fetchState = PixelFetchState::TileFetch;
    innerFetchState = InnerPixelFetchState::Computing;
    paused = true;
}

void PixelFetcher::BackgroundFiFo::Continue()
{
    paused = false;
}

// *************** Object FiFo Functions ***************

void PixelFetcher::ObjectFiFo::TickTileFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {

        AdvanceFetcherState();
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        AdvanceFetcherState();
    }
}

void PixelFetcher::ObjectFiFo::TickDataLowFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {

        AdvanceFetcherState();
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        assert(dataLowFetchAddress >= AddressTileDataStart && dataLowFetchAddress <= AddressTileDataEnd);
        fetchData.dataLow = ppu->InternalReadByte(dataLowFetchAddress);

        AdvanceFetcherState();
    }
}

void PixelFetcher::ObjectFiFo::TickDataHighFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {
        // The second pixel data values are found right after the first ones.
        dataHighFetchAddress = dataLowFetchAddress + 1;

        AdvanceFetcherState();
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {
        assert(dataHighFetchAddress >= AddressTileDataStart && dataHighFetchAddress <= AddressTileDataEnd);
        fetchData.dataHigh = ppu->InternalReadByte(dataHighFetchAddress);

        AdvanceFetcherState();
    }
}

void PixelFetcher::ObjectFiFo::TickFiFoPush()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {

        AdvanceFetcherState();
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {

        AdvanceFetcherState();
        backgroundFiFo->Continue();
        pixelFetcher->ContinueMixing();
    }
}

}
