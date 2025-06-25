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
#include <play-man/gameboy/memory/MemoryDefines.hpp>
#include <stdint.h>
#include <array>

namespace GameBoy {

// Forward declaration of the CPU class.
class Cpu;

class PPU
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
             * @brief The Y position of the current tile being fetched.
             */
            uint8_t fetcherTileY;

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
             * @brief The amount of cycles the fetcher should be stalling.
             */
            uint8_t sleepCycles;

            /**
             * @brief Fetches and stores the tile number from which the pixels should be retrieved.
             */
            virtual void TickTileFetch(uint8_t scanlineX) = 0;

            /**
             * @brief Fetches the lower byte of the fetch data.
             */
            virtual void TickDataLowFetch() = 0;

            /**
             * @brief This functions the same as the TickDataLow() except the tile address is incremented by 1.
             */
            virtual void TickDataHighFetch() = 0;

            /**
             * @brief Sleeps for sleepCycles amount of T-ticks.
             */
            void TickSleep();

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
             * The fetcher goes in 5 different steps, where the first 4 steps each take 2 T-ticks.
             * The final step is attempted every dot till it succeeds.
             * 
             *    NumberFetch: Fetches the number of the tile the pixels are taken from.
             *    DataLowFetch: Fetches the lower byte of data.
             *    DataHighFetch: Fetches the higher byte of data.
             *    Sleep: Does as it says, nothing.
             *    FiFoPush: Attempts the push the fetched pixels inside the FiFo.
             */
            void TickFetcher(uint8_t currentX);

