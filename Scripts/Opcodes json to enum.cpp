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

#include <play-man/utility/UtilFunc.hpp>
#include <play-man/utility/JsonUtility.hpp>

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	const auto opcodeJson = Utility::Json::ReadJsonFromFile("opcodes.json");
	const auto unprefixedOpcodes = opcodeJson.find("cbprefixed");

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
		std::cout << "X(n, " << enumName << ", " << key << ")\t\t\t\\" << std::endl;
	}

	return 0;
}
