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
#include <play-man/utility/UtilFunc.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstring>
#include <assert.h>

namespace GameBoy {

    void RomHeader::ParseRawData(const std::vector<int8_t>& data)
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
        if (data[cgbFlagIndex] == static_cast<int8_t>(CgbFlag::BackwardsCompatible) || \
            data[cgbFlagIndex] == static_cast<int8_t>(CgbFlag::CgbOnly))
        {
            cgbFlag = data[cgbFlagIndex];
        }
        else
        {
            cgbFlag = 0x00;
        }

        // New Licensing Code
        const uint8_t firstByte = data[newLicensingCodeIndex];
        const uint8_t secondByte = data[newLicensingCodeIndex + 1];
        newLicensingCode = static_cast<NewLicensingCode>(firstByte << 8 | secondByte);

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

    void    Rom::ParseRomFile(const char* filePath)
    {
        std::ifstream rom(filePath);

        if (!rom.is_open())
        {
            std::string error;

            error += "Failed to open ROM: ";
            error += filePath;
            error += "\nError: " + Utility::ErrnoToString() + "\n";
            throw std::runtime_error(error); 
        }

        std::streamsize romSize = std::filesystem::file_size(filePath);

        _romData.resize(romSize);
        rom.read(reinterpret_cast<char*>(_romData.data()), romSize);
        rom.close();

        _header.ParseRawData(_romData);
    }

}
