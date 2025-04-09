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
#include <play-man/gameboy/cpu/Opcodes.hpp>
#include <play-man/gameboy/cpu/Cpu.hpp>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	std::cout << "Welcome to play-man!" << std::endl;

    if (argc > 1)
    {
        GameBoy::Rom rom(argv[1]);
        GameBoy::Cpu cpu;

        std::cout << rom << std::endl;

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

	GameBoy::Cpu cpu;
	cpu.ExecuteInstruction(GameBoy::PrefixedOpCode::BIT_0_A);

	return 0;
}
