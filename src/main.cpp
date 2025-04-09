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

	Gameboy::Cpu t;
	t.ExecuteInstruction(Gameboy::PrefixedOpCode::BIT_0_A);
	const auto opcodeJson = Utility::Json::ReadJsonFromFile("opcodes.json");
	const auto unprefixedOpcodes = opcodeJson.find("unprefixed");

	for (const auto& [key, value] : unprefixedOpcodes.value().items())
	{
		(void)key;
		(void)value;
		// std::cout << value["operands"];
		std::string enumName = value.find("mnemonic").value();
		for (const auto& op : value["operands"])
		{
			enumName += "_" + op.value<std::string>("name", "");
			if (op.contains("increment"))
			{
				enumName += "_INC";
			}
			else if (op.contains("decrement"))
			{
				enumName += "_DEC";
			}
			enumName += (op.value<bool>("immediate", true) ? "" : "_NI");
		}
		// std::cout << "X(n, " << enumName << ", " << key << ")\t\t\t\\" << std::endl;
		std::cout << "case OpCode::" << enumName << ": \n{\n\n\tbreak;\n}\n";
	}

	// std::cout << unprefixedOpcodes.value() << std::endl;

	return 0;
}
