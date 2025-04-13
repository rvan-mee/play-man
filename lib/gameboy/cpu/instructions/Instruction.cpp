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
#include <play-man/utility/UtilFunc.hpp>

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
		if (instructionToExecute == nullptr)
		{
			throw std::runtime_error("Illegal instruction, no instruction found for opcode: " + OpCodeAsHexString());
		}
	}

	Instruction::Instruction(PrefixedOpCode opCode_, InstructionPrototype instruction_)
		: opCode(OpCode::PREFIX)
		, prefixedOpCode(opCode_)
		, instructionToExecute(instruction_)
		, hasBeenExecuted(false)
	{
		if (instructionToExecute == nullptr)
		{
			throw std::runtime_error("Illegal instruction, no instruction found for opcode: " + OpCodeAsHexString());
		}
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

	std::string Instruction::OpCodeAsHexString() const
	{
		int16_t opcode16bit = GetEnumAsValue(opCode);
		if (IsPrefixed())
		{
			opcode16bit <<= 8;
			opcode16bit += GetEnumAsValue(GetPrefixedOpCode());
		}
		return Utility::IntAsHexString(opcode16bit);
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
