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

#include <play-man/gameboy/cartridge/MBC5.hpp>
#include <play-man/logger/Logger.hpp>

/*     Default Register Values     */
static constexpr uint8_t DefaultRomBankNumberLower = 0x01;
static constexpr uint8_t DefaultRomBankNumberUpper = 0b0;
static constexpr uint8_t DefaultRamBankNumber = 0x00;
static constexpr bool    DefaultRamEnabled = false;

/*     Read Address Ranges     */
static constexpr uint16_t   RomBank00RangeStart = 0x0000;
static constexpr uint16_t   RomBank00RangeEnd = 0x3FFF;
static constexpr uint16_t   RomBankedRangeStart = 0x4000;
static constexpr uint16_t   RomBankedRangeEnd = 0x7FFF;

/*     Write Address Ranges     */
static constexpr uint16_t   RamEnableRangeStart = 0x0000;
static constexpr uint16_t   RamEnableRangeEnd = 0x1FFF;
static constexpr uint16_t   RomBankNumberLowerRangeStart = 0x2000;
static constexpr uint16_t   RomBankNumberLowerRangeEnd = 0x2FFF;
static constexpr uint16_t   RomBankNumberUpperRangeStart = 0x3000;
static constexpr uint16_t   RomBankNumberUpperRangeEnd = 0x3FFF;
static constexpr uint16_t   RamBankNumberRangeStart = 0x4000;
static constexpr uint16_t   RamBankNumberRangeEnd = 0x5FFF;

/*     Read/Write Address Ranges     */
static constexpr uint16_t   RamBankedRangeStart = 0xA000;
static constexpr uint16_t   RamBankedRangeEnd = 0xBFFF;

/*     Masks     */
static constexpr uint8_t    RamEnableMask = 0b0000'1111;
static constexpr uint8_t    RomBankNumberUpperMask = 0b0000'0001;
static constexpr uint8_t    EnableRumbleMotorMask = 0b0000'1000;

/*     Misc     */
static constexpr uint8_t    RamEnableValue = 0x0A;

namespace GameBoy
{

    MBC5Cartridge::MBC5Cartridge(std::unique_ptr<Rom> rom) : ACartridge(std::move(rom))
    {
        romBankNumberLowerbits = DefaultRomBankNumberLower;
        romBankNumberUpperbit = DefaultRomBankNumberUpper;
        ramBankNumber = DefaultRamBankNumber;
        ramEnabled = DefaultRamEnabled;

        const CartridgeType cType = GetType();
        hasRumbleMotor = false;
        if (cType == CartridgeType::MBC5_RUMBLE ||
            cType == CartridgeType::MBC5_RUMBLE_RAM ||
            cType == CartridgeType::MBC5_RUMBLE_RAM_BATTERY)
            hasRumbleMotor = true;

        if (hasRumbleMotor)
            LOG_WARNING("Cartridge: Detected a cartridge with a rumble pack which is currently not implemented");
    }

    void    MBC5Cartridge::ActivateRumbleMotor()
    {
        // TODO:
        LOG_WARNING("Cartridge: Trying to activate the rumble motor, currently not implemented");
    }

    void    MBC5Cartridge::DeactivateRumbleMotor()
    {
        // TODO:
        LOG_WARNING("Cartridge: Trying to deactivate the rumble motor, currently not implemented");
    }

    uint8_t MBC5Cartridge::ReadByte(const uint16_t address)
    {
        if (address >= RomBank00RangeStart && address <= RomBank00RangeEnd)
        {
            return rom->ReadFromBank(0, address);
        }
        else if (address >= RomBankedRangeStart && address <= RomBankedRangeEnd)
        {
            uint16_t bank = (romBankNumberUpperbit << 8) | romBankNumberLowerbits;

            if (bank >= rom->GetRomBankCount())
            {
                LOG_DEBUG(ROM_BANK_INVALID);
                return OpenBusValue;
            }

            return rom->ReadFromBank(bank, address - RomBankedRangeStart);
        }
        else if (address >= RamBankedRangeStart && address <= RamBankedRangeEnd)
        {
            if (!ramEnabled)
            {
                LOG_DEBUG(READ_RAM_DISABLED);
                return OpenBusValue;
            }

            return ramBanks[ramBankNumber][address - RamBankedRangeStart];
        }

        LOG_DEBUG(READ_OUT_OF_RANGE);
        return OpenBusValue;
    }

    void    MBC5Cartridge::WriteByte(const uint16_t address, const uint8_t value)
    {
        if (address >= RamEnableRangeStart && address <= RamEnableRangeEnd)
        {
            // Writing a value with 0x0A in its lower 4 bits will enable the RAM. 
            ramEnabled = (value & RamEnableMask) == RamEnableValue;
        }
        else if (address >= RomBankNumberLowerRangeStart && address <= RomBankNumberLowerRangeEnd)
        {
            romBankNumberLowerbits = value;
        }
        else if (address >= RomBankNumberUpperRangeStart && address <= RomBankNumberUpperRangeEnd)
        {
            romBankNumberUpperbit = value & RomBankNumberUpperMask;
        }
        else if (address >= RamBankNumberRangeStart && address <= RamBankNumberRangeEnd)
        {
            ramBankNumber = value;

            // If the cartridge has a rumble pack, the 3rd bit is used to (de)activate the motor.
            if (hasRumbleMotor)
            {
                if (value & EnableRumbleMotorMask)
                    ActivateRumbleMotor();
                else
                    DeactivateRumbleMotor();
            }
        }
        else if (address >= RamBankedRangeStart && address <= RamBankedRangeEnd)
        {
            if (!ramEnabled)
            {
                LOG_DEBUG(WRITE_RAM_DISABLED);
                return ;
            }

            if (ramBankNumber >= GetRamBankCount())
            {
                LOG_DEBUG(RAM_BANK_INVALID);
                return ;
            }

            ramBanks[ramBankNumber][address - RamBankedRangeStart] = value;
        }
        else
        {
            LOG_DEBUG(WRITE_OUT_OF_RANGE);
        }
    }

}
