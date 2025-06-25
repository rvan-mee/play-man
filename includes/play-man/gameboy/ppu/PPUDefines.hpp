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

#include <play-man/utility/EnumMacro.hpp>
#include <queue>

namespace GameBoy {

#define PpuStateSeq(x, n) \
    x(n, hBlank, 0)       \
    x(n, vBlank, 1)       \
    x(n, ScanOAM, 2)      \
    x(n, Drawing, 3)      \

CREATE_ENUM_WITH_UTILS(PpuStateSeq, PixelProcessingState);
#undef PpuStateSeq

#define OamScanStateSeq(x, n) \
    x(n, Reading)             \
    x(n, Comparing)           \

CREATE_ENUM_WITH_UTILS(OamScanStateSeq, OamScanState);
#undef OamScanStateSeq

/**
 * @note The inner states of the PPU's FiFo fetchers all take 2 cycles for each state.
 * The way we emulate this is in the first cycle compute the address for the Read/Write and
 * in the second cycle perform the IO instruction since these seem to take a full tick.
 */
#define PpuInnerPixelFetchStateSeq(x, n) \
    x(n, Computing)                      \
    x(n, IO)                             \

CREATE_ENUM_WITH_UTILS(PpuInnerPixelFetchStateSeq, InnerPixelFetchState);
#undef PpuInnerPixelFetchStateSeq

#define PpuFiFoTypeSeq(x, n) \
    x(n, Object)             \
    x(n, BackgroundWindow)   \

CREATE_ENUM_WITH_UTILS(PpuFiFoTypeSeq, FiFoType);
#undef PpuFiFoTypeSeq

#define PixelFetchStateSeq(x, n) \
    x(n, TileFetch)                   \
    x(n, DataLowFetch)                \
    x(n, DataHighFetch)               \
    x(n, Sleep)                       \
    x(n, FiFoPush)                    \

CREATE_ENUM_WITH_UTILS(PixelFetchStateSeq, PixelFetchState);
#undef PixelFetchStateSeq

typedef struct s_FiFoEntry {
    /**
     * @brief The color number between 0 and 3, taken from the tile data. 
     */
    uint8_t color;

    /**
     * @brief on CGB: A value between 0 and 7 representing the palette.
     *        on DMG: Represents the OBP0 or OBP1 palette.
     * 
     * @note Unused for DMG backgroundFiFo entries.
     */
    uint8_t palette;

    /**
     * @brief The OAM index for the object.
     * 
     * @note This value does not exist on DMG mode.
     */
    uint8_t spritePriority;

    /**
     * @brief Stores the bit (7) from the attributes/flags (byte 3) from an OAM entry
     * that specifies the background/window priority.
     * 
     * @note Only relevant for sprites.
     */
    uint8_t backgroundPriority;

    s_FiFoEntry() : color(0), palette(0), spritePriority(0), backgroundPriority(0) {}

    s_FiFoEntry(uint8_t _color, uint8_t _palette, uint8_t _sPriority, uint8_t _bPriority) :
    color(_color), palette(_palette), spritePriority(_sPriority), backgroundPriority(_bPriority) {}

    s_FiFoEntry(s_FiFoEntry& other) { *this = other; }

    s_FiFoEntry(s_FiFoEntry&& other) { *this = other; }

    struct s_FiFoEntry& operator = (const struct s_FiFoEntry& rhs)
    {
        this->color = rhs.color;
        this->palette = rhs.palette;
        this->spritePriority = rhs.spritePriority;
        this->backgroundPriority = rhs.backgroundPriority;
        return *this;
    }

    void clear()
    {
        color = 0;
        palette = 0;
        spritePriority = 0;
        backgroundPriority = 0;
    }

}   FiFoEntry;

/**
 * @brief The amount of entries are pushed to the FiFo per FiFoPush state.
 */
constexpr uint8_t FiFoEntriesPerPush = 8;

/**
 * @brief Some FiFo entry values are unused depending on if they are background/object
 * entries or if we are in CGB mode or not. This define exist to prevent a magic number.
 */
constexpr uint8_t UnusedFiFoEntryValue = 0;

using PixelFiFo = std::queue<FiFoEntry>;

typedef struct s_FiFoFetchData {
    /**
     * @brief This number represents the number of the tile which to fetch the pixels from.
     */
    uint8_t tileNumber;

    /**
     * @brief The lower byte of additional data being fetched.
     */
    uint8_t dataLow;

    /**
     * @brief The upper byte of additional data being fetched.
     */
    uint8_t dataHigh;

    s_FiFoFetchData() : tileNumber(0), dataLow(0), dataHigh(0) {}

    s_FiFoFetchData(uint8_t _tileNumber, uint8_t _dataLow, uint8_t _dataHigh) :
    tileNumber(_tileNumber), dataLow(_dataLow), dataHigh(_dataHigh) {}

    s_FiFoFetchData(s_FiFoFetchData& other) { *this = other; }

    struct s_FiFoFetchData& operator = (const struct s_FiFoFetchData& rhs)
    {
        this->tileNumber = rhs.tileNumber;
        this->dataLow = rhs.dataLow;
        this->dataHigh = rhs.dataHigh;
        return *this;
    }

    void clear()
    {
        tileNumber = 0;
        dataLow = 0;
        dataHigh = 0;
    }
}   FiFoFetchData;

// These are the default values of the registers after the boot rom has ran it course.

constexpr uint8_t DefaultDMAregisterValue = 0x00;
constexpr uint8_t DefaultLCDCregisterValue = 0x00;
constexpr uint8_t DefaultLYregisterValue = 0x00;
constexpr uint8_t DefaultLYCregisterValue = 0x00;
constexpr uint8_t DefaultSTATregisterValue = 0x00;
constexpr uint8_t DefaultSCYregisterValue = 0x00;
constexpr uint8_t DefaultSCXregisterValue = 0x00;
constexpr uint8_t DefaultWYregisterValue = 0x00;
constexpr uint8_t DefaultWXregisterValue = 0x00;
constexpr uint8_t DefaultVRamBankRegisterValue = 0x00;
constexpr uint8_t DefaultBGPregisterValue = 0x00;
constexpr uint8_t DefaultOBP0registerValue = 0x00;
constexpr uint8_t DefaultOBP1registerValue = 0x00;
constexpr uint8_t DefaultBGPIregisterValue = 0x00;
constexpr uint8_t DefaultBCPDregisterValue = 0x00;
constexpr uint8_t DefaultOBPIregisterValue = 0x00;
constexpr uint8_t DefaultOBPDregisterValue = 0x00;

// OAM related values
constexpr uint8_t DefaultAmountOfSelectedObjects = 0;
constexpr uint8_t DefaultCurrentOamScanAddress = 0;
constexpr uint8_t DefaultDotsPassedInScanline = 0;

constexpr PixelProcessingState DefaultStateValue = PixelProcessingState::ScanOAM;
constexpr OamScanState DefaultOamScanState = OamScanState::Reading;

// Drawing related values
constexpr uint8_t DefaultDrawDelay = 0;

// The address range for the video RAM
constexpr uint16_t AddressVramStart = 0x8000;
constexpr uint16_t AddressVramEnd = 0x9FFF;

constexpr uint16_t AddressTileMapStart = 0x9800;
constexpr uint16_t AddressTileMapEnd = 0x9FFF;

constexpr uint16_t AddressTileDataStart = 0x8000;
constexpr uint16_t AddressTileDataEnd = 0x9800;

// The address range for the OAM
constexpr uint16_t AddressOamStart = 0xFE00;
constexpr uint16_t AddressOamEnd = 0xFE9F;

// The address ranges for addressing the PPU registers
constexpr uint16_t AddressLCDC = 0xFF40;
constexpr uint16_t AddressSTAT = 0xFF41;
constexpr uint16_t AddressSCY = 0xFF42;
constexpr uint16_t AddressSCX = 0xFF43;
constexpr uint16_t AddressLY = 0xFF44; // Read only.
constexpr uint16_t AddressLYC = 0xFF45;
constexpr uint16_t AddressDMA = 0xFF46;
constexpr uint16_t AddressBGP = 0xFF47;
constexpr uint16_t AddressOBP0 = 0xFF48;
constexpr uint16_t AddressOBP1 = 0xFF49;
constexpr uint16_t AddressWY = 0xFF4A;
constexpr uint16_t AddressWX = 0xFF4B;
constexpr uint16_t AddressVramBank = 0xFF4F;
constexpr uint16_t AddressBGPI = 0xFF68;
constexpr uint16_t AddressBCPD = 0xFF69;
constexpr uint16_t AddressOBPI = 0xFF6A;
constexpr uint16_t AddressOBPD = 0xFF6B;

/**
 * @brief Some bits of the STAT register can only be read from,
 * this mask masks out the bits that can be written to.
 * 
 * The bits that can be changed represent (in order):
 * 
 */
constexpr uint8_t WriteMaskSTATvalue = 0b01111000;

/**
 * @brief The vram bank can be switched from 0 to 1 so we will only check a single bit.
 */
constexpr uint8_t vRamBankMask = 0b00000001;

/**
 * @brief The size in bytes of a single entry inside the OAM (Object Attribute Memory).
 */
constexpr uint8_t OamEntrySize = 4;

/**
 * @brief The amount of entries inside the OAM (Object Attribute memory).
 */
constexpr uint8_t OamEntryCount = 40;

/**
 * @brief The size of the OAM (Object Attribute Memory) in bytes.
 */
constexpr uint8_t OamSize = OamEntryCount * OamEntrySize;

/**
 * @brief The amount of objects that could be selected for drawing during OAM scan (mode 2).
 */
constexpr uint8_t SelectedObjectsStoreSize = 10;

using ObjectAttributeMemory = std::array<uint8_t, OamSize>;
using SelectedObjectsStoreMemory = std::array<uint8_t, SelectedObjectsStoreSize>;

/**
 * @brief After a DMA transfer is requested there is a small delay (2 M-ticks) before the transfer actually starts.
 */
constexpr uint8_t StartDMATransferDelay = 8;

/**
 * @brief A DMA transfer fills the entire OAM, with this being the count of final byte that should be transferred.
 */
constexpr uint8_t FinalDMATransferByte = 0x9F;

/**
 * @brief The value written to the DMA register need to be multiplied by this offset to get the correct starting value. 
 */
constexpr uint16_t OffsetDMATransfer = 0x100;


/********************************  LCDC register masks  ********************************/

/**
 * @brief The bit inside the LCDC that can toggle the LCD and PPU.
 * 
 * Having this bit set will enable the LCD and PPU.
 * If the bit is unset both are disabled and full access to the VRAM and OAM is possible.
 * 
 * When the screen is disabled the screen will blank, turning white (more so than color #0).
 * Re-enabling the LCD will cause the PPU to instantly start drawing again,
 * but the screen will stay blank during the first frame.
 */
constexpr uint8_t LCDandPPUenableMask = 0b10000000;

/**
 * @brief The bit inside the LCDC that controls which background map
 * the window uses for rendering.
 * 
 * Having this bit set will use the 0x9C00 tilemap.
 * If the bit is unset tilemap 0x9800 is used.
 */
constexpr uint8_t WindowTileMapAreaMask = 0b01000000;

/**
 * @brief The bit inside the LCDC that enables or disables the window.
 * 
 * Having this bit set enables the window.
 * If this bit is unset the window is disabled.
 * 
 * On DMG this bit is overridden by disabling the BackgroundWindowEnablePriorityMask bit.
 */
constexpr uint8_t WindowEnableMask = 0b00100000;

/**
 * @brief The bit inside the LCDC that controls which addressing mode is used
 * for the background and window tiles.
 * 
 * Having this bit enabled will use the PrimaryTileAddressingMethod.
 * If this is unset the SecondaryTileAddressingMethod will be used.
 */
constexpr uint8_t BackgroundWindowTileDataAreaMask = 0b00010000;

/**
 * @brief The bit inside the LCDC that controls which background tile map
 * the background uses for rendering.
 * 
 * Having this bit set will use the 0x9C00 tilemap.
 * If the bit is unset tilemap 0x9800 is used.
 */
constexpr uint8_t BackgroundTilemapAreaMask = 0b00001000;

/**
 * @brief The bit inside the LCDC that controls the size of all objects.
 * 
 * Having this bit set will use objects as 8x16 (vertically stacked) pixels.
 * If the bit is unset an object is seen as 8x8 pixels.
 * 
 * @note When this bit is changed in PPU mode 2 or 3 could cause artifacts.
 */
constexpr uint8_t ObjectSizeMask = 0b00000100;

/**
 * @brief The bit inside the LCDC that controls wether objects are enabled.
 * 
 * Having this bit set will cause objects to be rendered.
 * If this bit is unset objects will not be displayed.
 * 
 * @note This bit can be changed mid scanline.
 */
constexpr uint8_t ObjectEnableMask = 0b00000010;

/**
 * This bit has different effects for CGB or DMG mode.
 * 
 * DMG:
 * Having this bit set will cause the window and background to be rendered normally.
 * If this bit is unset both the background and window will become blank (white)
 * and the WindowEnableMask bit is ignored.
 * 
 * CGB:
 * Having this bit set will cause the window and background to be rendered normally.
 * If this bit is unset the background and window lose their priority in rendering
 * meaning that objects will always be displayed on top, regardless of the priority bits
 * found inside the OAM and BackgroundMapAttributes. 
 */
constexpr uint8_t BackgroundWindowEnablePriorityMask = 0b00000001;

/********************************                       ********************************/


/******************************  LCD STAT register masks  ******************************/

/**
 * @brief If this bit is set the condition LYC == LY is selected for the STAT interrupt.
 */
constexpr uint8_t InterruptSelectLYCMask = 0b01000000;

/**
 * @brief If this bit is set the condition for mode 0 is selected for the STAT interrupt.
 */
constexpr uint8_t InterruptSelectMode2Mask = 0b00100000;

/**
 * @brief If this bit is set the condition for mode 1 is selected for the STAT interrupt.
 */
constexpr uint8_t InterruptSelectMode1Mask = 0b00010000;

/**
 * @brief If this bit is set the condition for mode 2 is selected for the STAT interrupt.
 */
constexpr uint8_t InterruptSelectMode0Mask = 0b00001000;

/**
 * @brief This bit gets set by the PPU when the LY and LYC register are the same.
 * It gets constantly updated by the PPU.
 * 
 * @note This part of the STAT register should be read only.
 */
constexpr uint8_t LYCEqualsLYMask = 0b00000100;

/**
 * @brief These bits indicate the PPU's current status/mode. If all bits
 * are unset then the PPU is disabled.
 * 
 * @note This part of the STAT register should be read only.
 */
constexpr uint8_t PPUModeMask = 0b00000011;

/********************************                       ********************************/


/**
 * @brief The base pointer used for addressing the window and background tiles in VRAM,
 * when the 4th bit inside the LCDC register is set.
 * 
 * @note For addressing objects this is always the base pointer.
 */
constexpr uint16_t PrimaryTileAddressingMethod = 0x8000;

/**
 * @brief The base pointer used for addressing the window and background tiles in VRAM,
 * when the 4th bit inside the LCDC register is unset.
 * 
 * When this mode is enabled a signed addressing is used, meaning offsets -1 to -128
 * are in the 0x8800 to 0x8FFF range and offsets 0 to 127 are in the 0x9000 to 0x97FF range. 
 * 
 * @note For addressing objects the PrimaryTileAddressingMethod is used as the base pointer.
 */
constexpr uint16_t SecondaryTileAddressingMethod = 0x8800;

/**
 * @brief The base address used inside the PPU for fetching the tile number used for
 * rendering the current background/window pixel.
 * 
 * @note A visual overview of the tile map can be found here: https://gbdev.io/pandocs/Memory_Map.html
 */
constexpr uint16_t TileMapBaseAddress = 0x9800;

/**
 * @brief Depending on the state of the LCDC register the way the tile map is addressed
 * can be done with a different base address (0x9C00 instead of 0x9800). This offset is
 * applied to the base address to get there.
 * 
 * @note A visual overview of the tile map can be found here: https://gbdev.io/pandocs/Memory_Map.html
 */
constexpr uint16_t TileMapBaseAddressOffset = 0x400;

/**
 * @brief When calculating the tile's X position inside the FiFo we need to make sure
 * that its value is between 0 and 31 when using the scroll registers, this limiter
 * can be AND with the value to get a wrapping X position.
 */
constexpr uint8_t TilePositionLimiterX = 0x1F;

/**
 * @brief When calculating the tile's Y position inside the FiFo we need to make sure
 * that its value is between 0 and 255 when using the scroll registers, this limiter
 * can be AND with the value to get a wrapping Y position.
 */
constexpr uint8_t TilePositionLimiterY = 0xFF; 

/**
 * @brief The amount of bytes used for all the pixels in a tile.
 * 
 * Each line of a tile consists of 2 bytes, using 2 bits per pixel. 
 */
constexpr uint8_t TileSize = 16;

/**
 * @brief The amount of bytes used for the pixels in a single tile line.
 */
constexpr uint8_t BytesPerTileLine = 2;

/**
 * @brief The pixel width of a tile.
 */
constexpr uint8_t TileWidth = 8;

/**
 * @brief The pixel length of a tile.
 */
constexpr uint8_t TileHeight = 8;

/**
 * @brief The window's topleft X coordinate starts off at WX - 7, so the offset
 * to get the X coordinate to be 0 is this number.
 */
constexpr uint8_t WindowStartOffset = 7;

/**
 * @brief The amount of scanlines that have to be passed when rendering a frame for the PPU to reach
 * the VBlank state (mode 1).
 */
constexpr uint8_t ScanlinesPassedTillVBlank = 144;

/**
 * @brief The amount of scanlines required to render a single frame.
 *
 * @note 0 based indexed.
 */
constexpr uint8_t ScanlinesPerFrame = 153;

/**
 * @brief The amount of pixels that can be found on a single scanline.
 */
constexpr uint8_t PixelsPerScanline = 160;

/**
 * @brief The color values used when displaying DMG pixels. 
 * 
 * 2 separate color types can be used, black and white or green.
 * 
 * @note Can be indexed using 'BlackAndWhitePixelsDMG' and 'GreenPixelsDMG'.
 */
constexpr uint32_t ColorsDMG[2][4] = {
    { // Black and White pixel values
        0xFFFFFFFF, // White
        0xFFD3D3D3, // Light Gray
        0xFF808080, // Dark Gray
        0xFF000000, // Black
    },
    { // Green pixel values
        0xFF9BBC0F, // Lightest Green
        0xFF8BAC0F, // Light Green
        0xFF306230, // Dark Green
        0xFF0F380F, // Darkest Green
    }
};

/**
 * @brief Used to index into the 'ColorsDMG' array to get the desired colors.
 */
constexpr uint8_t BlackAndWhitePixelsDMG = 0;

/**
 * @brief Used to index into the 'ColorsDMG' array to get the desired colors.
 */
constexpr uint8_t GreenPixelsDMG = 1;

/**
 * @brief The mask used to get the shade of the retrieved color ID from the
 * background palette register.
 */
constexpr uint8_t BackgroundShadeMaskDMG = 0b11; 

/**
 * @brief A single shift is not enough to move to the next index within the
 * background palette, to move from one index to the next 2 shifts need to be performed.
 */
constexpr uint8_t BackgroundPaletteIndexShiftSize = 2;

#define PPU_READ_OUT_OF_RANGE "PPU: Trying to read from an address that is not within range"
#define PPU_READ_IN_MODE_3 "PPU: Trying to read from a register inaccessible during PPU mode 3 (drawing)"
#define PPU_READ_IN_CGB_MODE "PPU: Trying to perform a read from a register that is not accessible when the CgbMode is enabled"
#define PPU_READ_IN_NON_CGB_MODE "PPU: Trying to perform a read from a register that is not accessible when the CgbMode is disabled"

#define PPU_WRITE_OUT_OF_RANGE "PPU: Trying to write to an address that is not within range"
#define PPU_WRITE_IN_MODE_3 "PPU: Trying to write to a register inaccessible during PPU mode 3 (drawing)"
#define PPU_WRITE_IN_CGB_MODE "PPU: Trying to perform a write to a register that is not accessible when the CgbMode is enabled"
#define PPU_WRITE_IN_NON_CGB_MODE "PPU: Trying to perform a write to a register that is not accessible when the CgbMode is disabled"

#define PPU_READ_DURING_DMA "PPU: Trying to read from the OAM whilst a DMA transfer is active"
#define PPU_WRITE_DURING_DMA "PPU: Trying to write to the OAM whilst a DMA transfer is active"

}
