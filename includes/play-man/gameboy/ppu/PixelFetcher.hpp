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
             * @brief The internal X position within a scanline. Gets updated by +8 after every tile fetch.
             */
            uint8_t fetcherX;

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
            void Clear();

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

			FetchingTileType fetchingType;

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
             * @brief Pushes an entire row of 8 pixels into the background FiFo.
             */
            void PushBackgroundPixels(uint8_t lowPixelData, uint8_t highPixelData);

        public:
            BackgroundFiFo() = delete;
            BackgroundFiFo(PPU* _ppu) : PixelFetcher::FiFoBase(_ppu) {};
            ~BackgroundFiFo() = default;

            /**
             * @brief The background FiFo has to be flushed and restarted if the window gets enabled
             * and the current fetcherX is within the window.
             */
            void UpdateWindow();

            /**
             * @brief Updates the internal 'windowLineCounter' after a scanline
             * with a window pixel has been pushed.
             */
            void UpdateWindowLineCounter();

            /**
             * @brief Resets the window line counter and flags.
             */
            void ResetWindowLineCounter();

			/**
			 * @brief When an object fetch is initiated the Background Fetcher is reset to step 1 and paused.
			 */
			void ResetAndPause();

			/**
			 * @brief After an object fetch is done, the Background fetcher continued.
			 */
			void Continue();
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

        public:
            ObjectFiFo() = delete;
            ObjectFiFo(PPU* _ppu): PixelFetcher::FiFoBase(_ppu) {};
            ~ObjectFiFo() = default;
        };

        // The FiFos are apart of the PPU, hence the friend.
        friend class ObjectFiFo;
        ObjectFiFo  objectFiFo;

		/**
		 * @brief
		 */
		PPU* ppu;

		/**
		 * @brief
		 */
		uint8_t fetcherX;
		
		/**
		 * @brief
		 */
		uint8_t mixerX; 
		
		/**
		 * @brief
		 */
		void PixelMixerTick();

	public:
		PixelFetcher() = delete;
		PixelFetcher(PPU* _ppu);
		~PixelFetcher();

		/**
		 * @brief 
		 */
		void Reset();

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
