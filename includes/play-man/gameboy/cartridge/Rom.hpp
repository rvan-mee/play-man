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

#pragma once

#include <play-man/gameboy/cartridge/RomHeaderDefines.hpp>
#include <array>
#include <vector>
#include <stdint.h>
#include <ostream>

namespace GameBoy {

class RomHeader {
    public:
        std::array<uint8_t, nintendoLogoSize>       nintendoLogo;
        std::array<char, romTitleSize + 1>          title;
        std::array<char, manufacturerCodeSize + 1>  manufacturerCode;
        CgbFlag                                     cgbFlag;
        NewLicensingCode                            newLicensingCode;
        uint8_t                                     sgbFlag; 
        CartridgeType                               cartridgeType;
        RomSize                                     romSize;
        RamSize                                     ramSize;
        DestinationCode                             destinationCode;
        OldLicensingCode                            oldLicensingCode;
        uint8_t                                     romVersion;
        uint8_t                                     headerChecksum;
        uint16_t                                    globalChecksum;

        /**
         * @brief Parses the data given and sets all the RomHeader values.
         * 
         * @param data A vector that contains the raw data from a ROM. 
         */
        void ParseRawData(const std::vector<uint8_t>& data);
};

std::ostream& operator << (std::ostream& lhs, const RomHeader& header);

class Rom
{
    private:
        RomHeader            header;
        std::vector<uint8_t> romData;
        const char*          filePath;
        // TODO: Bank switching

        void ParseRomFile(const char* filePath) noexcept(false);

    public:
        Rom() = delete;
        Rom(const char* romFilePath) noexcept(false);

        const RomHeader&                GetHeader() const;
        const std::vector<uint8_t>&     GetData() const;
        const char*                     GetFilePath();
        uint8_t                         ReadByte(const uint16_t address);
        CartridgeType                   GetCartridgeType() const;

};

std::ostream& operator << (std::ostream& lhs, Rom& rom);

};
