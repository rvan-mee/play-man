
#include <iostream>
#include <play-man/ROM/RomParser.hpp>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	std::cout << "Welcome to play-man!" << std::endl;

	{
		GameBoy::RomHeader header;

		header.title = {"Pokemon Blue"};
		header.manufacturerCode = {"NINT"};
		header.cgbFlag = static_cast<int8_t>(0x01);
		header.newLicensingCode = NewLicensingCodes::Nintendo;
		header.sgbFlag = 0x00;
		header.cartridgeType = CartridgeType::MBC1;
		header.romSize = RomSize::MiB8;
		header.ramSize = RamSize::KiB128;
		header.destinationCode = DestinationCode::Overseas;
		header.oldLicensingCodes = OldLicensingCodes::NewLicenseeCode;
		header.romVersion = 0xFF;
		header.headerChecksum = 0x16;
		header.globalChecksum = 0;

		std::cout << "Hard-coded header:" << std::endl;
		std::cout << header << std::endl;
	}

	return 0;
}
