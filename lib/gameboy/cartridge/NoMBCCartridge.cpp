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

#include <play-man/gameboy/cartridge/NoMBC.hpp>
#include <play-man/logger/Logger.hpp>

/*     Address Ranges     */
static constexpr uint16_t RomAddressRangeStart = 0x0000;
static constexpr uint16_t RomAddressRangeEnd = 0x7FFF;
static constexpr uint16_t RamAddressRangeStart = 0xA000;
static constexpr uint16_t RamAddressRangeEnd = 0xBFFF;


namespace GameBoy {

    NoMBCCartridge::NoMBCCartridge(std::unique_ptr<Rom> rom) : ACartridge(std::move(rom))
    {
        CartridgeType cType = GetType();

        hasRam = false;
        if (cType == CartridgeType::ROM_RAM || cType == CartridgeType::ROM_RAM_BATTERY)
            hasRam = true;
    }


    uint8_t NoMBCCartridge::ReadByte(const uint16_t address)
    {
        if (address >= RomAddressRangeStart && address <= RomAddressRangeEnd)
        {
            // Up to 32kiB was mapped directly to this range.
            // Since we separate all the cartridge's roms automatically into rom banks we have to
            // check if we need to address another bank or not.
            if (address >= RomBankSize)
            {
                if (rom->GetRomBankCount() == 1)
                {
                    LOG_DEBUG(READ_OUT_OF_RANGE);
                    return OpenBusValue;
                }

                return rom->ReadFromBank(1, address - RomBankSize);
            }

            // The address falls within bank 0's range.
            return rom->ReadFromBank(0, address);
        }
        else if (address >= RamAddressRangeStart && address <= RamAddressRangeEnd)
        {
            if (!hasRam)
            {
                LOG_DEBUG("Cartridge: Reading from RAM range but no ram is found on this cartridge");
                return OpenBusValue;
            }

            if (hasRam && ramBanks.size() == 0)
            {
                LOG_ERROR("Cartridge: Cartridge has ram but no ram bank is initialized, returning OpenBusValue");
                return OpenBusValue;
            }

            return ramBanks[0][address - RamAddressRangeStart];
        }
        else
        {
            LOG_DEBUG(READ_OUT_OF_RANGE);
            return OpenBusValue;
        }
    }

    void    NoMBCCartridge::WriteByte(const uint16_t address, const uint8_t value)
    {
        // If this cartridge has no ram writing is useless.
        if (!hasRam)
        {
            LOG_DEBUG("Cartridge: Trying to write to a cartridge without RAM");
            return ;
        }

        if (hasRam && ramBanks.size() == 0)
        {
            LOG_ERROR("Cartridge: Cartridge has ram but no ram bank is initialized");
            return ;
        }

        // This cartridge type only supports one bank of 8 KiB.
        if (address >= RamAddressRangeStart && address <= RamAddressRangeEnd)
        {
            ramBanks[0][address - RamAddressRangeStart] = value;
        }
        else
        {
            LOG_DEBUG(RAM_BANK_INVALID);
        }
    }

}
