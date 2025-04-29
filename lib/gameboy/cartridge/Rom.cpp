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

#include <play-man/gameboy/cartridge/Rom.hpp>
#include <play-man/utility/UtilFunc.hpp>
#include <string_view>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ios>

static int32_t GetPaddingAfterName(const std::string_view name)
{
    constexpr size_t headerPrintLineWidth = 52;
    constexpr size_t decorationWidth = (sizeof("/     ")-1) + (sizeof(": ")-1) + (sizeof("     \\")-1);
    const size_t padding = headerPrintLineWidth - (decorationWidth + name.size());

    return (padding);
}

template<class T>
static void PrintIntegralLine(std::ostream& stream, const std::string_view name, T var, int32_t bytesInHexValue)
{
    int32_t padding = GetPaddingAfterName(name) - (bytesInHexValue * 2);
    auto resetFlags = stream.flags();
    stream << std::hex << std::uppercase;

    // print the starting decoration and the name
    stream << "/     " << name << ": ";
    // offset the start of the hex by as many bytes needed for proper formatting
    stream << std::setw(padding) << "0x";
    // print the value as a hex, padded by 0's
    stream << std::setfill('0') << std::setw(bytesInHexValue * 2) << var;
    // print the trailing decoration
    stream << "     \\" << std::endl;

    // reset the stream formatting
    stream.flags(resetFlags);
    stream << std::setfill(' ');
}

static uint32_t ConvertInt8(uint8_t val)
{
    return (static_cast<uint32_t>(val));
}

template<class T>
static void PrintHeaderLine(std::ostream& stream, const std::string_view name, T var)
{
    int32_t padding = GetPaddingAfterName(name);

    stream << "/     " << name << ": " << std::setw(padding) << var << "     \\" << std::endl;
}

static void PrintHeaderLine(std::ostream& stream, const std::string_view name, uint8_t var)
{
    PrintIntegralLine(stream, name, ConvertInt8(var), sizeof(uint8_t));
}

static void PrintHeaderLine(std::ostream& stream, const std::string_view name, uint16_t var)
{
    PrintIntegralLine(stream, name, var, sizeof(uint16_t));
}

namespace GameBoy {

    /*     RomHeader     */

    void RomHeader::Init(const std::vector<uint8_t>& data)
    {
        assert(data.size() >= romHeaderSize);
        title.fill('\0');
        manufacturerCode.fill('\0');

        // Nintendo Logo
        for (uint32_t i = 0; i < nintendoLogoSize; i++)
            nintendoLogo[i] = data[i + nintendoLogoIndex];

        // Cartridge Title
        for (uint32_t i = 0; i < romTitleSize; i++)
        {
            // Newer cartridges possibly used some bytes of the title for the manufacturer code
            // or the CBG flag. Since the CBG flag is not a printable ASCII value,
            // we can prevent adding it into the title with this check.
            if (!(std::isprint(data[i + titleIndex])))
                break ;
            title[i] = data[i + titleIndex];
        }

        // Manufacturer Code
        // Not all cartridges contained a manufacturer code, instead using the space for the title
        // TODO: figure out if there is a way to separate the title and the manufacturer code
        // Opened issue #20 to track this TODO.
        for (uint32_t i = 0; i < manufacturerCodeSize; i++)
            manufacturerCode[i] = data[manufacturerCodeIndex + i];

        // CGB Flag
        // Not all cartridges contained a CGB Flag, instead using the space for the title
        if (data[cgbFlagIndex] == static_cast<uint8_t>(CgbFlag::BackwardsCompatible) || \
            data[cgbFlagIndex] == static_cast<uint8_t>(CgbFlag::CgbOnly))
        {
            cgbFlag = static_cast<CgbFlag>(data[cgbFlagIndex]);
        }
        else
        {
            cgbFlag = CgbFlag::NotSet;
        }

        // New Licensing Code
        const uint8_t firstByte = data[newLicensingCodeIndex];
        const uint8_t secondByte = data[newLicensingCodeIndex + 1];
        newLicensingCode = static_cast<NewLicensingCode>(firstByte << 8 | secondByte);

        // Remaining data
        sgbFlag = data[sgbFlagIndex];
        cartridgeType = static_cast<CartridgeType>(data[cartridgeTypeIndex]);
        romSize = static_cast<RomSize>(data[romSizeIndex]);
        ramSize = static_cast<RamSize>(data[ramSizeIndex]);
        destinationCode = static_cast<DestinationCode>(data[destinationCodeindex]);
        oldLicensingCode = static_cast<OldLicensingCode>(data[oldLicensingCodeindex]);
        romVersion = data[romVersionIndex];
        headerChecksum = data[headerChecksumIndex];
        globalChecksum = data[globalChecksumIndex];
    }

