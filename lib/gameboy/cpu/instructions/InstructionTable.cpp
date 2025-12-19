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

#include <play-man/gameboy/cpu/Cpu.hpp>

namespace GameBoy
{
	void Cpu::InitInstructionTable()
	{
		constexpr auto thisPtr = std::placeholders::_1;

		// 0x0-
		instructions[OpCode::NOP] = std::bind(&Cpu::NOP, thisPtr);
		instructions[OpCode::LD_BC_n16] = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, &CpuCore::BC);
		instructions[OpCode::LD_BC_NI_A] = std::bind(&Cpu::Store_8bit_Addr_High, thisPtr, &CpuCore::BC, &CpuCore::AF);
		instructions[OpCode::INC_BC] = std::bind(&Cpu::Increment_16bit, thisPtr, &CpuCore::BC);
		instructions[OpCode::INC_B] = std::bind(&Cpu::Increment_8bit_High, thisPtr, &CpuCore::BC);
		instructions[OpCode::DEC_B] = std::bind(&Cpu::Decrement_8bit_High, thisPtr, &CpuCore::BC);
		instructions[OpCode::LD_B_n8] = std::bind(&Cpu::Load_8bit_High_ImmediateData, thisPtr, &CpuCore::BC);
		instructions[OpCode::RLCA] = std::bind(&Cpu::Rotate_8bit_High_Left, thisPtr, &CpuCore::AF);
		instructions[OpCode::LD_a16_NI_SP] = std::bind(&Cpu::Load_16bit_RegToImmediateAddr, thisPtr, &CpuCore::SP);
		instructions[OpCode::ADD_HL_BC] = std::bind(&Cpu::Add_16bit, thisPtr, &CpuCore::HL, &CpuCore::BC);
		instructions[OpCode::LD_A_BC_NI] = std::bind(&Cpu::Load_8bit_High_Addr, thisPtr, &CpuCore::AF, &CpuCore::BC);

		// 0x1-
		// 0x2-
		// 0x3-
		// 0x4-
		// 0x5-
		// 0x6-
		// 0x7-
		// 0x8-
		// 0x9-
		// 0xA-
		// 0xB-
		// 0xC-
		instructions[OpCode::JP_a16] = std::bind(&Cpu::Jump_16bit_ImmediateData, thisPtr);

		// 0xD-
		instructions[OpCode::ILLEGAL_D3] = std::bind(&Cpu::HardLock, thisPtr);
		instructions[OpCode::ILLEGAL_DB] = std::bind(&Cpu::HardLock, thisPtr);
		instructions[OpCode::ILLEGAL_DD] = std::bind(&Cpu::HardLock, thisPtr);

		// 0xE-
		instructions[OpCode::ILLEGAL_E3] = std::bind(&Cpu::HardLock, thisPtr);
		instructions[OpCode::ILLEGAL_E4] = std::bind(&Cpu::HardLock, thisPtr);
		instructions[OpCode::ILLEGAL_EB] = std::bind(&Cpu::HardLock, thisPtr);
		instructions[OpCode::ILLEGAL_EC] = std::bind(&Cpu::HardLock, thisPtr);
		instructions[OpCode::ILLEGAL_ED] = std::bind(&Cpu::HardLock, thisPtr);

		// 0xF-
		instructions[OpCode::CP_A_n8] = std::bind(&Cpu::Compare_8bit_High_ImmediateData, thisPtr, &CpuCore::AF);
		instructions[OpCode::ILLEGAL_F4] = std::bind(&Cpu::HardLock, thisPtr);
		instructions[OpCode::ILLEGAL_FC] = std::bind(&Cpu::HardLock, thisPtr);
		instructions[OpCode::ILLEGAL_FD] = std::bind(&Cpu::HardLock, thisPtr);


		/** Prefixed instructions **/

		// 0x0-
		// 0x1-
		// 0x2-
		// 0x3-
		// 0x4-
		// 0x5-
		// 0x6-
		// 0x7-
		// 0x8-
		// 0x9-
		// 0xA-
		// 0xB-
		// 0xC-
		// 0xD-
		// 0xE-
		// 0xF-	
	}
}
