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

namespace GameBoy
{

std::shared_ptr<ACartridge> MakeCartridge(const char* filePath) noexcept(false)
{
    auto rom = std::make_unique<Rom>(filePath);

    switch (rom->GetCartridgeType())
    {
        case CartridgeType::MBC1:
        case CartridgeType::MBC1_RAM:
        case CartridgeType::MBC1_RAM_BATTERY:
            return std::make_shared<MCB1Cartridge>(std::move(rom));
        default:
            std::stringstream error;
            error << "Unsupported ROM type: ";
            error << rom->GetCartridgeType();
            LOG_FATAL(error.str());
            throw std::runtime_error(error.str());
    }
}

}
