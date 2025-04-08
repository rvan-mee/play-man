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

#include <iostream>
#include <play-man/ROM/RomParser.hpp>
#include <play-man/settings/PlayManSettings.hpp>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	std::cout << "Welcome to play-man!" << std::endl;

    if (argc > 1)
    {
        GameBoy::Rom rom(argv[1]);

        std::cout << rom << std::endl;
    }
    else
    {
        GameBoy::RomHeader header;

        header.title = {"Pokemon Blue"};
        header.manufacturerCode = {"NINT"};
        header.cgbFlag = static_cast<int8_t>(0x01);
        header.newLicensingCode = NewLicensingCode::Nintendo;
        header.sgbFlag = 0x00;
        header.cartridgeType = CartridgeType::MBC1;
        header.romSize = RomSize::MiB8;
        header.ramSize = RamSize::KiB128;
        header.destinationCode = DestinationCode::Overseas;
        header.oldLicensingCode = OldLicensingCode::UseNewLicenseeCode;
        header.romVersion = 0x01;
        header.headerChecksum = 0x16;
        header.globalChecksum = 0;

        std::cout << "Hard-coded header:" << std::endl;
        std::cout << header << std::endl;
    }

	std::shared_ptr<PlayManSettings> settings = PlayManSettings::ReadFromFile("settings.json");
	Logger::LogInterface::Initialize(settings->logDirectory, settings->logLevel);
	
	LOG_INFO("Settings being used:\n" + settings->ToString());
	return 0;
}
