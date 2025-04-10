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
#include <play-man/utility/UtilFunc.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstring>
#include <assert.h>

namespace GameBoy {

    void RomHeader::ParseRawData(const std::vector<int8_t>& data)
    {
        assert(data.size() >= 0x14E);
        title.fill('\0');
        manufacturerCode.fill('\0');

        // Nintendo Logo
        for (uint32_t i = 0; i < nintendoLogoSize; i++)
            nintendoLogo[i] = data[i + nintendoLogoIndex];

        // Cartridge Title
        for (uint32_t i = 0; i < romTitleSize; i++)
        {
            if (!(data[i + titleIndex] >= 32 && data[i + titleIndex] <= 126))
                break ;
            title[i] = data[i + titleIndex];
        }

        // Manufacturer Code
        // Not all cartridges contained a manufacturer code, instead using the space for the title
        // TODO: figure out if there is a way to separate the title and the manufacturer code 
        for (uint32_t i = 0; i < manufacturerCodeSize; i++)
            manufacturerCode[i] = data[manufacturerCodeIndex + i];

        // CGB Flag
        // Not all cartridges contained a CGB Flag, instead using the space for the title
        if (data[cgbFlagIndex] == static_cast<int8_t>(0xC0) || data[cgbFlagIndex] == static_cast<int8_t>(0x80))
            cgbFlag = data[cgbFlagIndex];
        else
            cgbFlag = 0x00;

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
            std::cerr << "Failed to open ROM: " << filePath << std::endl;
            std::cerr << "Error: " << Utility::ErrnoToString() << std::endl;
            throw std::runtime_error("Failed open ROM"); 
        }

        std::streamsize romSize = std::filesystem::file_size(filePath);

        _romData.resize(romSize);
        rom.read(reinterpret_cast<char*>(_romData.data()), romSize);
        rom.close();

        _header.ParseRawData(_romData);
    }

}
