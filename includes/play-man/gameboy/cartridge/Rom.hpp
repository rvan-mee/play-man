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
#include <play-man/gameboy/cartridge/CartridgeDefines.hpp>
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
        void Init(const std::vector<uint8_t>& data);
};

std::ostream& operator << (std::ostream& lhs, const RomHeader& header);

class Rom
{
    private:
        RomHeader   header;
        MemoryBanks romBanks;
        const char* filePath;

        void InitRomBanks(std::vector<uint8_t>& rawRomData);

    public:
        Rom() = delete;
        Rom(const char* romFilePath) noexcept(false);

        const RomHeader&                GetHeader() const;
        const MemoryBanks&              GetBanks() const;
        const char*                     GetFilePath() const;
        CartridgeType                   GetCartridgeType() const;
        uint32_t                        GetRomBankCount() const;
        uint32_t                        GetRamBankCount() const;

        uint8_t                         ReadFromBank(uint32_t bank, uint16_t address) const;

};

std::ostream& operator << (std::ostream& lhs, Rom& rom);

};
