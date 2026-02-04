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
		#define RegisterBC &CpuCore::BC
		#define RegisterDE &CpuCore::DE
		#define RegisterHL &CpuCore::HL
		#define RegisterAF &CpuCore::AF
		#define RegisterSP &CpuCore::SP

		#define GetB RegisterBC, &Register::HighByte
		#define GetC RegisterBC, &Register::LowByte
		#define GetD RegisterDE, &Register::HighByte
		#define GetE RegisterDE, &Register::LowByte
		#define GetH RegisterHL, &Register::HighByte
		#define GetL RegisterHL, &Register::LowByte
		#define GetA RegisterAF, &Register::HighByte

		#define SetB RegisterBC, &Register::SetHighByte
		#define SetC RegisterBC, &Register::SetLowByte
		#define SetD RegisterDE, &Register::SetHighByte
		#define SetE RegisterDE, &Register::SetLowByte
		#define SetH RegisterHL, &Register::SetHighByte
		#define SetL RegisterHL, &Register::SetLowByte
		#define SetA RegisterAF, &Register::SetHighByte

		#define GetSetB RegisterBC, &Register::HighByte, &Register::SetHighByte
		#define GetSetC RegisterBC, &Register::LowByte, &Register::SetLowByte
		#define GetSetD RegisterDE, &Register::HighByte, &Register::SetHighByte
		#define GetSetE RegisterDE, &Register::LowByte, &Register::SetLowByte
		#define GetSetH RegisterHL, &Register::HighByte, &Register::SetHighByte
		#define GetSetL RegisterHL, &Register::LowByte, &Register::SetLowByte
		#define GetSetA RegisterAF, &Register::HighByte, &Register::SetHighByte

		constexpr auto thisPtr = std::placeholders::_1;
		auto& table = instructions;

		// 0x0-
		table[OpCode::NOP]            = std::bind(&Cpu::NOP, thisPtr);
		table[OpCode::LD_BC_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, RegisterBC);
		table[OpCode::LD_BC_NI_A]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterBC, GetA);
		table[OpCode::INC_BC]         = std::bind(&Cpu::Increment_16bit, thisPtr, RegisterBC);
		table[OpCode::INC_B]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetB);
		table[OpCode::DEC_B]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetB);
		table[OpCode::LD_B_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetB);
		table[OpCode::RLCA]           = std::bind(&Cpu::Rotate_8bit_Left, thisPtr, GetSetA);
		table[OpCode::LD_a16_NI_SP]   = std::bind(&Cpu::Load_16bit_RegToImmediateAddr, thisPtr, RegisterSP);
		table[OpCode::ADD_HL_BC]      = std::bind(&Cpu::Add_16bit, thisPtr, RegisterHL, RegisterBC);
		table[OpCode::LD_A_BC_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetA, RegisterBC);
		table[OpCode::DEC_BC]         = std::bind(&Cpu::Decrement_16bit, thisPtr, RegisterBC);
		table[OpCode::INC_C]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetC);
		table[OpCode::DEC_C]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetC);
		table[OpCode::LD_C_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetC);
		table[OpCode::RRCA]           = std::bind(&Cpu::Rotate_8bit_Right, thisPtr, GetSetA);

		// 0x1-
			// 0x10
			// TODO: STOP, implement when PPU/LCD and input register have been implemented;
			// https://gbdev.io/pandocs/Reducing_Power_Consumption.html#using-the-stop-instruction
		table[OpCode::LD_DE_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, RegisterDE);
		table[OpCode::LD_DE_NI_A]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterDE, GetA);
		table[OpCode::INC_DE]         = std::bind(&Cpu::Increment_16bit, thisPtr, RegisterDE);
		table[OpCode::INC_D]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetD);
		table[OpCode::DEC_D]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetD);
		table[OpCode::LD_D_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetD);
		table[OpCode::RLA]            = std::bind(&Cpu::Rotate_8bit_Left_Carry, thisPtr, GetSetA);
		table[OpCode::JR_e8]          = std::bind(&Cpu::Jump_Relative_8bit_SignedImmediateData, thisPtr);
		table[OpCode::ADD_HL_DE]      = std::bind(&Cpu::Add_16bit, thisPtr, RegisterHL, RegisterDE);
		table[OpCode::LD_A_DE_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetA, RegisterDE);
		table[OpCode::DEC_DE]         = std::bind(&Cpu::Decrement_16bit, thisPtr, RegisterDE);
		table[OpCode::INC_E]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetE);
		table[OpCode::DEC_E]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetE);
		table[OpCode::LD_E_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetE);
		table[OpCode::RRA]            = std::bind(&Cpu::Rotate_8bit_Right_Carry, thisPtr, GetSetA);

		// 0x2-
		table[OpCode::JR_NZ_e8]       = std::bind(&Cpu::Jump_Relative_Conditional_8bit_SignedImmediateData, thisPtr, FlagRegisterFlag::ZERO, false);
		table[OpCode::LD_HL_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, RegisterHL);
		table[OpCode::LD_HL_INC_NI_A] = std::bind(&Cpu::Store_8bit_AddrIncrement, thisPtr, RegisterHL, GetA);
		table[OpCode::INC_HL]         = std::bind(&Cpu::Increment_16bit, thisPtr, RegisterHL);
		table[OpCode::INC_H]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetH);
		table[OpCode::DEC_H]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetH);
		table[OpCode::LD_H_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetH);
		table[OpCode::DAA]            = std::bind(&Cpu::DDA, thisPtr);
		table[OpCode::JR_Z_e8]        = std::bind(&Cpu::Jump_Relative_Conditional_8bit_SignedImmediateData, thisPtr, FlagRegisterFlag::ZERO, true);
		table[OpCode::ADD_HL_HL]      = std::bind(&Cpu::Add_16bit, thisPtr, RegisterHL, RegisterHL);
		table[OpCode::LD_A_HL_INC_NI] = std::bind(&Cpu::Load_8bit_AddrIncrement, thisPtr, SetA, RegisterHL);
		table[OpCode::DEC_HL]         = std::bind(&Cpu::Decrement_16bit, thisPtr, RegisterHL);
		table[OpCode::INC_L]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetL);
		table[OpCode::DEC_L]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetL);
		table[OpCode::LD_L_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetL);
		table[OpCode::CPL]            = std::bind(&Cpu::CPL, thisPtr);

		// 0x3-
		table[OpCode::JR_NC_e8]       = std::bind(&Cpu::Jump_Relative_Conditional_8bit_SignedImmediateData, thisPtr, FlagRegisterFlag::CARRY, false);
		table[OpCode::LD_SP_n16]      = std::bind(&Cpu::Load_16bit_ImmediateData, thisPtr, RegisterSP);
		table[OpCode::LD_HL_DEC_NI_A] = std::bind(&Cpu::Store_8bit_AddrDecrement, thisPtr, RegisterHL, GetA);
		table[OpCode::INC_SP]         = std::bind(&Cpu::Increment_16bit, thisPtr, RegisterSP);
		table[OpCode::INC_HL_NI]      = std::bind(&Cpu::Increment_Dereferenced, thisPtr, RegisterHL);
		table[OpCode::DEC_HL_NI]      = std::bind(&Cpu::Decrement_Dereferenced, thisPtr, RegisterHL);
		table[OpCode::LD_HL_NI_n8]    = std::bind(&Cpu::Store_8bit_Addr_ImmediateData, thisPtr, RegisterHL);
		table[OpCode::SCF]            = std::bind(&Cpu::SCF, thisPtr);
		table[OpCode::JR_C_e8]        = std::bind(&Cpu::Jump_Relative_Conditional_8bit_SignedImmediateData, thisPtr, FlagRegisterFlag::CARRY, true);
		table[OpCode::ADD_HL_SP]      = std::bind(&Cpu::Add_16bit, thisPtr, RegisterHL, RegisterSP);
		table[OpCode::LD_A_HL_DEC_NI] = std::bind(&Cpu::Load_8bit_AddrDecrement, thisPtr, SetA, RegisterHL);
		table[OpCode::DEC_SP]         = std::bind(&Cpu::Decrement_16bit, thisPtr, RegisterSP);
		table[OpCode::INC_A]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetA);
		table[OpCode::DEC_A]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetA);
		table[OpCode::LD_A_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetA);
		table[OpCode::CCF]            = std::bind(&Cpu::CCF, thisPtr);

		// 0x4-
		table[OpCode::LD_B_B]         = std::bind(&Cpu::Load_8bit, thisPtr, SetB, GetB);
		table[OpCode::LD_B_C]         = std::bind(&Cpu::Load_8bit, thisPtr, SetB, GetC);
		table[OpCode::LD_B_D]         = std::bind(&Cpu::Load_8bit, thisPtr, SetB, GetD);
		table[OpCode::LD_B_E]         = std::bind(&Cpu::Load_8bit, thisPtr, SetB, GetE);
		table[OpCode::LD_B_H]         = std::bind(&Cpu::Load_8bit, thisPtr, SetB, GetH);
		table[OpCode::LD_B_L]         = std::bind(&Cpu::Load_8bit, thisPtr, SetB, GetL);
		table[OpCode::LD_B_HL_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetB, RegisterHL);
		table[OpCode::LD_B_A]         = std::bind(&Cpu::Load_8bit, thisPtr, SetB, GetA);
		table[OpCode::LD_C_B]         = std::bind(&Cpu::Load_8bit, thisPtr, SetC, GetB);
		table[OpCode::LD_C_C]         = std::bind(&Cpu::Load_8bit, thisPtr, SetC, GetC);
		table[OpCode::LD_C_D]         = std::bind(&Cpu::Load_8bit, thisPtr, SetC, GetD);
		table[OpCode::LD_C_E]         = std::bind(&Cpu::Load_8bit, thisPtr, SetC, GetE);
		table[OpCode::LD_C_H]         = std::bind(&Cpu::Load_8bit, thisPtr, SetC, GetH);
		table[OpCode::LD_C_L]         = std::bind(&Cpu::Load_8bit, thisPtr, SetC, GetL);
		table[OpCode::LD_C_HL_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetC, RegisterHL);
		table[OpCode::LD_C_A]         = std::bind(&Cpu::Load_8bit, thisPtr, SetC, GetA);

		// 0x5-
		table[OpCode::LD_D_B]         = std::bind(&Cpu::Load_8bit, thisPtr, SetD, GetB);
		table[OpCode::LD_D_C]         = std::bind(&Cpu::Load_8bit, thisPtr, SetD, GetC);
		table[OpCode::LD_D_D]         = std::bind(&Cpu::Load_8bit, thisPtr, SetD, GetD);
		table[OpCode::LD_D_E]         = std::bind(&Cpu::Load_8bit, thisPtr, SetD, GetE);
		table[OpCode::LD_D_H]         = std::bind(&Cpu::Load_8bit, thisPtr, SetD, GetH);
		table[OpCode::LD_D_L]         = std::bind(&Cpu::Load_8bit, thisPtr, SetD, GetL);
		table[OpCode::LD_D_HL_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetD, RegisterHL);
		table[OpCode::LD_D_A]         = std::bind(&Cpu::Load_8bit, thisPtr, SetD, GetA);
		table[OpCode::LD_E_B]         = std::bind(&Cpu::Load_8bit, thisPtr, SetE, GetB);
		table[OpCode::LD_E_C]         = std::bind(&Cpu::Load_8bit, thisPtr, SetE, GetC);
		table[OpCode::LD_E_D]         = std::bind(&Cpu::Load_8bit, thisPtr, SetE, GetD);
		table[OpCode::LD_E_E]         = std::bind(&Cpu::Load_8bit, thisPtr, SetE, GetE);
		table[OpCode::LD_E_H]         = std::bind(&Cpu::Load_8bit, thisPtr, SetE, GetH);
		table[OpCode::LD_E_L]         = std::bind(&Cpu::Load_8bit, thisPtr, SetE, GetL);
		table[OpCode::LD_E_HL_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetE, RegisterHL);
		table[OpCode::LD_E_A]         = std::bind(&Cpu::Load_8bit, thisPtr, SetE, GetA);

		// 0x6-
		table[OpCode::LD_H_B]         = std::bind(&Cpu::Load_8bit, thisPtr, SetH, GetB);
		table[OpCode::LD_H_C]         = std::bind(&Cpu::Load_8bit, thisPtr, SetH, GetC);
		table[OpCode::LD_H_D]         = std::bind(&Cpu::Load_8bit, thisPtr, SetH, GetD);
		table[OpCode::LD_H_E]         = std::bind(&Cpu::Load_8bit, thisPtr, SetH, GetE);
		table[OpCode::LD_H_H]         = std::bind(&Cpu::Load_8bit, thisPtr, SetH, GetH);
		table[OpCode::LD_H_L]         = std::bind(&Cpu::Load_8bit, thisPtr, SetH, GetL);
		table[OpCode::LD_H_HL_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetH, RegisterHL);
		table[OpCode::LD_H_A]         = std::bind(&Cpu::Load_8bit, thisPtr, SetH, GetA);
		table[OpCode::LD_L_B]         = std::bind(&Cpu::Load_8bit, thisPtr, SetL, GetB);
		table[OpCode::LD_L_C]         = std::bind(&Cpu::Load_8bit, thisPtr, SetL, GetC);
		table[OpCode::LD_L_D]         = std::bind(&Cpu::Load_8bit, thisPtr, SetL, GetD);
		table[OpCode::LD_L_E]         = std::bind(&Cpu::Load_8bit, thisPtr, SetL, GetE);
		table[OpCode::LD_L_H]         = std::bind(&Cpu::Load_8bit, thisPtr, SetL, GetH);
		table[OpCode::LD_L_L]         = std::bind(&Cpu::Load_8bit, thisPtr, SetL, GetL);
		table[OpCode::LD_L_HL_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetL, RegisterHL);
		table[OpCode::LD_L_A]         = std::bind(&Cpu::Load_8bit, thisPtr, SetL, GetA);

		// 0x7-
		table[OpCode::LD_HL_NI_B]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterHL, GetB);
		table[OpCode::LD_HL_NI_C]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterHL, GetC);
		table[OpCode::LD_HL_NI_D]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterHL, GetD);
		table[OpCode::LD_HL_NI_E]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterHL, GetE);
		table[OpCode::LD_HL_NI_H]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterHL, GetH);
		table[OpCode::LD_HL_NI_L]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterHL, GetL);
			// 0x76
			// TODO: HALT, implement when the interrupts are handled properly
		table[OpCode::LD_HL_NI_A]     = std::bind(&Cpu::Store_8bit_Addr, thisPtr, RegisterHL, GetA);
		table[OpCode::LD_A_B]         = std::bind(&Cpu::Load_8bit, thisPtr, SetA, GetB);
		table[OpCode::LD_A_C]         = std::bind(&Cpu::Load_8bit, thisPtr, SetA, GetC);
		table[OpCode::LD_A_D]         = std::bind(&Cpu::Load_8bit, thisPtr, SetA, GetD);
		table[OpCode::LD_A_E]         = std::bind(&Cpu::Load_8bit, thisPtr, SetA, GetE);
		table[OpCode::LD_A_H]         = std::bind(&Cpu::Load_8bit, thisPtr, SetA, GetH);
		table[OpCode::LD_A_L]         = std::bind(&Cpu::Load_8bit, thisPtr, SetA, GetL);
		table[OpCode::LD_A_HL_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetA, RegisterHL);
		table[OpCode::LD_A_A]         = std::bind(&Cpu::Load_8bit, thisPtr, SetA, GetA);

		// 0x8-
		table[OpCode::ADD_A_B]        = std::bind(&Cpu::Add_8bit, thisPtr, GetB);
		table[OpCode::ADD_A_C]        = std::bind(&Cpu::Add_8bit, thisPtr, GetC);
		table[OpCode::ADD_A_D]        = std::bind(&Cpu::Add_8bit, thisPtr, GetD);
		table[OpCode::ADD_A_E]        = std::bind(&Cpu::Add_8bit, thisPtr, GetE);
		table[OpCode::ADD_A_H]        = std::bind(&Cpu::Add_8bit, thisPtr, GetH);
		table[OpCode::ADD_A_L]        = std::bind(&Cpu::Add_8bit, thisPtr, GetL);
		table[OpCode::ADD_A_HL_NI]    = std::bind(&Cpu::Add_8bit_Addr, thisPtr, RegisterHL);
		table[OpCode::ADD_A_A]        = std::bind(&Cpu::Add_8bit, thisPtr, GetA);
		table[OpCode::ADC_A_B]        = std::bind(&Cpu::AddCarry_8bit, thisPtr, GetB);
		table[OpCode::ADC_A_C]        = std::bind(&Cpu::AddCarry_8bit, thisPtr, GetC);
		table[OpCode::ADC_A_D]        = std::bind(&Cpu::AddCarry_8bit, thisPtr, GetD);
		table[OpCode::ADC_A_E]        = std::bind(&Cpu::AddCarry_8bit, thisPtr, GetE);
		table[OpCode::ADC_A_H]        = std::bind(&Cpu::AddCarry_8bit, thisPtr, GetH);
		table[OpCode::ADC_A_L]        = std::bind(&Cpu::AddCarry_8bit, thisPtr, GetL);
		table[OpCode::ADC_A_HL_NI]    = std::bind(&Cpu::AddCarry_8bit_Addr, thisPtr, RegisterHL);
		table[OpCode::ADC_A_A]        = std::bind(&Cpu::AddCarry_8bit, thisPtr, GetA);

		// 0x9-
		table[OpCode::SUB_A_B]        = std::bind(&Cpu::Sub_8bit, thisPtr, GetB);
		table[OpCode::SUB_A_C]        = std::bind(&Cpu::Sub_8bit, thisPtr, GetC);
		table[OpCode::SUB_A_D]        = std::bind(&Cpu::Sub_8bit, thisPtr, GetD);
		table[OpCode::SUB_A_E]        = std::bind(&Cpu::Sub_8bit, thisPtr, GetE);
		table[OpCode::SUB_A_H]        = std::bind(&Cpu::Sub_8bit, thisPtr, GetH);
		table[OpCode::SUB_A_L]        = std::bind(&Cpu::Sub_8bit, thisPtr, GetL);
		table[OpCode::SUB_A_HL_NI]    = std::bind(&Cpu::Sub_8bit_Addr, thisPtr, RegisterHL);
		table[OpCode::SUB_A_A]        = std::bind(&Cpu::Sub_8bit, thisPtr, GetA);
		table[OpCode::SBC_A_B]        = std::bind(&Cpu::SubCarry_8bit, thisPtr, GetB);
		table[OpCode::SBC_A_C]        = std::bind(&Cpu::SubCarry_8bit, thisPtr, GetC);
		table[OpCode::SBC_A_D]        = std::bind(&Cpu::SubCarry_8bit, thisPtr, GetD);
		table[OpCode::SBC_A_E]        = std::bind(&Cpu::SubCarry_8bit, thisPtr, GetE);
		table[OpCode::SBC_A_H]        = std::bind(&Cpu::SubCarry_8bit, thisPtr, GetH);
		table[OpCode::SBC_A_L]        = std::bind(&Cpu::SubCarry_8bit, thisPtr, GetL);
		table[OpCode::SBC_A_HL_NI]    = std::bind(&Cpu::SubCarry_8bit_Addr, thisPtr, RegisterHL);
		table[OpCode::SBC_A_A]        = std::bind(&Cpu::SubCarry_8bit, thisPtr, GetA);

		// 0xA-
		table[OpCode::AND_A_B]        = std::bind(&Cpu::BitwiseAnd, thisPtr, GetB);
		table[OpCode::AND_A_C]        = std::bind(&Cpu::BitwiseAnd, thisPtr, GetC);
		table[OpCode::AND_A_D]        = std::bind(&Cpu::BitwiseAnd, thisPtr, GetD);
		table[OpCode::AND_A_E]        = std::bind(&Cpu::BitwiseAnd, thisPtr, GetE);
		table[OpCode::AND_A_H]        = std::bind(&Cpu::BitwiseAnd, thisPtr, GetH);
		table[OpCode::AND_A_L]        = std::bind(&Cpu::BitwiseAnd, thisPtr, GetL);
		table[OpCode::AND_A_HL_NI]    = std::bind(&Cpu::BitwiseAnd_Addr, thisPtr, RegisterHL);
		table[OpCode::AND_A_A]        = std::bind(&Cpu::BitwiseAnd, thisPtr, GetA);
		table[OpCode::XOR_A_B]        = std::bind(&Cpu::BitwiseXor, thisPtr, GetB);
		table[OpCode::XOR_A_C]        = std::bind(&Cpu::BitwiseXor, thisPtr, GetC);
		table[OpCode::XOR_A_D]        = std::bind(&Cpu::BitwiseXor, thisPtr, GetD);
		table[OpCode::XOR_A_E]        = std::bind(&Cpu::BitwiseXor, thisPtr, GetE);
		table[OpCode::XOR_A_H]        = std::bind(&Cpu::BitwiseXor, thisPtr, GetH);
		table[OpCode::XOR_A_L]        = std::bind(&Cpu::BitwiseXor, thisPtr, GetL);
		table[OpCode::XOR_A_HL_NI]    = std::bind(&Cpu::BitwiseXor_Addr, thisPtr, RegisterHL);
		table[OpCode::XOR_A_A]        = std::bind(&Cpu::BitwiseXor, thisPtr, GetA);

		// 0xB-
		table[OpCode::OR_A_B]        = std::bind(&Cpu::BitwiseOr, thisPtr, GetB);
		table[OpCode::OR_A_C]        = std::bind(&Cpu::BitwiseOr, thisPtr, GetC);
		table[OpCode::OR_A_D]        = std::bind(&Cpu::BitwiseOr, thisPtr, GetD);
		table[OpCode::OR_A_E]        = std::bind(&Cpu::BitwiseOr, thisPtr, GetE);
		table[OpCode::OR_A_H]        = std::bind(&Cpu::BitwiseOr, thisPtr, GetH);
		table[OpCode::OR_A_L]        = std::bind(&Cpu::BitwiseOr, thisPtr, GetL);
		table[OpCode::OR_A_HL_NI]    = std::bind(&Cpu::BitwiseOr_Addr, thisPtr, RegisterHL);
		table[OpCode::OR_A_A]        = std::bind(&Cpu::BitwiseOr, thisPtr, GetA);
		table[OpCode::CP_A_B]        = std::bind(&Cpu::Compare_8bit, thisPtr, GetB);
		table[OpCode::CP_A_C]        = std::bind(&Cpu::Compare_8bit, thisPtr, GetC);
		table[OpCode::CP_A_D]        = std::bind(&Cpu::Compare_8bit, thisPtr, GetD);
		table[OpCode::CP_A_E]        = std::bind(&Cpu::Compare_8bit, thisPtr, GetE);
		table[OpCode::CP_A_H]        = std::bind(&Cpu::Compare_8bit, thisPtr, GetH);
		table[OpCode::CP_A_L]        = std::bind(&Cpu::Compare_8bit, thisPtr, GetL);
		table[OpCode::CP_A_HL_NI]    = std::bind(&Cpu::Compare_8bit_Addr, thisPtr, RegisterHL);
		table[OpCode::CP_A_A]        = std::bind(&Cpu::Compare_8bit, thisPtr, GetA);

		// 0xC-
		table[OpCode::RET_NZ]    = std::bind(&Cpu::ConditionalReturn, thisPtr, FlagRegisterFlag::ZERO, false);
		table[OpCode::POP_BC]    = std::bind(&Cpu::Pop, thisPtr, RegisterBC);
		table[OpCode::JP_a16]    = std::bind(&Cpu::Jump_16bit_ImmediateData, thisPtr);

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
		table[OpCode::CP_A_n8] = std::bind(&Cpu::Compare_8bit_ImmediateData, thisPtr);
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
