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

#include <play-man/gameboy/cartridge/Cartridge.hpp>
#include <play-man/gameboy/cartridge/Rom.hpp>
#include <play-man/logger/Logger.hpp>
#include <sstream>

namespace GameBoy
{

std::shared_ptr<ACartridge> MakeCartridge(const char* filePath) noexcept(false)
{
    auto rom = std::make_unique<Rom>(filePath);

    switch (rom->GetCartridgeType())
    {
        case CartridgeType::ROM_ONLY:
        case CartridgeType::ROM_RAM:
        case CartridgeType::ROM_RAM_BATTERY:
            return std::make_shared<NoMBCCartridge>(std::move(rom));
        case CartridgeType::MBC1:
        case CartridgeType::MBC1_RAM:
        case CartridgeType::MBC1_RAM_BATTERY:
            return std::make_shared<MBC1Cartridge>(std::move(rom));
        case CartridgeType::MBC2:
        case CartridgeType::MBC2_BATTERY:
            return std::make_shared<MBC2Cartridge>(std::move(rom));
        case CartridgeType::MBC3_RAM:
        case CartridgeType::MBC3_RAM_BATTERY:
        case CartridgeType::MBC3_TIMER_BATTERY:
        case CartridgeType::MBC3_TIMER_RAM_BATTERY:
            return std::make_shared<MBC3Cartridge>(std::move(rom));
        case CartridgeType::MBC5:
        case CartridgeType::MBC5_RAM:
        case CartridgeType::MBC5_RAM_BATTERY:
        case CartridgeType::MBC5_RUMBLE:
        case CartridgeType::MBC5_RUMBLE_RAM:
        case CartridgeType::MBC5_RUMBLE_RAM_BATTERY:
            return std::make_shared<MBC5Cartridge>(std::move(rom));
        default:
            std::stringstream error;
            error << "Unsupported ROM type: ";
            error << rom->GetCartridgeType();
            error << "\n";
            error << rom->GetHeader();
            LOG_FATAL(error.str());
            throw std::runtime_error(error.str());
    }
}

}
