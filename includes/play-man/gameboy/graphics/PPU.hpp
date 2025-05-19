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

#include <play-man/gameboy/graphics/PPUDefines.hpp>
#include <stdint.h>

namespace GameBoy {

    class PPU
    {
    private:
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
         * @brief A bool to keep track if the PPU is in CGB mode or not;
         */
        bool CgbMode;

    public:
        PPU() = delete;
        PPU(bool cgbEnabled);
        ~PPU() = default;

        void SetCgbMode(bool enabled);

        uint8_t ReadByte(uint16_t address);
        void    WriteByte(uint16_t address, uint8_t value);

    };

}
