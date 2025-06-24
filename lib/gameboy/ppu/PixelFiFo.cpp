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
    sleepCycles = 0;
    fetchData.clear();
    tileFetchAddress = 0;
    dataLowFetchAddress = 0;
    dataHighFetchAddress = 0;
    fetchState = PixelFetchState::TileFetch;
    innerFetchState = InnerPixelFetchState::Computing;
    fifo = {}; // clear the queue (no .clear member function)

    // clear any fetched entries
    for (uint8_t i = 0; i < FiFoEntriesPerPush; i++)
        currentFetchEntries[i].clear();
}

void    PPU::FiFoBase::TickSleep()
{
    if (sleepCycles)
        sleepCycles--;

    // To be in the correct mode for the next tick.
    if (sleepCycles == 0)
        fetchState = PixelFetchState::FiFoPush;
}

void PPU::FiFoBase::TickFetcher(uint8_t currentX)
{
    switch (fetchState)
    {
        case PixelFetchState::TileFetch: TickTileFetch(currentX); break;
        case PixelFetchState::DataLowFetch: TickDataLowFetch(); break;
        case PixelFetchState::DataHighFetch: TickDataHighFetch(); break;
        case PixelFetchState::Sleep: TickSleep(); break;
        case PixelFetchState::FiFoPush: TickFiFoPush(); break;
        default: break;
    }
}

PixelFiFo& PPU::FiFoBase::GetFiFo()
{
    return fifo;
}


// *************** Background FiFo Functions ***************

void PPU::BackgroundFiFo::TickTileFetch(uint8_t scanlineX)
{
    (void) scanlineX;
    if (innerFetchState == InnerPixelFetchState::Computing)
    {
        // In DMG mode, if the LCDC has windows and backgrounds turned off
        // a white pixel must be rendered.
        if (!ppu->CgbMode && !(ppu->LCDCregister & BackgroundWindowEnablePriorityMask))
        {
            // TODO:
            // Push white pixel into FiFo.
            return ;
        }

        // We have to decide if we are rendering a Background or a Window tile.
        // This is done by checking a combination of things, if the LCDC's 5th bit
        // is enabled and the LY is >= WY and the scanlineX is >= WX we are
        // rendering a window tile.
        const bool windowEnabled = ppu->LCDCregister & WindowEnableMask;
        const bool windowWithinY = ppu->LYregister >= ppu->WYregister;
        const bool windowWithinX = scanlineX >= ppu->WXregister;
        const bool renderWindowTile = windowEnabled && windowWithinY && windowWithinX;

        if (renderWindowTile)
        {

        }
        else
        {

        }

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::DataLowFetch;
    }
}

void PPU::BackgroundFiFo::TickDataLowFetch()
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

void PPU::BackgroundFiFo::TickDataHighFetch()
{
    if (innerFetchState == InnerPixelFetchState::Computing)
    {

        innerFetchState = InnerPixelFetchState::IO;
    }
    else if (innerFetchState == InnerPixelFetchState::IO)
    {

        innerFetchState = InnerPixelFetchState::Computing;
        fetchState = PixelFetchState::Sleep;
    }
}

void PPU::BackgroundFiFo::TickFiFoPush()
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

// *************** Object FiFo Functions ***************

void PPU::ObjectFiFo::TickTileFetch(uint8_t scanlineX)
{
    (void) scanlineX;
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
        fetchState = PixelFetchState::Sleep;
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
