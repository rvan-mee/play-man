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

#include <play-man/gameboy/rom/Rom.hpp>
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

    int8_t  Rom::ReadRomByte(const int16_t address)
    {
        // TODO: Check how bank switching works
        assert(static_cast<size_t>(address) < _romData.size());
        return (_romData[address]);
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

    std::ostream& operator << (std::ostream& lhs, Rom& rom)
    {
        lhs << "Info for ROM located on: " << rom.GetFilePath() << std::endl;
        lhs << rom.GetHeader();
        return (lhs);
    }
}
