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

#include <play-man/gameboy/cpu/Instruction.hpp>
#include <play-man/utility/JsonUtility.hpp>

namespace GameBoy
{

	Instruction::Instruction()
		: opCode(OpCode::NOP)
		, prefixedOpCode(std::nullopt)
		, instructionToExecute(nullptr)
		, hasBeenExecuted(true) // To prevent it from being executed.
	{

	}

	Instruction::Instruction(OpCode opCode_, InstructionPrototype instruction_)
		: opCode(opCode_)
		, prefixedOpCode(std::nullopt)
		, instructionToExecute(instruction_)
		, hasBeenExecuted(false)
	{
		assert(instructionToExecute != nullptr);
	}

	Instruction::Instruction(PrefixedOpCode opCode_, InstructionPrototype instruction_)
		: opCode(OpCode::PREFIX)
		, prefixedOpCode(opCode_)
		, instructionToExecute(instruction_)
		, hasBeenExecuted(false)
	{
		assert(instructionToExecute != nullptr);
	}

	size_t Instruction::Execute()
	{
		assert(!hasBeenExecuted);

		hasBeenExecuted = true;
		return instructionToExecute();
	}

	bool Instruction::IsPrefixed() const
	{
		return opCode == OpCode::PREFIX;
	}

	OpCode Instruction::GetOpCode() const
	{
		return opCode;
	}

	PrefixedOpCode Instruction::GetPrefixedOpCode() const
	{
		return prefixedOpCode.value();
	}

	std::ostream& operator << (std::ostream& os, Instruction& i)
	{
		return Utility::Json::PrintFormatted(os, i);
	}

	void to_json(nlohmann::json& j, const Instruction& instruction)
	{
		j = Utility::Json::CreateEmptyJson();
		j.emplace("opCode", instruction.opCode);
		if (instruction.prefixedOpCode.has_value())
		{
			j.emplace("prefixedOpCode", instruction.prefixedOpCode.value());
		}
		j.emplace("hasBeenExecuted", instruction.hasBeenExecuted);
	}
	
} /* namespace GameBoy */
