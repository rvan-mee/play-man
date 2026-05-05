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

#include <play-man/gameboy/ppu/PPUDefines.hpp>
#include <play-man/gameboy/memoryBus/MemoryDefines.hpp>
#include <stdint.h>

namespace GameBoy {

class PPU;

class PixelFetcher
{
    private:

        /**
         * @brief The base class for the Pixel FiFos.
         * 
         * Both the Background and Object FiFos share the same pipeline, the main differences
         * being what memory is accessed and how it is addressed.
         */
        class FiFoBase
        {
        protected:

            /**
             * @brief The Y position inside the current tile being fetched.
             */
            uint8_t fetcherTileY;

            /**
             * @brief The X position inside the current tile being fetched.
             */
            uint8_t fetcherTileX;

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
             * @brief The state of the current fetch.
             * 
             * Every fetch takes 2 T-ticks.
             * 
             * @note It is not very well documented what specifically happens on each tick of the fetch, though it
             * does seem that reads from memory take 1 T-tick. The way we implement the individual cycles is by computing
             * the read address the first tick and then reading the actual value on that address the next tick.
             * 
             * @note https://github.com/LIJI32/SameBoy/blob/master/Core/display.c mentions the read completing
             * on the second tick as well.
             */
            InnerPixelFetchState innerFetchState;

            /**
             * @brief Fetches and stores the tile number from which the pixels should be retrieved.
             */
            virtual void TickTileFetch() = 0;

            /**
             * @brief Fetches the lower byte of the fetch data.
             */
            virtual void TickDataLowFetch() = 0;

            /**
             * @brief This functions the same as the TickDataLow() except the tile address is incremented by 1.
             */
            virtual void TickDataHighFetch() = 0;

            /**
             * @brief Computes the correct pixel values and then attempts to push those entries to the FiFo.
             */
            virtual void TickFiFoPush() = 0;

            /**
             * @brief Pointer to the PPU this FiFo is apart of.
             */
            PPU* ppu;

            /**
             * @brief The Pixel FiFo.
             */
            PixelFiFo   fifo;

        public:

            FiFoBase() = delete;
            FiFoBase(PPU* _ppu);
            virtual ~FiFoBase() = default;

            /**
             * @brief Resets the FiFo and fetch data.
             */
            virtual void Clear();

            /**
             * @brief Performs a T-tick for the FiFo pixel fetcher, used in Mode 3.
             * 
             * The fetcher goes in 4 different steps, where the first 3 steps each take 2 T-ticks.
             * The final step is attempted every dot till it succeeds.
             * 
             *    NumberFetch: Fetches the number of the tile the pixels are taken from.
             *    DataLowFetch: Fetches the lower byte of data.
             *    DataHighFetch: Fetches the higher byte of data.
             *    FiFoPush: Attempts to push the fetched pixels inside the FiFo.
             */
            void TickFetcher();

            /**
             * @brief Returns a reference to the Pixel FiFo.
             */
            PixelFiFo& GetFiFo();

            /**
             * @brief Returns the oldest element and pops it off the FiFo.
             */
            FiFoEntry GetFrontAndPop();

            /**
             * @brief Returns the size of the FiFo.
             */
            size_t Size();
        };

        /**
         * @brief The Background/Window pixel FiFo and fetcher.
         */
        class BackgroundFiFo : public FiFoBase
        {
        private:
            void TickTileFetch() override;
            void TickDataLowFetch() override;
            void TickDataHighFetch() override;
            void TickFiFoPush() override;

            enum class FetchingTileType {
                Window,
                Background,
            };

            /**
             * @brief Whether we are currently fetching background or window tiles.
             */
            FetchingTileType fetchingType;

            /**
             * @brief Whether the Background FiFo is paused and waiting for
             * an Object fetch to be finished.
             */
            bool paused;

            /**
             * @brief The line being fetched inside of the window tile map.
             * 
             * This can be seen as the window's Y coordinate, which is not directly linked
             * to the value inside the WY register. It will only be incremented if a window
             * pixel is rendered on a scanline.
             * 
             * @note For more information:
             * @note https://gbdev.io/pandocs/Scrolling.html?highlight=window%20selects#window
             */
            uint8_t windowLineCounter;

            /**
             * @brief The current pixel within a scanline the background fetcher is fetching.
             */
            uint8_t fetcherX;

            /**
             * @brief Pushes an entire row of 8 pixels into the background FiFo.
             */
            void PushBackgroundPixels(uint8_t lowPixelData, uint8_t highPixelData);

        public:
            BackgroundFiFo() = delete;
            BackgroundFiFo(PPU* _ppu) : PixelFetcher::FiFoBase(_ppu), windowLineCounter(-1) {};
            ~BackgroundFiFo() = default;

