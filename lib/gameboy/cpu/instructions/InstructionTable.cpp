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
		table[OpCode::JR_NZ_e8]       = std::bind(&Cpu::Jump_Relative_FlagNotSet_8bit_SignedImmediateData, thisPtr, FlagRegisterFlag::ZERO);
		table[OpCode::LD_HL_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, &CpuCore::HL);
		table[OpCode::LD_HL_INC_NI_A] = std::bind(&Cpu::Store_8bit_AddrIncrement_High, thisPtr, &CpuCore::HL, &CpuCore::AF);
		table[OpCode::INC_HL]         = std::bind(&Cpu::Increment_16bit, thisPtr, &CpuCore::HL);
		table[OpCode::INC_H]          = std::bind(&Cpu::Increment_8bit_High, thisPtr, &CpuCore::HL);
		table[OpCode::DEC_H]          = std::bind(&Cpu::Decrement_8bit_High, thisPtr, &CpuCore::HL);
		table[OpCode::LD_H_n8]        = std::bind(&Cpu::Load_8bit_High_ImmediateData, thisPtr, &CpuCore::HL);
		table[OpCode::DAA]            = std::bind(&Cpu::DDA, thisPtr);
		table[OpCode::JR_Z_e8]        = std::bind(&Cpu::Jump_Relative_FlagSet_8bit_SignedImmediateData, thisPtr, FlagRegisterFlag::ZERO);
		table[OpCode::ADD_HL_HL]      = std::bind(&Cpu::Add_16bit, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_A_HL_INC_NI] = std::bind(&Cpu::Load_8bit_High_AddrIncrement, thisPtr, &CpuCore::AF, &CpuCore::HL);
		table[OpCode::DEC_HL]         = std::bind(&Cpu::Decrement_16bit, thisPtr, &CpuCore::HL);
		table[OpCode::INC_L]          = std::bind(&Cpu::Increment_8bit_Low, thisPtr, &CpuCore::HL);
		table[OpCode::DEC_L]          = std::bind(&Cpu::Decrement_8bit_Low, thisPtr, &CpuCore::HL);
		table[OpCode::LD_L_n8]        = std::bind(&Cpu::Load_8bit_Low_ImmediateData, thisPtr, &CpuCore::HL);
		table[OpCode::CPL]            = std::bind(&Cpu::CPL, thisPtr);

		// 0x3-
		table[OpCode::JR_NC_e8]       = std::bind(&Cpu::Jump_Relative_FlagNotSet_8bit_SignedImmediateData, thisPtr, FlagRegisterFlag::CARRY);
		table[OpCode::LD_SP_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, &CpuCore::SP);
		table[OpCode::LD_HL_DEC_NI_A] = std::bind(&Cpu::Store_8bit_AddrDecrement_High, thisPtr, &CpuCore::HL, &CpuCore::AF);
		table[OpCode::INC_SP]         = std::bind(&Cpu::Increment_16bit, thisPtr, &CpuCore::SP);
		table[OpCode::INC_HL_NI]      = std::bind(&Cpu::Increment_Dereferenced, thisPtr, &CpuCore::HL);
		table[OpCode::DEC_HL_NI]      = std::bind(&Cpu::Decrement_Dereferenced, thisPtr, &CpuCore::HL);
		table[OpCode::LD_HL_NI_n8]    = std::bind(&Cpu::Store_8bit_Addr_ImmediateData, thisPtr, &CpuCore::HL);
		table[OpCode::SCF]            = std::bind(&Cpu::SCF, thisPtr);
		table[OpCode::JR_C_e8]        = std::bind(&Cpu::Jump_Relative_FlagSet_8bit_SignedImmediateData, thisPtr, FlagRegisterFlag::CARRY);
		table[OpCode::ADD_HL_SP]      = std::bind(&Cpu::Add_16bit, thisPtr, &CpuCore::HL, &CpuCore::SP);
		table[OpCode::LD_A_HL_DEC_NI] = std::bind(&Cpu::Load_8bit_High_AddrDecrement, thisPtr, &CpuCore::AF, &CpuCore::HL);
		table[OpCode::DEC_SP]         = std::bind(&Cpu::Decrement_16bit, thisPtr, &CpuCore::SP);
		table[OpCode::INC_A]          = std::bind(&Cpu::Increment_8bit_High, thisPtr, &CpuCore::AF);
		table[OpCode::DEC_A]          = std::bind(&Cpu::Decrement_8bit_High, thisPtr, &CpuCore::AF);
		table[OpCode::LD_A_n8]        = std::bind(&Cpu::Load_8bit_High_ImmediateData, thisPtr, &CpuCore::AF);
		table[OpCode::CCF]            = std::bind(&Cpu::CCF, thisPtr);

		// 0x4-
		table[OpCode::LD_B_B]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::BC, &CpuCore::BC);
		table[OpCode::LD_B_C]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::BC, &CpuCore::BC);
		table[OpCode::LD_B_D]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::BC, &CpuCore::DE);
		table[OpCode::LD_B_E]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::BC, &CpuCore::DE);
		table[OpCode::LD_B_H]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::BC, &CpuCore::HL);
		table[OpCode::LD_B_L]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::BC, &CpuCore::HL);
		table[OpCode::LD_B_HL_NI]     = std::bind(&Cpu::Load_8bit_High_Addr, thisPtr, &CpuCore::BC, &CpuCore::HL);
		table[OpCode::LD_B_A]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::BC, &CpuCore::AF);
		table[OpCode::LD_C_B]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::BC, &CpuCore::BC);
		table[OpCode::LD_C_C]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::BC, &CpuCore::BC);
		table[OpCode::LD_C_D]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::BC, &CpuCore::DE);
		table[OpCode::LD_C_E]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::BC, &CpuCore::DE);
		table[OpCode::LD_C_H]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::BC, &CpuCore::HL);
		table[OpCode::LD_C_L]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::BC, &CpuCore::HL);
		table[OpCode::LD_C_HL_NI]     = std::bind(&Cpu::Load_8bit_Low_Addr, thisPtr, &CpuCore::BC, &CpuCore::HL);
		table[OpCode::LD_C_A]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::BC, &CpuCore::AF);

		// 0x5-
		table[OpCode::LD_D_B]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::DE, &CpuCore::BC);
		table[OpCode::LD_D_C]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::DE, &CpuCore::BC);
		table[OpCode::LD_D_D]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::DE, &CpuCore::DE);
		table[OpCode::LD_D_E]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::DE, &CpuCore::DE);
		table[OpCode::LD_D_H]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::DE, &CpuCore::HL);
		table[OpCode::LD_D_L]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::DE, &CpuCore::HL);
		table[OpCode::LD_D_HL_NI]     = std::bind(&Cpu::Load_8bit_High_Addr, thisPtr, &CpuCore::DE, &CpuCore::HL);
		table[OpCode::LD_D_A]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::DE, &CpuCore::AF);
		table[OpCode::LD_E_B]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::DE, &CpuCore::BC);
		table[OpCode::LD_E_C]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::DE, &CpuCore::BC);
		table[OpCode::LD_E_D]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::DE, &CpuCore::DE);
		table[OpCode::LD_E_E]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::DE, &CpuCore::DE);
		table[OpCode::LD_E_H]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::DE, &CpuCore::HL);
		table[OpCode::LD_E_L]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::DE, &CpuCore::HL);
		table[OpCode::LD_E_HL_NI]     = std::bind(&Cpu::Load_8bit_Low_Addr, thisPtr, &CpuCore::DE, &CpuCore::HL);
		table[OpCode::LD_E_A]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::DE, &CpuCore::AF);

		// 0x6-
		table[OpCode::LD_H_B]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::HL, &CpuCore::BC);
		table[OpCode::LD_H_C]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::HL, &CpuCore::BC);
		table[OpCode::LD_H_D]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::HL, &CpuCore::DE);
		table[OpCode::LD_H_E]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::HL, &CpuCore::DE);
		table[OpCode::LD_H_H]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_H_L]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_H_HL_NI]     = std::bind(&Cpu::Load_8bit_High_Addr, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_H_A]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::HL, &CpuCore::AF);
		table[OpCode::LD_L_B]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::HL, &CpuCore::BC);
		table[OpCode::LD_L_C]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::HL, &CpuCore::BC);
		table[OpCode::LD_L_D]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::HL, &CpuCore::DE);
		table[OpCode::LD_L_E]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::HL, &CpuCore::DE);
		table[OpCode::LD_L_H]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_L_L]         = std::bind(&Cpu::Load_8bit_Low_Low, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_L_HL_NI]     = std::bind(&Cpu::Load_8bit_Low_Addr, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_L_A]         = std::bind(&Cpu::Load_8bit_Low_High, thisPtr, &CpuCore::HL, &CpuCore::AF);

		// 0x7-
		table[OpCode::LD_HL_NI_B]     = std::bind(&Cpu::Store_8bit_Addr_High, thisPtr, &CpuCore::HL, &CpuCore::BC);
		table[OpCode::LD_HL_NI_C]     = std::bind(&Cpu::Store_8bit_Addr_Low, thisPtr, &CpuCore::HL, &CpuCore::BC);
		table[OpCode::LD_HL_NI_D]     = std::bind(&Cpu::Store_8bit_Addr_High, thisPtr, &CpuCore::HL, &CpuCore::DE);
		table[OpCode::LD_HL_NI_E]     = std::bind(&Cpu::Store_8bit_Addr_Low, thisPtr, &CpuCore::HL, &CpuCore::DE);
		table[OpCode::LD_HL_NI_H]     = std::bind(&Cpu::Store_8bit_Addr_High, thisPtr, &CpuCore::HL, &CpuCore::HL);
		table[OpCode::LD_HL_NI_L]     = std::bind(&Cpu::Store_8bit_Addr_Low, thisPtr, &CpuCore::HL, &CpuCore::HL);
			// 0x76
			// TODO: HALT, implement when the interrupts are handled properly
		table[OpCode::LD_HL_NI_A]     = std::bind(&Cpu::Store_8bit_Addr_High, thisPtr, &CpuCore::HL, &CpuCore::AF);
		table[OpCode::LD_A_B]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::AF, &CpuCore::BC);
		table[OpCode::LD_A_C]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::AF, &CpuCore::BC);
		table[OpCode::LD_A_D]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::AF, &CpuCore::DE);
		table[OpCode::LD_A_E]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::AF, &CpuCore::DE);
		table[OpCode::LD_A_H]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::AF, &CpuCore::HL);
		table[OpCode::LD_A_L]         = std::bind(&Cpu::Load_8bit_High_Low, thisPtr, &CpuCore::AF, &CpuCore::HL);
		table[OpCode::LD_A_HL_NI]     = std::bind(&Cpu::Load_8bit_High_Addr, thisPtr, &CpuCore::AF, &CpuCore::HL);
		table[OpCode::LD_A_A]         = std::bind(&Cpu::Load_8bit_High_High, thisPtr, &CpuCore::AF, &CpuCore::AF);

		// 0x8-
		table[OpCode::ADD_A_B]        = std::bind(&Cpu::Add_8bit_High, thisPtr, &CpuCore::BC);
		table[OpCode::ADD_A_C]        = std::bind(&Cpu::Add_8bit_Low, thisPtr, &CpuCore::BC);
		table[OpCode::ADD_A_D]        = std::bind(&Cpu::Add_8bit_High, thisPtr, &CpuCore::DE);
		table[OpCode::ADD_A_E]        = std::bind(&Cpu::Add_8bit_Low, thisPtr, &CpuCore::DE);
		table[OpCode::ADD_A_H]        = std::bind(&Cpu::Add_8bit_High, thisPtr, &CpuCore::HL);
		table[OpCode::ADD_A_L]        = std::bind(&Cpu::Add_8bit_Low, thisPtr, &CpuCore::HL);
		table[OpCode::ADD_A_HL_NI]    = std::bind(&Cpu::Add_8bit_Addr, thisPtr, &CpuCore::HL);
		table[OpCode::ADD_A_A]        = std::bind(&Cpu::Add_8bit_High, thisPtr, &CpuCore::AF);
		table[OpCode::ADC_A_B]        = std::bind(&Cpu::AddCarry_8bit_High, thisPtr, &CpuCore::BC);
		table[OpCode::ADC_A_C]        = std::bind(&Cpu::AddCarry_8bit_Low, thisPtr, &CpuCore::BC);
		table[OpCode::ADC_A_D]        = std::bind(&Cpu::AddCarry_8bit_High, thisPtr, &CpuCore::DE);
		table[OpCode::ADC_A_E]        = std::bind(&Cpu::AddCarry_8bit_Low, thisPtr, &CpuCore::DE);
		table[OpCode::ADC_A_H]        = std::bind(&Cpu::AddCarry_8bit_High, thisPtr, &CpuCore::HL);
		table[OpCode::ADC_A_L]        = std::bind(&Cpu::AddCarry_8bit_Low, thisPtr, &CpuCore::HL);
		table[OpCode::ADC_A_HL_NI]    = std::bind(&Cpu::AddCarry_8bit_Addr, thisPtr, &CpuCore::HL);
		table[OpCode::ADC_A_A]        = std::bind(&Cpu::AddCarry_8bit_High, thisPtr, &CpuCore::AF);

		// 0x9-
		table[OpCode::SUB_A_B]        = std::bind(&Cpu::Sub_8bit_High, thisPtr, &CpuCore::BC);
		table[OpCode::SUB_A_C]        = std::bind(&Cpu::Sub_8bit_Low, thisPtr, &CpuCore::BC);
		table[OpCode::SUB_A_D]        = std::bind(&Cpu::Sub_8bit_High, thisPtr, &CpuCore::DE);
		table[OpCode::SUB_A_E]        = std::bind(&Cpu::Sub_8bit_Low, thisPtr, &CpuCore::DE);
		table[OpCode::SUB_A_H]        = std::bind(&Cpu::Sub_8bit_High, thisPtr, &CpuCore::HL);
		table[OpCode::SUB_A_L]        = std::bind(&Cpu::Sub_8bit_Low, thisPtr, &CpuCore::HL);
		table[OpCode::SUB_A_HL_NI]    = std::bind(&Cpu::Sub_8bit_Addr, thisPtr, &CpuCore::HL);
		table[OpCode::SUB_A_A]        = std::bind(&Cpu::Sub_8bit_High, thisPtr, &CpuCore::AF);
		table[OpCode::SBC_A_B]        = std::bind(&Cpu::SubCarry_8bit_High, thisPtr, &CpuCore::BC);
		table[OpCode::SBC_A_C]        = std::bind(&Cpu::SubCarry_8bit_Low, thisPtr, &CpuCore::BC);
		table[OpCode::SBC_A_D]        = std::bind(&Cpu::SubCarry_8bit_High, thisPtr, &CpuCore::DE);
		table[OpCode::SBC_A_E]        = std::bind(&Cpu::SubCarry_8bit_Low, thisPtr, &CpuCore::DE);
		table[OpCode::SBC_A_H]        = std::bind(&Cpu::SubCarry_8bit_High, thisPtr, &CpuCore::HL);
		table[OpCode::SBC_A_L]        = std::bind(&Cpu::SubCarry_8bit_Low, thisPtr, &CpuCore::HL);
		table[OpCode::SBC_A_HL_NI]    = std::bind(&Cpu::SubCarry_8bit_Addr, thisPtr, &CpuCore::HL);
		table[OpCode::SBC_A_A]        = std::bind(&Cpu::SubCarry_8bit_High, thisPtr, &CpuCore::AF);

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
