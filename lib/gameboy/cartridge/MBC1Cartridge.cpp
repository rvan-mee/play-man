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

#include <play-man/gameboy/cartridge/MBC1.hpp>
#include <play-man/logger/Logger.hpp>

/*          Write Address Defines          */
constexpr uint16_t RamEnableAddressStart = 0x0000;
constexpr uint16_t RamEnableAddressEnd = 0x1FFF;
constexpr uint16_t RomBankNumberAddressStart = 0x2000;
constexpr uint16_t RomBankNumberAddressEnd = 0x3FFF;
constexpr uint16_t RamBankNumberAddressStart = 0x4000;
constexpr uint16_t RamBankNumberAddressEnd = 0x5FFF;
constexpr uint16_t BankingModeAddressStart = 0x6000;
constexpr uint16_t BankingModeAddressEnd = 0x7FFF;

/*          Read Address Defines          */
constexpr uint16_t RomAddressStart = 0x0000;
constexpr uint16_t RomAddressEnd = 0x3FFF;
constexpr uint16_t RomBankedAddressStart = 0x4000;
constexpr uint16_t RomBankedAddressEnd = 0x7FFF;
constexpr uint16_t RamBankedAddressStart = 0xA000;
constexpr uint16_t RamBankedAddressEnd = 0xBFFF;

/*          Control Registers Defines          */
constexpr bool DefaultRamEnabled = false;
constexpr uint8_t DefaultSelectedRomBank = 0x01;
constexpr uint8_t DefaultBankingModeSelect = 0x00;
constexpr uint8_t DefaultSecondarySelectedBankBits = 0x00;
constexpr uint8_t DefaultBankRegisterBitCount = 0x05;
constexpr uint8_t MBC1MBankRegisterBitCount = 0x04;

// TODO: figure out the correct value: https://gbdev.io/pandocs/MBC1.html
// mentions that 0xFF is often the value, but not guaranteed.
constexpr uint8_t OpenBusValue = 0xFF;

constexpr uint32_t AmountOfBanksToBeLargeRom = 64;
constexpr uint32_t RomBankCountMBC1M = 64;

/**
 * @brief 
 */
constexpr uint8_t SimpleBankingMode = 0x00;
/**
 * @brief Advanced banking mode allows the use of the secondary banking register
 * to 
 */
constexpr uint8_t AdvancedBankingMode = 0x01;

namespace GameBoy
{

    MBC1Cartridge::MBC1Cartridge(std::unique_ptr<Rom> rom) : ACartridge(std::move(rom))
    {
        secondarySelectedBankRegister = DefaultSecondarySelectedBankBits;
        bankRegisterBitCount = DefaultBankRegisterBitCount;
        selectedBankRegister = DefaultSelectedRomBank;
        bankingModeSelect = DefaultBankingModeSelect;
        ramEnabled = DefaultRamEnabled;

        InitRomBanks();
        InitRamBanks();

        cartIsMBC1M = CheckMBC1M();
        if (cartIsMBC1M)
            bankRegisterBitCount = MBC1MBankRegisterBitCount;
    }

    static void CopyRomBank(const std::vector<uint8_t>& romData, std::vector<uint8_t>& cartData, uint32_t bankNumber)
    {
        uint32_t    currentBankStart = bankNumber * RomBankSize;
        uint32_t    currentBankEnd = (bankNumber + 1) * RomBankSize;

        if (currentBankStart > romData.size())
            return ;

        if (currentBankEnd > romData.size())
            currentBankEnd = romData.size();
        
        cartData.insert(cartData.begin(), romData.begin() + currentBankStart, romData.begin() + currentBankEnd);
    }

    void MBC1Cartridge::InitRomBanks()
    {
        uint8_t romBankCount = rom->GetRomBankCount();
        
        romBanks.resize(romBankCount);
        for (uint32_t i = 0; i < romBankCount; i++)
        {
            romBanks[i].resize(RomBankSize);
            CopyRomBank(rom->GetData(), romBanks[i], i);
        }
        rom->ClearData();
    }

    void MBC1Cartridge::InitRamBanks()
    {
        uint8_t ramBankCount = rom->GetRamBankCount();

        ramBanks.resize(ramBankCount);
        for (uint32_t i = 0; i < ramBankCount; i++)
            ramBanks[i].resize(RamBankSize);
    }

    uint8_t MBC1Cartridge::RomBankMask()
    {
        // Since the banks are indexed starting at 0 we can simply take the
        // amount of banks needed and do -1 to get the right mask e.g:
        // - 16 banks results in a mask of 0x00001111 that represents 15 in decimal.
        // - 128 banks results in a mask of 0x01111111 that represents 127 in decimal.
        //
        // Since the MBC1 cart supports up to a max of 2 MiB (128 banks) we do not have
        // to worry about a bank count higher than that.
        return (rom->GetRomBankCount() - 1);
    }