            /**
             * @brief Returns a reference to the Pixel FiFo.
             */
            PixelFiFo& GetFiFo();
        };

        /**
         * @brief The Background pixel FiFo and fetcher.
         */
        class BackgroundFiFo : public FiFoBase
        {
        private:
            void TickTileFetch(uint8_t scanlineX) override;
            void TickDataLowFetch() override;
            void TickDataHighFetch() override;
            void TickFiFoPush() override;

            /**
             * @brief Pushes an entire row of 8 pixels into the background FiFo.
             */
            void PushBackgroundPixels(uint8_t lowPixelData, uint8_t highPixelData);

            /**
             * @brief Checks if the current tile being rendered is a
             * window tile or background tile.
             * 
             * @return True if the tile is a window tile.
             */
            bool RenderingWindow(uint8_t scanlineX);

        public:
            BackgroundFiFo() = delete;
            BackgroundFiFo(PPU* _ppu) : PPU::FiFoBase(_ppu) {};
            ~BackgroundFiFo() = default;
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
            void TickTileFetch(uint8_t scanlineX) override;
            void TickDataLowFetch() override;
            void TickDataHighFetch() override;
            void TickFiFoPush() override;

        public:
            ObjectFiFo() = delete;
            ObjectFiFo(PPU* _ppu): PPU::FiFoBase(_ppu) {};
            ~ObjectFiFo() = default;
        };

        // The FiFos are apart of the PPU, hence the friend.
        friend class ObjectFiFo;
        ObjectFiFo  objectFiFo;

        /**
         * @brief Writing to this register will start a DMA transfer from ROM or RAM to the OAM.
         * 
         * The written value specifies the transfer source address divided by 0x100.
         * 
         * @note DMA: Direct Memory Access
         */
        uint8_t DMAregister;


        /*     LCD registers     */

        /**
         * @brief The LCD Control Register.
         * 
         * The bits inside this register toggle which elements are displayed and the way they are.
         */
        uint8_t LCDCregister;

        /**
         * @brief The current horizontal line (Y coordinate).
         * 
         * LY can hold any value from 0 to 153, with values from 144 to 153 indicating the VBlank period.
         */
        uint8_t LYregister;

        /**
         * @brief Register used to compare against the LY register.
         * 
         * When both registers contain identical values, the LYC==LY flag is set inside the STAT register.
         */
        uint8_t LYCregister;

        /**
         * @brief Increments the LY register and then compares the LY register to the LYC register
         * and sets the correct bit inside the STAT register.
         */
        void IncrementLY();

        /**
         * @brief Resets the LY register to 0 and does then compares the LY and LYC registers.
         */
        void ResetLY();

        /**
         * @brief Compares the LY and LYC register and updates the STAT register.
         */
        void CompareLYC();

        /**
         * @brief Register used to keep track of the status of the LCD.
         * 
         * Each bit inside this register represents the 
         * 
         * Bit   -   Read/Write   -   Data
         *  0    -   R            -   PPU mode. The current status of the PPU (0 if PPU is disabled).
         *  1    -   R            -   PPU mode. The current status of the PPU (0 if PPU is disabled).
         *  2    -   R            -   LYC==LY. Enabled when LYC and LC are equal, constantly updated. 
         *  3    -   R/W          -   Sets the mode 0 condition for the STAT interrupt.
         *  4    -   R/W          -   Sets the mode 1 condition for the STAT interrupt.
         *  5    -   R/W          -   Sets the mode 2 condition for the STAT interrupt.
         *  6    -   R/W          -   Sets the LYC int for the STAT interrupt.
         *  7    -   -            -   Unused.
         */
        uint8_t STATregister;

        /**
         * @brief Updates the STAT register to report the current PPU mode in bits 0 and 1.
         */
        void UpdateStatMode();

        /**
         * @brief Background Viewport Y Coordinate.
         * 
         * This register specifies the top-left Y coordinates of the visible 160×144 pixel area
         * within the 256×256 pixels background map. 
         */
        uint8_t SCYregister;

        /**
         * @brief Background Viewport X Coordinate.
         * 
         * This register specifies the top-left X coordinates of the visible 160×144 pixel area
         * within the 256×256 pixels background map. 
         */
        uint8_t SCXregister;

        /**
         * @brief Window 7 Coordinate.
         * 
         * If the window is enabled it will be visible when the X coordinate is in the range
         * of 0 to 166 and the Y coordinate is in the range of 0 to 143.
         */
        uint8_t WYregister;

        /**
         * @brief Window X Coordinate Plus 7.
         * 
         * If the window is enabled it will be visible when the X coordinate is in the range
         * of 0 to 166 and the Y coordinate is in the range of 0 to 143.
         * 
         * @note WX values 0 and 166 are unreliable due to hardware bugs.
         * @note If WX is set to 0, the window will “stutter” horizontally
         * when SCX changes (depending on SCX % 8).
         * @note If WX is set to 166, the window will span the entirety of the following scanline.
         */
        uint8_t WXregister;

        /**
         * @brief Background Palette Data.
         * 
         * This register assigns the shades of grey to the color indices
         * for the background and window tiles.
         * 
         * Each index consists of 2 bits to keep track of the color that that index represents:
         * 
         *  Bits     -     Index    |
         *  0 & 1    -     ID 0     |
         *  2 & 3    -     ID 1     |
         *  4 & 5    -     ID 2     |
         *  6 & 7    -     ID 3     |
         * 
         * Since each index contains 2 bits these are the values and the colors they can represent:
         * 
         *  Value    -     Color       |
         *  0        -     White       |
         *  1        -     Light Gray  |
         *  2        -     Dark Gray   |
         *  3        -     Black       |     
         * 
         * @note Used in non-CGB mode only, In CGB mode CGB palette memory is used instead. 
         */
        uint8_t BGPregister;

        /**
         * @brief These register assign the gray shades to the color indexes of the OBJs that use
         * the corresponding palette.
         * 
         * These registers work in the exact same way as the BGPregister, apart from the fact that
         * ID 0 is transparent for OBJs.
         * 
         * @note Used in non-CGB mode only.
         */
        uint8_t OBP0register;
        uint8_t OBP1register;

        /**
         * @brief Background Palette Index.
         * 
         * Used to access one of 64 bytes inside the CGB's background palette RAM.
         * The memory cannot be accessed directly, only through indexing it with this register,
         * and then accessing it through the BCPDregister.
         * 
         * Layout:
         * Bits 0 through 5 contain the address.
         * Bit 6 is unused.
         * Bit 7 is used to enable or disable the auto-increment of the address.
         * 
         * Auto-increment:
         * When enabled (bit set to 1) writing to this register will increment the address
         * bits by 1. If the PPU is in mode 3 writing will fail, but the increment will
         * still succeed.
         * 
         * @note Used in CGB mode only.
         * @note Also referred to as the Background Color Specification register.
         * @note This register can be accessed outside the VBlank and HBlank modes.
         */
        uint8_t BGPIregister;

        /**
         * @brief Background Color Palette Data.
         * 
         * Allows reading and writing to the CGBs background palette memory,
         * addressed using the BGPIregister.
         * 
         * Each color is stored as little-endian RGB555:
         * Bits 0 through 4: Red intensity.
         * Bits 5 through 9: Green intensity.
         * Bits 10 through 14: Blue intensity. 
         * 
         * @note Used in CGB mode only.
         * @note Also referred to as Background Palette Data.
         */
        uint8_t BCPDregister;

        /**
         * @brief OBJ Palette Index.
         * 
         * Used to access one of 64 bytes inside the CGB's OBJ Palette Memory.
         * The memory cannot be accessed directly, only through indexing it with this register,
         * and then accessing it through the OBPDregister.
         * 
         * Layout:
         * Bits 0 through 5 contain the address/index.
         * Bit 6 is unused.
         * Bit 7 is used to enable or disable the auto-increment of the address.
         * 
         * Auto-increment:
         * When enabled (bit set to 1) writing to this register will increment the address
         * bits by 1. If the PPU is in mode 3 writing will fail, but the increment will
         * still succeed.
         *  
         * @note Used in CGB mode only.
         * @note Also referred to as the OBJ Color Palette Specification (OCPS) register.
         */
        uint8_t  OBPIregister;

        /**
         * @brief OBJ Palette Data.
         * 
         * This register allows reads and writes into the CGB OBJ Palette Memory.
         * The addressed location is set inside the OBPIregister.
         * 
         * @note Used in CGB mode only.
         * @note Also referred to as the OBJ Color Palette Data (OCPD) register.
         * @note Data cannot be read or written when the PPU is set to mode 3.
         * @note Color #0 inside an OBJ is never used as it is always transparent,
         * it is fine to write garbage values or to leave it uninitialized.
         */
        uint8_t OBPDregister;

        /**
         * @brief The selected vram bank.
         * 
         * Can be switched from 0 to 1.
         * 
         * @note Used in CGB mode only.
         */
        uint8_t vRamBankRegister;

        /**
         * @brief The state the PPU is currently in.
         * 
         * Possible states:
         *  hBlank  - 0
         *  vBlank  - 1
         *  ScanOAM - 2
         *  Drawing - 3
         */
        PixelProcessingState    state;

        /**
         * @brief A bool to keep track if the PPU is in CGB mode or not.
         */
        bool CgbMode;

        /**
         * @brief A bool to keep track if there is currently an active DMA transfer.
         */
        bool DMATransferActive;

        /**
         * @brief The amount of T-ticks remaining before the next DMA tick can commence.
         */
        uint8_t CyclesRemainingDMA;

        /**
         * @brief The amount of bytes transferred in the current DMA transfer.
         */
        uint8_t BytesDMATransferred;

        /**
         * @brief Initializes the data for the DMA transfer to begin.
         * 
         * @note The address where the transfer starts from should be set inside the DMAregister beforehand.
         */
        void StartDmaTransfer();

        /**
         * @brief A pointer to the CPU to access all other modules.
         */
        Cpu* cpu;

        /**
         * @brief Initializes the vram banks.
         * 
         * Will always allocate for 2 banks, so CGB switching is possible.
         */
        void InitVram();

        /**
         * @brief The video ram, in CGB mode it consists of 2 banks. 
         */
        MemoryBanks  vRam;

        /**
         * @brief The Object Attribute Memory.
         * 
         * This block of memory contains the extra information (attributes) of the displayed sprites (objects).
         * The GameBoy PPU can display up to 40 movable sprites, consisting of 8x8 or 8x16 pixels.
         * Due to a hardware limitation only 10 sprites could be displayed per scanline.
         * The sprites themselves are located at the address 0x8000 - 0x8FFF inside the VRAM.
         * 
         * An entry inside the OAM consists of 4 bytes:
         * 
         * Byte 0: The Y position on the screen.
         * 
         * Byte 1: The X position on the screen.
         * 
         * Byte 2: The tile index which can have 2 different addressing modes, depending if bit 2 of the LCDC register is set:
         *       - Unset: 8x8 mode, uses the value as an index inside range 0x8000 - 0x8FFF which can be banked in CGB mode.
         *       - Set:   8x16 mode, The same area is interpereted as a series of 8x8 sprites where 2 sprites form an object.
         *                           This byte is pointing to the index of the first (top) sprite.
         * 
         * Byte 3: The attributes and flags:
         *       Bit 0: CGB mode only - CGB pallette - Which of OBP0-7 to use for the color.
         *       Bit 1: CGB mode only - CGB pallette - Which of OBP0-7 to use for the color.
         *       Bit 2: CGB mode only - CGB pallette - Which of OBP0-7 to use for the color.
         *       Bit 3: CGB mode only - VRAM bank - Unset: Use bank 0 - Set: use bank 1.
         * 
         *       Bit 4: Non CGB mode only - DMG palette - Unset: Use palette OBP0 - Set: use palette OBP1.
         * 
         *       Bit 5: X flip   - Unset: Normal - Set: This object is horizontally mirrored/flipped.
         *       Bit 6: Y flip   - Unset: Normal - Set: This object is vertically mirrored/flipped. 
         *       Bit 7: Priority - Unset: None - Set: Background and window color indices 1-3 are drawn over this object.
         * 
         */
        ObjectAttributeMemory   oam;

        /**
         * @brief The objects that are selected during OAM Scan (mode 2) are stored here.
         * 
         * This array contains the index into the oam where the sprites to be drawn are stored.
         */
        SelectedObjectsStoreMemory   selectedObjects;

        /**
         * @brief The Amount of objects that are currently selected to be drawn, stored inside selectedObjects.
         * 
         * The maximum amount of objects to be stored per scanline is 10.
         */
        uint8_t amountOfSelectedObjects;

        /**
         * @brief This represents what the OAM has to handle in its current cycle.
         */
        OamScanState oamScanState;

        /**
         * @brief The current byte being scanned in mode 2.
         */
        uint8_t currentOamScanAddress;

        /**
         * @brief The byte retrieved by the last OAM scan tick, used for comparing with the LY register.
         */
        uint8_t currentScanSpriteY;

        /**
         * @brief The amount of Dots that have passed for the current scanline. 
         */
        uint16_t dotsPassedInScanline;

        /**
         * @brief Readies the PPU for the next regular (non VBlank) scanline.
         */
        void ResetForNextScanline();

        /**
         * @brief Performs a T-tick for the OAM Scan (Mode 2).
         * 
         * The OAM scan takes a total of 80 Dots to complete (2 per object), within this time
         * it checks the entire OAM for (up to 10) sprites to use on the current scanline
         * by comparing their Y values with the LY register.
         * 
         * @note I have not been able to find good documentation about why it takes 2 Dots
         * per sprite so the way we are emulating it is by, in the first Dot, reading the
         * Y value from the OAM and the next Dot comparing it and saving it's oam index.
         * 
         * @note Documentation about how sprites are selected varies, some saying the X position
         * is taken into account and some say it is not, we follow the pandocs for now:
         * 
         * https://gbdev.io/pandocs/OAM.html#object-attribute-memory-oam
         * vs
         * https://hacktix.github.io/GBEDG/ppu/#oam-scan-mode-2
         */
        void TickOamScan();

        /**
         * @brief During Mode 3 there are multiple things that can cause a delay for drawing the next pixel.
         * 
         * On Mode 3 start, there is a delay of (SCXregister % 8) dots.
         * After the last non-window pixel is written a 6 dot delay is incurred whilst the background fetcher is setup.
         * Each drawn object causes a 6 to 11 dot delay.
         */
        uint8_t drawDelay;

        /**
         * @brief The color mode used for displaying DMG pixels.
         * 
         * @note Values should be 'BlackAndWhitePixelsDMG' or 'GreenPixelsDMG'.
         */
        uint8_t colorModeDMG;

        /**
         * @brief Returns the color to be displayed on the screen based on the pixel data.
         */
        uint32_t GetBackgroundPixelColor(FiFoEntry pixelData);

        /**
         * @brief Returns the color to be displayed on the screen based on the pixel data.
         */
        uint32_t GetObjectPixelColor(FiFoEntry pixelData);

        /**
         * @brief chooses and returns the pixel color that should be presented on the LCD
         * based on what is inside the Background FiFo and Sprite/Object FiFo.
         */
        uint32_t PixelMixer();

        /**
         * @brief The internal X position used by the fetchers.
         * 
         * This value is incremented after every pixel transfer.
         */
        uint8_t scanlineX;

        /**
         * @brief Performs a T-tick that simulates pushing a pixel to the LCD, depending on the
         * contents of both the FiFos.
         */
        void TickPixelTransferLCD();

        /**
         * @brief Performs a T-tick for the Drawing Pixel (Mode 3). 
         */
        void TickDrawingPixel();

        /**
         * @brief Performs a T-tick for the Horizontal Blank (Mode 0).
         */
        void TickHorizontalBlank();

        /**
         * @brief Performs a T-tick for the Vertical Blank (Mode 1).
         */
        void TickVerticalBlank();

        public:
        PPU() = delete;
        PPU(bool cgbEnabled, Cpu* _cpu);
        ~PPU() = default;

        void SetCgbMode(bool enabled);
        
        uint8_t ReadByte(uint16_t address);
        void    WriteByte(uint16_t address, uint8_t value);
        
        /**
         * @brief Simulates a T-tick for the DMA transfer, if active.
         */
        void TickDMA();
    
        /**
         * @brief Simulates a T-tick for the PPU. 
         */
        void TickPPU();
    };

}

#include <play-man/gameboy/cpu/Cpu.hpp>