    std::ostream& operator << (std::ostream& lhs, const RomHeader& header)
    {
        lhs << "/**************************************************\\" << "\n";
        lhs << "/                    Rom Header:                   \\" << "\n";
        lhs << "/                                                  \\" << "\n";
        PrintHeaderLine(lhs, "Title",              header.title.data());
        PrintHeaderLine(lhs, "Manufacturer code",  header.manufacturerCode.data());
        PrintHeaderLine(lhs, "CGB flag",           header.cgbFlag);
        PrintHeaderLine(lhs, "New licensing code", header.newLicensingCode);
        PrintHeaderLine(lhs, "SGB flag",           header.sgbFlag);
        PrintHeaderLine(lhs, "Cartridge type",     header.cartridgeType);
        PrintHeaderLine(lhs, "Rom size",           header.romSize);
        PrintHeaderLine(lhs, "Ram size",           header.ramSize);
        PrintHeaderLine(lhs, "Destination code",   header.destinationCode);
        PrintHeaderLine(lhs, "Old licensing code", header.oldLicensingCode);
        PrintHeaderLine(lhs, "Rom version",        header.romVersion);
        PrintHeaderLine(lhs, "Header checksum",    header.headerChecksum);
        PrintHeaderLine(lhs, "Global checksum",    header.globalChecksum);
        lhs << "/                                                  \\" << "\n";
        lhs << "/**************************************************\\" << "\n";
        return (lhs);
    }

    /*     Rom     */

    Rom::Rom(const char* romFilePath) noexcept(false)
    {
        filePath = romFilePath;
        std::ifstream rom(filePath);
    
        if (!rom.good())
        {
            std::string error;

            error += "Failed to open ROM: ";
            error += filePath;
            error += "\nError: " + Utility::ErrnoToString() + "\n";
            throw std::runtime_error(error); 
        }

        std::streamsize romSize = std::filesystem::file_size(filePath);
        std::vector<uint8_t> rawRomData;

        rawRomData.resize(romSize);
        rom.read(reinterpret_cast<char*>(rawRomData.data()), romSize);
        rom.close();

        header.Init(rawRomData);
        InitRomBanks(rawRomData);
    }

    /**
     * @brief Copies the given raw data into the correct rom bank
     * 
     * @param rawRomData The full data inside the ROM
     * @param romBank The bank needing to be filled
     * @param bankNumber The number of the bank being filled
     */
    static void CopyRomBankData(const std::vector<uint8_t>& rawRomData, std::vector<uint8_t>& romBank, uint32_t bankNumber)
    {
        uint32_t    currentBankStart = bankNumber * RomBankSize;
        uint32_t    currentBankEnd = (bankNumber + 1) * RomBankSize;

        if (currentBankStart > rawRomData.size())
            return ;

        if (currentBankEnd > rawRomData.size())
            currentBankEnd = rawRomData.size();
        
        romBank.insert(romBank.begin(), rawRomData.begin() + currentBankStart, rawRomData.begin() + currentBankEnd);
    }
    
    void    Rom::InitRomBanks(std::vector<uint8_t>& rawRomData)
    {
        uint32_t romBankCount = GetRomBankCount();
        
        romBanks.resize(romBankCount);
        for (uint32_t i = 0; i < romBankCount; i++)
        {
            romBanks[i].resize(RomBankSize);
            CopyRomBankData(rawRomData, romBanks[i], i);
        }
    }

    const RomHeader& Rom::GetHeader() const
    {
        return header;
    }

    const MemoryBanks& Rom::GetBanks() const
    {
        return romBanks;
    }

    const char* Rom::GetFilePath() const
    {
        return filePath;
    }

    CartridgeType   Rom::GetCartridgeType() const
    {
        return header.cartridgeType;
    }

    uint32_t Rom::GetRomBankCount() const
    {
        switch (header.romSize)
        {
            case RomSize::KiB32:
                return 2;
            case RomSize::KiB64:
                return 4;
            case RomSize::KiB128:
                return 8;
            case RomSize::KiB256:
                return 16;
            case RomSize::KiB512:
                return 32;
            case RomSize::MiB1:
                return 64;
            case RomSize::MiB2:
                return 128;
            case RomSize::MiB4:
                return 256;
            case RomSize::MiB8:
                return 512;
            default:
                return 0;
        }
    }

    uint32_t Rom::GetRamBankCount() const
    {
        switch (header.ramSize)
        {
            case RamSize::NoRam:
                return 0;
            case RamSize::KiB8:
                return 1;
            case RamSize::KiB32:
                return 4;
            case RamSize::KiB64:
                return 8;
            case RamSize::KiB128:
                return 16;
            default:
                return 0;
        }
    }

    uint8_t Rom::ReadFromBank(uint32_t bank, uint16_t address) const
    {
        assert(bank < romBanks.size());
        assert(address < romBanks[bank].size());
        return romBanks[bank][address];
    }

    std::ostream& operator << (std::ostream& lhs, Rom& rom)
    {
        lhs << "Info for ROM located on: " << rom.GetFilePath() << std::endl;
        lhs << rom.GetHeader();
        return (lhs);
    }
}
