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
        // After a Cartridge is created from the ROM the romData is
        // cleared and stored inside romBanks inside the cartridge.
        std::vector<uint8_t> romData;
        const char*          filePath;

        void ParseRomFile(const char* filePath) noexcept(false);

    public:
        Rom() = delete;
        Rom(const char* romFilePath) noexcept(false);

        const RomHeader&                GetHeader() const;
        const std::vector<uint8_t>&     GetData() const;
        const char*                     GetFilePath();
        CartridgeType                   GetCartridgeType() const;
        uint32_t                        GetRomBankCount() const;
        uint32_t                        GetRamBankCount() const;

        /**
         * @note After a Cartridge is created there is no use
         * to keep storing the romData inside the Rom class,
         * the Cartridge will use its own ROM banks. 
         */
        void                            ClearData();

};

std::ostream& operator << (std::ostream& lhs, Rom& rom);

};
