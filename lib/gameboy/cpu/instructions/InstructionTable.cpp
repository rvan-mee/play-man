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
		auto& table = instructions;

		// 0x0-
		table[OpCode::NOP]            = std::bind(&Cpu::NOP, thisPtr);
		table[OpCode::LD_BC_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, &CpuCore::BC);
		table[OpCode::LD_BC_NI_A]     = std::bind(&Cpu::Store_8bit_Addr_High, thisPtr, &CpuCore::BC, &CpuCore::AF);
		table[OpCode::INC_BC]         = std::bind(&Cpu::Increment_16bit, thisPtr, &CpuCore::BC);
		table[OpCode::INC_B]          = std::bind(&Cpu::Increment_8bit_High, thisPtr, &CpuCore::BC);
		table[OpCode::DEC_B]          = std::bind(&Cpu::Decrement_8bit_High, thisPtr, &CpuCore::BC);
		table[OpCode::LD_B_n8]        = std::bind(&Cpu::Load_8bit_High_ImmediateData, thisPtr, &CpuCore::BC);
		table[OpCode::RLCA]           = std::bind(&Cpu::Rotate_8bit_High_Left, thisPtr, &CpuCore::AF);
		table[OpCode::LD_a16_NI_SP]   = std::bind(&Cpu::Load_16bit_RegToImmediateAddr, thisPtr, &CpuCore::SP);
		table[OpCode::ADD_HL_BC]      = std::bind(&Cpu::Add_16bit, thisPtr, &CpuCore::HL, &CpuCore::BC);
		table[OpCode::LD_A_BC_NI]     = std::bind(&Cpu::Load_8bit_High_Addr, thisPtr, &CpuCore::AF, &CpuCore::BC);
		table[OpCode::DEC_BC]         = std::bind(&Cpu::Decrement_16bit, thisPtr, &CpuCore::BC);
		table[OpCode::INC_C]          = std::bind(&Cpu::Increment_8bit_Low, thisPtr, &CpuCore::BC);
		table[OpCode::DEC_C]          = std::bind(&Cpu::Decrement_8bit_Low, thisPtr, &CpuCore::BC);
		table[OpCode::LD_C_n8]        = std::bind(&Cpu::Load_8bit_Low_ImmediateData, thisPtr, &CpuCore::BC);
		table[OpCode::RRCA]           = std::bind(&Cpu::Rotate_8bit_Low_Right, thisPtr, &CpuCore::BC);

		// 0x1-
			// 0x10
			// TODO: STOP, implement when PPU/LCD and input register have been implemented;
			// https://gbdev.io/pandocs/Reducing_Power_Consumption.html#using-the-stop-instruction
		table[OpCode::LD_DE_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, &CpuCore::DE);
		table[OpCode::LD_DE_NI_A]     = std::bind(&Cpu::Store_8bit_Addr_High, thisPtr, &CpuCore::DE, &CpuCore::AF);
		table[OpCode::INC_DE]         = std::bind(&Cpu::Increment_16bit, thisPtr, &CpuCore::DE);
		table[OpCode::INC_D]          = std::bind(&Cpu::Increment_8bit_High, thisPtr, &CpuCore::DE);
		table[OpCode::DEC_D]          = std::bind(&Cpu::Decrement_8bit_High, thisPtr, &CpuCore::DE);
		table[OpCode::LD_D_n8]        = std::bind(&Cpu::Load_8bit_High_ImmediateData, thisPtr, &CpuCore::DE);
		table[OpCode::RLA]            = std::bind(&Cpu::Rotate_8bit_High_Left_Carry, thisPtr, &CpuCore::AF);
		table[OpCode::JR_e8]          = std::bind(&Cpu::Jump_Relative_8bit_SignedImmediateData, thisPtr);
		table[OpCode::ADD_HL_DE]      = std::bind(&Cpu::Add_16bit, thisPtr, &CpuCore::HL, &CpuCore::DE);
		table[OpCode::LD_A_DE_NI]     = std::bind(&Cpu::Load_8bit_High_Addr, thisPtr, &CpuCore::AF, &CpuCore::DE);
		table[OpCode::DEC_DE]         = std::bind(&Cpu::Decrement_16bit, thisPtr, &CpuCore::DE);
		table[OpCode::INC_E]          = std::bind(&Cpu::Increment_8bit_Low, thisPtr, &CpuCore::DE);
		table[OpCode::DEC_E]          = std::bind(&Cpu::Decrement_8bit_Low, thisPtr, &CpuCore::DE);
		table[OpCode::LD_E_n8]        = std::bind(&Cpu::Load_8bit_Low_ImmediateData, thisPtr, &CpuCore::DE);
		table[OpCode::RRA]            = std::bind(&Cpu::Rotate_8bit_High_Right_Carry, thisPtr, &CpuCore::AF);

		// 0x2-
		table[OpCode::JR_NZ_e8]       = std::bind(&Cpu::Jump_Relative_NotZero_8bit_SignedImmediateData, thisPtr);
		table[OpCode::LD_HL_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, &CpuCore::HL);
		table[OpCode::LD_HL_INC_NI_A] = std::bind(&Cpu::Store_8bit_AddrIncrement_High, thisPtr, &CpuCore::HL, &CpuCore::AF);
		table[OpCode::INC_HL]         = std::bind(&Cpu::Increment_16bit, thisPtr, &CpuCore::HL);
		table[OpCode::INC_H]          = std::bind(&Cpu::Increment_8bit_High, thisPtr, &CpuCore::HL);
		table[OpCode::DEC_H]          = std::bind(&Cpu::Decrement_8bit_High, thisPtr, &CpuCore::HL);
		table[OpCode::LD_H_n8]        = std::bind(&Cpu::Load_8bit_High_ImmediateData, thisPtr, &CpuCore::HL);
		table[OpCode::DAA]            = std::bind(&Cpu::DDA, thisPtr);
		table[OpCode::JR_Z_e8]        = std::bind(&Cpu::Jump_Relative_Zero_8bit_SignedImmediateData, thisPtr);
		table[OpCode::ADD_HL_HL]      = std::bind(&Cpu::Add_16bit, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_A_HL_INC_NI] = std::bind(&Cpu::Load_8bit_High_AddrIncrement, thisPtr, &CpuCore::AF, &CpuCore::HL);
		table[OpCode::DEC_HL]         = std::bind(&Cpu::Decrement_16bit, thisPtr, &CpuCore::HL);
		table[OpCode::INC_L]          = std::bind(&Cpu::Increment_8bit_Low, thisPtr, &CpuCore::HL);
		table[OpCode::DEC_L]          = std::bind(&Cpu::Decrement_8bit_Low, thisPtr, &CpuCore::HL);
		table[OpCode::LD_L_n8]        = std::bind(&Cpu::Load_8bit_Low_ImmediateData, thisPtr, &CpuCore::HL);
		table[OpCode::CPL]            = std::bind(&Cpu::CPL, thisPtr);

		// 0x3-
		table[OpCode::ADD_HL_SP]      = std::bind(&Cpu::Add_16bit, thisPtr, &CpuCore::HL, &CpuCore::SP);

		// 0x4-
		// 0x5-
		// 0x6-
		// 0x7-
		// 0x8-
		// 0x9-
		// 0xA-
		// 0xB-
		// 0xC-
		table[OpCode::JP_a16] = std::bind(&Cpu::Jump_16bit_ImmediateData, thisPtr);

		// 0xD-
		table[OpCode::ILLEGAL_D3] = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_DB] = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_DD] = std::bind(&Cpu::HardLock, thisPtr);

		// 0xE-
		table[OpCode::ILLEGAL_E3] = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_E4] = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_EB] = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_EC] = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_ED] = std::bind(&Cpu::HardLock, thisPtr);

		// 0xF-
		table[OpCode::CP_A_n8] = std::bind(&Cpu::Compare_8bit_High_ImmediateData, thisPtr, &CpuCore::AF);
		table[OpCode::ILLEGAL_F4] = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_FC] = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_FD] = std::bind(&Cpu::HardLock, thisPtr);


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