    bool MBC1Cartridge::CheckMBC1M()
    {
        constexpr uint8_t NintendoLogo[] = {0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
        0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89,
        0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
        0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};
        constexpr uint16_t  NintendoLogoStartAddress = 0x0104;
        constexpr uint8_t   NintendoLogoRomBank = 0x10;

        if (rom->GetRomBankCount() != RomBankCountMBC1M)
            return false;

        // Checking if this is a MBC1M cartridge or not.
        // If the Logo is found again this MBC1 cartridge contains 2 games, thus being a MBC1M cart.
        for (uint32_t i = 0; i < sizeof(NintendoLogo); i++)
        {
            if (romBanks[NintendoLogoRomBank][NintendoLogoStartAddress + i] != NintendoLogo[i])
                return false;
        }
        LOG_DEBUG("Cartridge detected as MBC1M");
        return true;
    }

    uint8_t MBC1Cartridge::ReadByte(const uint16_t address)
    {
        if (address >= RomAddressStart && address <= RomAddressEnd)
        {
            uint8_t selectedBank = 0x00;
            if (rom->GetRomBankCount() >= AmountOfBanksToBeLargeRom && bankingModeSelect == AdvancedBankingMode)
            {
                // For large ROMs this area can be banked, to make sure we address
                // the right bank we need to use the secondary bits bank which represents
                // the top 2 bits, hence the bit shift.
                // This means we can access the 0x00, 0x20, 0x40 and 0x60 banks
                // for a regular MBC1 and 0x00, 0x10, 0x20, and 0x30 for MCB1M cartridges
                selectedBank = secondarySelectedBankRegister << bankRegisterBitCount;
            }
            return romBanks[selectedBank][address];
        }
        else if (address >= RomBankedAddressStart && address <= RomBankedAddressEnd)
        {
            // Depending on the amount of banks of the cartridge we dont have to use the entire
            // register to get the selected bank e.g: a 256 KiB cart only needs a 4-bit bank number
            // to address all of its 16 banks, so this register is masked to 4 bits.
            // The upper bit would be ignored for bank selection. 
            uint8_t selectedBank = selectedBankRegister & RomBankMask();

            // If the main 5-bit ROM banking register is 0, it reads the bank as if it was set to 1.
            // MBC1M carts or smaller ROM cartridges still use the full 5 bits for this comparison as well.
            // The comparison is done on the full 5 bit register for both MBC1M carts or smaller ROM cartridges.
            // This means it is still possible to address bank 0 for cartridges that contain a 256 KiB or smaller ROM.
            if ((selectedBankRegister & 0b00011111) == 0x00)
                selectedBank += 1;

            if (rom->GetRomBankCount() >= AmountOfBanksToBeLargeRom && bankingModeSelect == AdvancedBankingMode)
            {
                selectedBank |= secondarySelectedBankRegister << bankRegisterBitCount;
            }

            return romBanks[selectedBank][address - RomBankedAddressStart];
        }
        else if (address >= RamBankedAddressStart && address <= RamBankedAddressEnd)
        {
            if (ramEnabled)
            {
                uint8_t selectedBank = 0;
                // The available amount of RAM banks on a MBC1 cartridge ranges from
                // 1 bank to 4 banks (8KiB to 4 banks of 8KiB resulting in 32KiB). 
                if (rom->GetRamBankCount() > 1)
                    selectedBank = secondarySelectedBankRegister;
                return ramBanks[selectedBank][address - RamBankedAddressStart];
            }
            LOG_DEBUG("Cartridge: Trying to read from RAM whilst it is not enabled");
            // If the cartridge has not enabled RAM writes are ignored and reads return
            // an open bus value, often being 0xFF but not guaranteed.
            return OpenBusValue;
        }
        // TODO: figure out what gets returned for other addresses
        LOG_DEBUG("Cartridge: Trying to read from an unsupported address");
        return OpenBusValue;
    }

    void MBC1Cartridge::WriteByte(const uint16_t address, const uint8_t value)
    {
        if (address >= RamEnableAddressStart && address <= RamEnableAddressEnd)
        {

        }
        else if (address >= RomBankNumberAddressStart && address <= RomBankNumberAddressEnd)
        {

        }
        else if (address >= RamBankNumberAddressStart && address <= RamBankNumberAddressEnd)
        {

        }
        else if (address >= BankingModeAddressStart && address <= BankingModeAddressEnd)
        {

        }
    }
}
