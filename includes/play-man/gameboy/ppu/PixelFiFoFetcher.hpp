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

#include <play-man/gameboy/cpu/Cpu.hpp>
#include <play-man/gameboy/ppu/PPUDefines.hpp>
#include <play-man/gameboy/memory/MemoryDefines.hpp>
#include <stdint.h>
#include <array>

namespace GameBoy {

template<FiFoType typeFiFo>
class PixelFiFoFetcher
{
private:

    /**
     * @brief In background/window mode the address of the tile number, in object mode the address
     * of the object inside OAM.
     */
    uint16_t tileFetchAddress;

    /**
     * @brief Depending on the FiFo type, the address of the lower byte of fetched data in VRAM or the OAM.
     */
    uint16_t dataLowFetchAddress;

    /**
     * @brief Depending on the FiFo type, the address of the upper byte of fetched data in VRAM or the OAM.
     */
    uint16_t dataHighFetchAddress;

    /**
     * @brief Each fetch state takes 2 T-ticks to complete.
     * 
     * @note After the 3rd state, 'DataHighFetch' is completed for the first time in a scanline
     * the Background Fetcher is reset, causing a delay of a total of 12 T-ticks before the
     * Background FiFo is filled with pixel data. 
     * 
     * @note The last state 'FiFoPush' will only happen when the background FiFo is completely
     * empty, it will loop till it is empty.
     */
    PixelFetchState fetchState;

    /**
     * @brief The data currently being fetched.
     */
    FiFoFetchData fetchData;

    /**
     * @brief The FiFo entries that are fetched.
     */
    FiFoEntry currentFetchEntries[8];

    /**
     * @brief The state of the current fetch.
     * 
     * Every fetch takes 2 T-ticks.
     * 
     * @note It is not very well documented what specifically happens on each tick of the fetch, though it
     * does seem that reads from memory take 1 T-tick. The way we implement the individual cycles is by computing
     * the read address the first tick and then reading the actual value on that address the next tick.
     */
    InnerPixelFetchState innerFetchState;

    /**
     * @brief The amount of cycles the fetcher should be stalling.
     */
    uint8_t sleepCycles;

    /**
     * @brief Fetches and stores the tile number from which the pixels should be retrieved.
     */
    void TickTileFetch()
    {
        if (innerFetchState == InnerPixelFetchState::Computing)
        {
            innerFetchState = InnerPixelFetchState::Reading;
        }
        else if (innerFetchState == InnerPixelFetchState::Reading)
        {
            innerFetchState = InnerPixelFetchState::Computing;
        }
    }

    /**
     * @brief Fetches the lower byte of the fetch data.
     */
    void TickDataLowFetch()
    {
        if (innerFetchState == InnerPixelFetchState::Computing)
        {
            innerFetchState = InnerPixelFetchState::Reading;
        }
        else if (innerFetchState == InnerPixelFetchState::Reading)
        {
            innerFetchState = InnerPixelFetchState::Computing;
        }
    }

    /**
     * @brief This functions the same as the TickDataLow() except the tile address is incremented by 1.
     */
    void TickDataHighFetch()
    {
        if (innerFetchState == InnerPixelFetchState::Computing)
        {
            innerFetchState = InnerPixelFetchState::Reading;
        }
        else if (innerFetchState == InnerPixelFetchState::Reading)
        {
            innerFetchState = InnerPixelFetchState::Computing;
        }
    }

    /**
     * @brief Sleeps for sleepCycles amount of T-ticks.
     */
    void TickSleep()
    {
        if (sleepCycles)
            sleepCycles--;

        // For the next tick, be in the correct mode.
        if (sleepCycles == 0)
            fetchState = PixelFetchState::FiFoPush;
    }

    /**
     * @brief Computes the correct pixel values and then attempts to push those entries to the FiFo.
     */
    void TickFiFoPush()
    {
        if (innerFetchState == InnerPixelFetchState::Computing)
        {
            if constexpr (typeFiFo == FiFoType::BackgroundWindow)
            {
                if (fifo.size() != 0)
                    return ;
            }

            for (uint8_t i = 0; i < FiFoEntriesPerPush; i++)
            {

            }

            innerFetchState = InnerPixelFetchState::Reading;
        }
        else if (innerFetchState == InnerPixelFetchState::Reading)
        {

            for (uint8_t i = 0; i < FiFoEntriesPerPush; i++)
                fifo.push(currentFetchEntries[i]);

            innerFetchState = InnerPixelFetchState::Computing;
            fetchState = PixelFetchState::TileFetch;
        }
    }

    /**
     * @brief The pixel FiFo.
     * 
     * The FiFo can hold 16 pixels at a time, where 8 pixels are required for the rendering to take place.
     */
    PixelFiFo   fifo;

    /**
     * @brief Pointer to the main CPU to get access to the memory bus and oam.
     */
    Cpu* cpu;

    /**
     * @brief Wether the PPU is in cgb mode or not.
     */
    bool cgbMode;

public:
    PixelFiFoFetcher() = delete;
    PixelFiFoFetcher(Cpu* _cpu, bool _cgbEnabled) : cpu(_cpu), cgbMode(_cgbEnabled) {};
    ~PixelFiFoFetcher() = default;

    /**
     * @brief Resets the FiFo and fetch data.
     */
    void Clear()
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

    /**
     * @brief Performs a T-tick for the FiFo pixel fetcher, used in Mode 3.
     * 
     * The fetcher goes in 5 different steps, where the first 4 steps each take 2 T-ticks.
     * The final step is attempted every dot till it succeeds.
     * 
     *    NumberFetch: Fetches the number of the tile the pixels are taken from.
     *    DataLowFetch: Fetches the lower byte of data.
     *    DataHighFetch: Fetches the higher byte of data.
     *    Sleep: Does as it says, nothing.
     *    FiFoPush: Attempts the push the fetched pixels inside the FiFo.
     */
    void TickFetcher(uint8_t currentX)
    {
        switch (fetchState)
        {
            case PixelFetchState::TileFetch: TickTileFetch(); break;
            case PixelFetchState::DataLowFetch: TickDataLowFetch(); break;
            case PixelFetchState::DataHighFetch: TickDataHighFetch(); break;
            case PixelFetchState::Sleep: TickSleep(); break;
            case PixelFetchState::FiFoPush: TickFiFoPush(); break;
            default: break;
        }
    }

    /**
     * @brief Returns a reference to the Pixel FiFo.
     */
    PixelFiFo& GetFiFo()
    {
        return fifo;
    }

    /**
     * @brief Enables or disables the CGB mode.
     */
    void    SetCgbMode(bool enabled)
    {
        cgbMode = enabled;
    }

};


}
