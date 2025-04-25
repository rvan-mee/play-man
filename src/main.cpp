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
#include <play-man/settings/PlayManSettings.hpp>
#include <play-man/gameboy/opcodes/Opcodes.hpp>
#include <play-man/gameboy/cpu/Cpu.hpp>
#include <play-man/logger/Logger.hpp>
#include <iostream>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	std::cout << "Welcome to play-man!" << std::endl;
    Logger::LogInterface::Initialize("Logging", Logger::LogLevel::Debug);

    if (argc > 1)
    {
        std::shared_ptr<GameBoy::ACartridge> cartridge = GameBoy::MakeCartridge(argv[1]);

        std::cout << *cartridge << std::endl;

        GameBoy::Cpu cpu(cartridge);

        while (true)
        {
            cpu.FetchInstruction();
            cpu.ExecuteInstruction();
        }

        return 0;
    }
    else
    {
        std::cout << "No ROM provided!" << std::endl;
    }

	return 0;
}
