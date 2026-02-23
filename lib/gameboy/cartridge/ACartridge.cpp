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

#include <play-man/gameboy/cartridge/ACartridge.hpp>

namespace GameBoy {

    ACartridge::ACartridge(std::unique_ptr<Rom> _rom) : rom(std::move(_rom))
    {
        InitRamBanks();
    };

    void ACartridge::InitRamBanks()
    {
        const uint8_t ramBankCount = rom->GetRamBankCount();

        ramBanks.resize(ramBankCount);
        for (uint32_t i = 0; i < ramBankCount; i++)
            ramBanks[i].resize(RamBankSize);
    }

    CartridgeType   ACartridge::GetType() const
    {
        return rom->GetCartridgeType();
    }

    uint32_t        ACartridge::GetRamBankCount() const
    {
        return rom->GetRamBankCount();
    }

    uint32_t        ACartridge::GetRomBankCount() const
    {
        return rom->GetRomBankCount();
    }

    void    ACartridge::LoadTestRom(const char* filePath)
    {
        rom->LoadTestRom(filePath);
    }

    std::ostream& operator << (std::ostream& lhs, ACartridge& cart)
    {
        lhs << "Cartridge: " << *cart.rom << std::endl;
        return (lhs);
    }

}
