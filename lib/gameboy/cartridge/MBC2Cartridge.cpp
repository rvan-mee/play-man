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

#include <play-man/gameboy/cartridge/MBC2.hpp>
#include <play-man/logger/Logger.hpp>

/*     Address Ranges Defines     */
static constexpr uint16_t RomBank00Start = 0x0000;
static constexpr uint16_t RomBank00End = 0x3FFF;
static constexpr uint16_t RomBankedStart = 0x4000;
static constexpr uint16_t RomBankedEnd = 0x7FFF;
static constexpr uint16_t RamStart = 0xA000;
static constexpr uint16_t RamEnd = 0xA1FF;
static constexpr uint16_t RamEchoesStart = 0xA200;
static constexpr uint16_t RamEchoesEnd = 0xBFFF;
static constexpr uint16_t RamEnableOrBankNumberStart = 0x0000;
static constexpr uint16_t RamEnableOrBankNumberEnd = 0x3FFF;

/*     Control Register values     */
static constexpr bool DefaultRamEnabled = false;
static constexpr uint8_t DefaultRomBankNumber = 01;

/*     Masks     */
static constexpr uint16_t Bit8Mask = 0b00000001'00000000;
static constexpr uint16_t Lower9BitsMask = 0b00000001'11111111;
static constexpr uint8_t  Lower4BitsMask = 0b0000'1111;

/*     Misc     */
static constexpr uint8_t  RamEnabledValue = 0x0A;

namespace GameBoy {

    MBC2Cartridge::MBC2Cartridge(std::unique_ptr<Rom> rom) : ACartridge(std::move(rom))
    {
        ramEnabled = DefaultRamEnabled;
        romBankNumber = DefaultRomBankNumber;
    }

    uint8_t MBC2Cartridge::ReadByte(const uint16_t address)
    {
        if (address >= RomBank00Start && address <= RomBank00End)
        {
            return rom->ReadFromBank(0, address);
        }
        else if (address >= RomBankedStart && address <= RomBankedEnd)
        {
            if (romBankNumber >= rom->GetRomBankCount())
            {
                LOG_DEBUG(ROM_BANK_INVALID);
                return OpenBusValue;
            }

            return rom->ReadFromBank(romBankNumber, address - RomBankedStart);
        }
        else if (address >= RamStart && address <= RamEnd)
        {
            if (!ramEnabled)
            {
                LOG_DEBUG(READ_RAM_DISABLED);
                return OpenBusValue;
            }

            return ram[address - RamStart];
        }
        else if (address >= RamEchoesStart && address <= RamEchoesEnd)
        {
            if (!ramEnabled)
            {
                LOG_DEBUG(READ_RAM_DISABLED);
                return OpenBusValue;
            }

            // Only the bottom 9 bits of the address are used here, meaning the
            // access repeats aka 'echoes'.
            LOG_DEBUG("Cartridge: Reading from MBC2's 'echoed' ram");
            return ram[address & Lower9BitsMask];
        }

        LOG_DEBUG(READ_OUT_OF_RANGE);
        return OpenBusValue;
    }

    void    MBC2Cartridge::WriteByte(const uint16_t address, const uint8_t value)
    {
        if (address >= RamEnableOrBankNumberStart && address <= RamEnableOrBankNumberEnd)
        {
            // The 8th bit of the address determines if we should change the ramEnabled state
            // or if we have to change the romBankNumber.
            //
            // If the bit is enabled we change the romBankNumber.
            // If it is disabled the ramEnable register is accessed.
            if (address & Bit8Mask)
            {
                // The lower 4 bits specify what bank to access.
                // If bank 00 is written then it will select the default rom bank 01. 
                romBankNumber = value & Lower4BitsMask;
                if (romBankNumber == 00)
                    romBankNumber = DefaultRomBankNumber;
            }
            else
            {
                // Ram can be enabled by writing a value containing 0x0A in its lower 4 bits.
                // Any other value will disable it.
                ramEnabled = ((value & Lower4BitsMask) == RamEnabledValue);
            }
        }
        else if (address >= RamStart && address <= RamEnd)
        {
            if (!ramEnabled)
            {
                LOG_DEBUG(WRITE_RAM_DISABLED);
                return ;
            }

            ram[address - RamStart] = value;
        }
        else if (address >= RamEchoesStart && address <= RamEchoesEnd)
        {
            if (!ramEnabled)
            {
                LOG_DEBUG(WRITE_RAM_DISABLED);
                return ;
            }

            // Only the bottom 9 bits of the address are used here, meaning the
            // access repeats aka 'echoes'.
            LOG_DEBUG("Cartridge: Writing to MBC2's 'echoed' ram");
            ram[address & Lower9BitsMask] = value;
        }
        else
        {
            LOG_ERROR(WRITE_OUT_OF_RANGE);
            assert(false);
        }
    }

}

