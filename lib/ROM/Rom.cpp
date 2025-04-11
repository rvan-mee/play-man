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

#include <play-man/ROM/Rom.hpp>
#include <iostream>
#include <iomanip>
#include <string_view>
#include <type_traits>
#include <optional>
#include <ios>

namespace GameBoy {

    Rom::Rom(const char* filePath)
    {
        _filePath = filePath;
        ParseRomFile(filePath);
    }
}


static int32_t GetPaddingAfterName(const std::string_view name)
{
    constexpr size_t headerPrintLineWidth = 52;
    constexpr size_t decorationWidth = (sizeof("/     ")-1) + (sizeof(": ")-1) + (sizeof("     \\")-1);
    const size_t padding = headerPrintLineWidth - (decorationWidth + name.size());

    return (padding);
}

template<class T>
static void PrintIntegralLine(const std::string_view name, T var, int32_t bytesInHexValue)
{
    int32_t padding = GetPaddingAfterName(name) - (bytesInHexValue * 2);
    auto resetFlags = std::cout.flags();
    std::cout << std::hex << std::uppercase;

    // print the starting decoration and the name
    std::cout << "/     " << name << ": ";
    // offset the start of the hex by as many bytes needed for proper formatting
    std::cout << std::setw(padding) << "0x";
    // print the value as a hex, padded by 0's
    std::cout << std::setfill('0') << std::setw(bytesInHexValue * 2) << var;
    // print the trailing decoration
    std::cout << "     \\" << std::endl;

    // reset the cout formatting
    std::cout.flags(resetFlags);
    std::cout << std::setfill(' ');
}

static uint32_t ConvertInt8(uint8_t val)
{
    return (static_cast<uint32_t>(val));
}

template<class T>
static void PrintHeaderLine(const std::string_view name, T var)
{
    int32_t padding = GetPaddingAfterName(name);

    std::cout << "/     " << name << ": " << std::setw(padding) << var << "     \\" << std::endl;
}

static void PrintHeaderLine(const std::string_view name, uint8_t var)
{
    PrintIntegralLine(name, ConvertInt8(var), sizeof(uint8_t));
}

static void PrintHeaderLine(const std::string_view name, uint16_t var)
{
    PrintIntegralLine(name, var, sizeof(uint16_t));
}

namespace GameBoy {

    RomHeader& Rom::GetHeader()
    {
        return _header;
    }

    std::vector<int8_t>& Rom::GetData()
    {
        return _romData;
    }

    const char* Rom::GetFilePath()
    {
        return _filePath;
    }

    std::ostream& operator << (std::ostream& lhs, const RomHeader& header)
    {
        std::cout << "/**************************************************\\" << "\n";
        std::cout << "/                    Rom Header:                   \\" << "\n";
        std::cout << "/                                                  \\" << "\n";
        PrintHeaderLine("Title",              header.title.data());
        PrintHeaderLine("Manufacturer code",  header.manufacturerCode.data());
        PrintHeaderLine("CGB flag",           header.cgbFlag);
        PrintHeaderLine("New licensing code", header.newLicensingCode);
        PrintHeaderLine("SGB flag",           header.sgbFlag);
        PrintHeaderLine("Cartridge type",     header.cartridgeType);
        PrintHeaderLine("Rom size",           header.romSize);
        PrintHeaderLine("Ram size",           header.ramSize);
        PrintHeaderLine("Destination code",   header.destinationCode);
        PrintHeaderLine("Old licensing code", header.oldLicensingCode);
        PrintHeaderLine("Rom version",        header.romVersion);
        PrintHeaderLine("Header checksum",    header.headerChecksum);
        PrintHeaderLine("Global checksum",    header.globalChecksum);
        std::cout << "/                                                  \\" << "\n";
        std::cout << "/**************************************************\\" << "\n";
        return (lhs);
    }

    std::ostream& operator << (std::ostream& lhs, Rom& rom)
    {
        std::cout << "Info for ROM located on: " << rom.GetFilePath() << std::endl;
        std::cout << rom.GetHeader();
        return (lhs);
    }
}