            /**
             * @brief When an object fetch is initiated the Background Fetcher is reset to step 1 and paused.
             * Once the object fetch is completed the background fifo fetching is resumed.
             */
            void ResetAndPause();

            /**
             * @brief Resumes the background FiFo after an object fetch has been completed.
             */
            void Continue();

            /**
             * @brief The pixel mixer checks if the window has been reached after every pixel pushed to the LCD.
             * If the window is enabled and is on the current X position, the FiFo is cleared and reset to the first step.
             * 
             * @note This can occur multiple times per scanline: https://gbdev.io/pandocs/Window.html
             */
            void StartWindowFetch();

            /**
             * @brief Returns the x position within a scanline of the current tile being fetched.
             */
            uint8_t GetFetcherX();

            /**
             * @brief Resets the FiFo and its internal states.
             */
            void Clear() override;
        };

        // The FiFos are apart of the PPU, hence the friend.
        friend class BackgroundFiFo;
        BackgroundFiFo  backgroundFiFo;

        /**
         * @brief The Object pixel FiFo and fetcher.
         */
        class ObjectFiFo : public FiFoBase
        {
        private:
            void TickTileFetch() override;
            void TickDataLowFetch() override;
            void TickDataHighFetch() override;
            void TickFiFoPush() override;

            /**
             * @brief Pointer to the Background FiFo this object fifo competes with.
             * We need access to the Background FiFo to pause and continue it.
             */
            BackgroundFiFo* backgroundFiFo;

        public:
            ObjectFiFo() = delete;
            ObjectFiFo(PPU* _ppu, BackgroundFiFo* _backgroundFiFo): PixelFetcher::FiFoBase(_ppu), backgroundFiFo(_backgroundFiFo) {};
            ~ObjectFiFo() = default;
        };

        // The FiFos are apart of the PPU, hence the friend.
        friend class ObjectFiFo;
        ObjectFiFo  objectFiFo;

        /**
         * @brief Pointer to the PPU this pixel fetcher is a part of.
         */
        PPU* ppu;

        /**
         * @brief The x position within a scanline of the current pixel being shifted out.
         */
        uint8_t pixelX;

        /**
         * @brief If a sprite fetch is initiated 
         */
        bool mixerPaused;

        /**
         * @brief The amount of pixels the background gets shifted by at the start of a scanline.
         */
        uint8_t backgroundShift;

        /**
         * @brief The pixel mixer is what pushes an actual pixel to the screen, it decides which
         * FiFos to take the pixel from. 
         */
        void PixelMixerTick();

        /**
         * @brief Readies the FiFos and pixel mixer for an object fetch.
         */
        void StartObjectFetch();

        /**
         * @brief Continues the shifting of pixels to the LCD
         * after an object fetch has been completed.
         */
        void ContinueMixing();

        /**
         * @brief Returns the RGBA value that has to be rendered to the screen for the given
         * object FiFo entry.
         */
        uint32_t ResolveObjectColor(const FiFoEntry& objectEntry);

        /**
         * @brief Returns the RGBA value that has to be rendered to the screen for the given
         * background FiFo entry.
         */
        uint32_t ResolveBackgroundColor(const FiFoEntry& backgroundEntry);

        enum class EntryPriority {
            Object,
            Background,
        };

        /**
         * @brief Returns the type of entry that has to be rendered after having resolved the
         * priority of the entries.
         */
        EntryPriority GetEntryPriority(const FiFoEntry& backgroundEntry, const FiFoEntry& objectEntry);

        /**
         * @brief Mixes the background/window entry with the object entry,
         * pushing the right pixel to the screen.
         */
        void MixPixel(const FiFoEntry& backgroundEntry, const FiFoEntry& objectEntry);

        /**
         * @brief Pushes a pixel to the screen with the values of the given background FiFo entry.
         */
        void PushBackgroundPixel(const FiFoEntry& backgroundEntry);

    public:
        PixelFetcher() = delete;
        PixelFetcher(PPU* _ppu);
        ~PixelFetcher();

        /**
         * @brief Resets the Pixel fetcher to be ready to fetch pixels for a new scanline.
         */
        void ResetForScanline();

        /**
         * @brief Resets the pixel fetcher 
         */
        void ResetVBlank();

        /**
         * @brief Performs a T-tick, internally handling the background/window and object FiFo's
         * 		  and the pushing of pixels to the LCD through the pixel mixer.
         */
        void Tick();

        /**
         * @brief Wether the PixelFetcher is done outputting pixels for the current scanline.
         */
        bool DoneWithScanline();
};

}
