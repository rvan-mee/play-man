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

class RomHeader {
    public:
        std::array<int8_t, NINTENDO_LOGO_SIZE>      nintendoLogo;
        std::array<char, ROM_TITLE_SIZE>            title;
        std::array<char, MANUFACTURER_CODE_SIZE>    manufacturerCode;
        int8_t                                      cgbFlag;
        NewLicensingCodes                           newLicensingCode;
        int8_t                                      sgbFlag; 
        CartridgeType                               cartridgeType;
        RomSize                                     romSize;
        RamSize                                     ramSize;
        DestinationCode                             destinationCode;
        OldLicensingCodes                           oldLicensingCodes;
        int8_t                                      romVersion;
        int8_t                                      headerChecksum;
        int16_t                                     globalChecksum;
}; // TODO: ostream OVERLOAD

class Rom
{
    private:
        RomHeader           header;
        std::vector<int8_t> romData;

    public:

}; // TODO: ostream OVERLOAD
