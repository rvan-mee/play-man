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

#include <play-man/gameboy/cartridge/MCB1.hpp>


/*          Write Address Defines          */
constexpr uint16_t RamEnableAddressStart = 0x0000;
constexpr uint16_t RamEnableAddressEnd = 0x1FFF;
constexpr uint16_t RomBankNumberAddressStart = 0x2000;
constexpr uint16_t RomBankNumberAddressEnd = 0x3FFF;
// The RAM bank number can also be used for the upper bits
// of the ROM bank number for => 1MB cartridges.
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
constexpr uint8_t DefaultSelectedRamBank = 0x00;
constexpr uint8_t DefaultSelectedRomBank = 0x01;
constexpr uint8_t DefaultSecondarySelectedBankBits = 0x00;

// TODO: figure out the correct value: https://gbdev.io/pandocs/MBC1.html
// mentions that 0xFF is often the value but not guaranteed.
constexpr uint8_t OpenBusValue = 0xFF;

namespace GameBoy
{

    MCB1Cartridge::MCB1Cartridge(std::unique_ptr<Rom> rom) : ACartridge(std::move(rom))
    {
        secondarySelectedBankBits = DefaultSecondarySelectedBankBits;
        selectedRomBank = DefaultSelectedRomBank;
        selectedRamBank = DefaultSelectedRamBank;
        ramEnabled = DefaultRamEnabled;

        InitRomBanks();
        InitRamBanks();
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

    void MCB1Cartridge::InitRomBanks()
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

    void MCB1Cartridge::InitRamBanks()
    {
        uint8_t ramBankCount = rom->GetRamBankCount();

        ramBanks.resize(ramBankCount);
        for (uint32_t i = 0; i < ramBankCount; i++)
            ramBanks[i].resize(RamBankSize);
    }

    uint8_t MCB1Cartridge::ReadByte(const uint16_t address)
    {
        if (address >= RomAddressStart && address <= RomAddressEnd)
        {
            /* TODO:
                This area normally contains the first 16 KiB (bank 00) of the cartridge ROM.

                In 1 MiB and larger cartridges (that use the 2-bit second banking register for extended ROM banking),
                entering mode 1 (see below) will allow that second banking register to apply to reads from this
                region in addition to the regular 4000–7FFF banked region,
                resulting in accessing banks $20/$40/$60 for regular large ROM carts,
                or banks $10/$20/$30 for an MBC1M multi-cart.
            */
            return romBanks[0][address];

        }
        else if (address >= RomBankedAddressStart && address <= RomBankedAddressEnd)
        {
            return romBanks[selectedRomBank][address - RomBankedAddressStart];
        }
        else if (address >= RamBankedAddressStart && address <= RamBankedAddressEnd)
        {
            // if (ramEnabled)
            // {

            // }
            // else
            // {
                return OpenBusValue;
            // }

        }
        // TODO: figure out what happens for other addresses
        return OpenBusValue;
    }

    void MCB1Cartridge::WriteByte(const uint16_t address, const uint8_t value)
    {
        (void) address;
        (void) value;
    }
}
