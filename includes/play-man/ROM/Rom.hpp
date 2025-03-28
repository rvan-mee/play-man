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

#include <play-man/ROM/RomHeaderDefines.hpp>
#include <array>
#include <vector>
#include <stdint.h>
#include <ostream>

namespace GameBoy {

class RomHeader {
    public:
        std::array<int8_t, nintendoLogoSize>        nintendoLogo;
        std::array<char, romTitleSize + 1>          title;
        std::array<char, manufacturerCodeSize + 1>  manufacturerCode;
        int8_t                                      cgbFlag;
        NewLicensingCode                            newLicensingCode;
        int8_t                                      sgbFlag; 
        CartridgeType                               cartridgeType;
        RomSize                                     romSize;
        RamSize                                     ramSize;
        DestinationCode                             destinationCode;
        OldLicensingCode                            oldLicensingCode;
        int8_t                                      romVersion;
        int8_t                                      headerChecksum;
        int16_t                                     globalChecksum;

        void ParseRawData(const std::vector<int8_t>& data);
};

std::ostream& operator << (std::ostream& lhs, const RomHeader& header);

class Rom
{
    private:
        RomHeader           _header;
        std::vector<int8_t> _romData;
        const char*         _filePath;

        void ParseRomFile(const char* filePath);

    public:
        Rom() = delete;
        Rom(const char* filePath);

        RomHeader&              GetHeader();
        std::vector<int8_t>&    GetData();
        const char*             GetFilePath();

};

std::ostream& operator << (std::ostream& lhs, Rom& rom);

};
