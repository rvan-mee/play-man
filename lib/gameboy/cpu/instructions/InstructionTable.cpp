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

		#define Bit0 0b00000001
		#define Bit1 0b00000010
		#define Bit2 0b00000100
		#define Bit3 0b00001000
		#define Bit4 0b00010000
		#define Bit5 0b00100000
		#define Bit6 0b01000000
		#define Bit7 0b10000000

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
		table[OpCode::RLCA]           = std::bind(&Cpu::NoPrefixRotateLeftCarry, thisPtr, GetSetA);
		table[OpCode::LD_a16_NI_SP]   = std::bind(&Cpu::Load_16bit_RegToImmediateAddr, thisPtr, RegisterSP);
		table[OpCode::ADD_HL_BC]      = std::bind(&Cpu::Add_16bit, thisPtr, RegisterHL, RegisterBC);
		table[OpCode::LD_A_BC_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetA, RegisterBC);
		table[OpCode::DEC_BC]         = std::bind(&Cpu::Decrement_16bit, thisPtr, RegisterBC);
		table[OpCode::INC_C]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetC);
		table[OpCode::DEC_C]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetC);
		table[OpCode::LD_C_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetC);
		table[OpCode::RRCA]           = std::bind(&Cpu::NoPrefixRotateRightCarry, thisPtr, GetSetA);

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
		table[OpCode::RLA]            = std::bind(&Cpu::NoPrefixRotateLeft, thisPtr, GetSetA);
		table[OpCode::JR_e8]          = std::bind(&Cpu::Jump_Relative_8bit_SignedImmediateData, thisPtr);
		table[OpCode::ADD_HL_DE]      = std::bind(&Cpu::Add_16bit, thisPtr, RegisterHL, RegisterDE);
		table[OpCode::LD_A_DE_NI]     = std::bind(&Cpu::Load_8bit_Addr, thisPtr, SetA, RegisterDE);
		table[OpCode::DEC_DE]         = std::bind(&Cpu::Decrement_16bit, thisPtr, RegisterDE);
		table[OpCode::INC_E]          = std::bind(&Cpu::Increment_8bit, thisPtr, GetSetE);
		table[OpCode::DEC_E]          = std::bind(&Cpu::Decrement_8bit, thisPtr, GetSetE);
		table[OpCode::LD_E_n8]        = std::bind(&Cpu::Load_8bit_ImmediateData, thisPtr, SetE);
		table[OpCode::RRA]            = std::bind(&Cpu::NoPrefixRotateRight, thisPtr, GetSetA);

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
		table[OpCode::RET_NZ]      = std::bind(&Cpu::ConditionalReturn, thisPtr, FlagRegisterFlag::ZERO, false);
		table[OpCode::POP_BC]      = std::bind(&Cpu::Pop, thisPtr, RegisterBC);
		table[OpCode::JP_NZ_a16]   = std::bind(&Cpu::Jump_Conditional_16bit_ImmediateData, thisPtr, FlagRegisterFlag::ZERO, false);
		table[OpCode::JP_a16]      = std::bind(&Cpu::Jump_16bit_ImmediateData, thisPtr);
		table[OpCode::CALL_NZ_a16] = std::bind(&Cpu::ConditionalCall_16bit_ImmediateData, thisPtr, FlagRegisterFlag::ZERO, false);
		table[OpCode::PUSH_BC]     = std::bind(&Cpu::Push, thisPtr, RegisterBC);
		table[OpCode::ADD_A_n8]    = std::bind(&Cpu::Add_8bit_ImmediateData, thisPtr);
		table[OpCode::RST_00]      = std::bind(&Cpu::RST, thisPtr, 0x00);
		table[OpCode::RET_Z]       = std::bind(&Cpu::ConditionalReturn, thisPtr, FlagRegisterFlag::ZERO, true);
		table[OpCode::RET]         = std::bind(&Cpu::Return, thisPtr);
		table[OpCode::JP_Z_a16]    = std::bind(&Cpu::Jump_Conditional_16bit_ImmediateData, thisPtr, FlagRegisterFlag::ZERO, true);
		// 0xCB reserved for prefixed instructions
		table[OpCode::CALL_Z_a16]  = std::bind(&Cpu::ConditionalCall_16bit_ImmediateData, thisPtr, FlagRegisterFlag::ZERO, true);
		table[OpCode::CALL_a16]    = std::bind(&Cpu::Call_16bit_ImmediateData, thisPtr);
		table[OpCode::ADC_A_n8]    = std::bind(&Cpu::AddCarry_8bit_ImmediateData, thisPtr);
		table[OpCode::RST_08]      = std::bind(&Cpu::RST, thisPtr, 0x08);

		// 0xD-
		table[OpCode::RET_NC]      = std::bind(&Cpu::ConditionalReturn, thisPtr, FlagRegisterFlag::CARRY, false);
		table[OpCode::POP_DE]      = std::bind(&Cpu::Pop, thisPtr, RegisterDE);
		table[OpCode::JP_NC_a16]   = std::bind(&Cpu::Jump_Conditional_16bit_ImmediateData, thisPtr, FlagRegisterFlag::CARRY, false);
		table[OpCode::ILLEGAL_D3]  = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::CALL_NC_a16] = std::bind(&Cpu::ConditionalCall_16bit_ImmediateData, thisPtr, FlagRegisterFlag::CARRY, false);
		table[OpCode::PUSH_DE]     = std::bind(&Cpu::Push, thisPtr, RegisterDE);
		table[OpCode::SUB_A_n8]    = std::bind(&Cpu::Sub_8bit_ImmediateData, thisPtr);
		table[OpCode::RST_10]      = std::bind(&Cpu::RST, thisPtr, 0x10);
		table[OpCode::RET_C]       = std::bind(&Cpu::ConditionalReturn, thisPtr, FlagRegisterFlag::CARRY, true);
		// 0xD9 - RETI
		//        Implement when interrupts have been handled
		table[OpCode::JP_C_a16]    = std::bind(&Cpu::Jump_Conditional_16bit_ImmediateData, thisPtr, FlagRegisterFlag::CARRY, true);
		table[OpCode::ILLEGAL_DB]  = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::CALL_C_a16]  = std::bind(&Cpu::ConditionalCall_16bit_ImmediateData, thisPtr, FlagRegisterFlag::CARRY, true); 
		table[OpCode::ILLEGAL_DD]  = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::SBC_A_n8]    = std::bind(&Cpu::SubCarry_8bit_ImmediateData, thisPtr);
		table[OpCode::RST_18]      = std::bind(&Cpu::RST, thisPtr, 0x18);

		// 0xE-
		table[OpCode::LDH_a8_NI_A] = std::bind(&Cpu::Store_8bit_8bitImmediateAddr, thisPtr, GetA);
		table[OpCode::POP_HL]      = std::bind(&Cpu::Pop, thisPtr, RegisterHL);
		table[OpCode::LDH_C_NI_A]  = std::bind(&Cpu::Store_8bit_8bitAddr, thisPtr, GetC, GetA);
		table[OpCode::ILLEGAL_E3]  = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_E4]  = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::PUSH_HL]     = std::bind(&Cpu::Push, thisPtr, RegisterHL);
		table[OpCode::AND_A_n8]    = std::bind(&Cpu::BitwiseAnd_ImmediateData, thisPtr);
		table[OpCode::RST_20]      = std::bind(&Cpu::RST, thisPtr, 0x20);
		table[OpCode::ADD_SP_e8]   = std::bind(&Cpu::Add_16bit_8bitSignedImmediateData, thisPtr, RegisterSP);
		table[OpCode::JP_HL]       = std::bind(&Cpu::Jump_Addr, thisPtr, RegisterHL);
		table[OpCode::LD_a16_NI_A] = std::bind(&Cpu::Store_8bit_16bitImmediateAddr, thisPtr, GetA);
		table[OpCode::ILLEGAL_EB]  = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_EC]  = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_ED]  = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::XOR_A_n8]    = std::bind(&Cpu::BitwiseXor_ImmediateData, thisPtr);
		table[OpCode::RST_28]      = std::bind(&Cpu::RST, thisPtr, 0x28);

		// 0xF-
		table[OpCode::LDH_A_a8_NI]     = std::bind(&Cpu::Load_8bit_8bitImmediateAddr, thisPtr, SetA);
		table[OpCode::POP_AF]          = std::bind(&Cpu::Pop, thisPtr, RegisterAF);
		table[OpCode::LDH_A_C_NI]      = std::bind(&Cpu::Load_8bit_8bitAddr, thisPtr, SetA, GetC);
		// TODO: 0xF3 - DI
		// Implement when interrupts are handled.
		table[OpCode::ILLEGAL_F4]      = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::PUSH_AF]         = std::bind(&Cpu::Push, thisPtr, RegisterAF);
		table[OpCode::OR_A_n8]         = std::bind(&Cpu::BitwiseOr_ImmediateData, thisPtr);
		table[OpCode::RST_30]          = std::bind(&Cpu::RST, thisPtr, 0x30);
		table[OpCode::LD_HL_SP_INC_e8] = std::bind(&Cpu::Store_StackPointerPlusSignedImmediateData, thisPtr, RegisterHL);
		table[OpCode::LD_SP_HL]        = std::bind(&Cpu::Load_16bit, thisPtr, RegisterSP, RegisterHL);
		table[OpCode::LD_A_a16_NI]     = std::bind(&Cpu::Load_8bit_ImmediateAddr, thisPtr, SetA);
		// TODO: 0xFB - EI
		// Implement when interrupts are handled.
		table[OpCode::ILLEGAL_FC]      = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::ILLEGAL_FD]      = std::bind(&Cpu::HardLock, thisPtr);
		table[OpCode::CP_A_n8]         = std::bind(&Cpu::Compare_8bit_ImmediateData, thisPtr);
		table[OpCode::RST_38]          = std::bind(&Cpu::RST, thisPtr, 0x38);


		/** Prefixed instructions **/

		auto& preTable = prefixedInstructions;
		using PreOpCode = PrefixedOpCode;

		// 0x0-
		preTable[PreOpCode::RLC_B]     = std::bind(&Cpu::RotateLeftCarry, thisPtr, GetSetB);
		preTable[PreOpCode::RLC_C]     = std::bind(&Cpu::RotateLeftCarry, thisPtr, GetSetC);
		preTable[PreOpCode::RLC_D]     = std::bind(&Cpu::RotateLeftCarry, thisPtr, GetSetD);
		preTable[PreOpCode::RLC_E]     = std::bind(&Cpu::RotateLeftCarry, thisPtr, GetSetE);
		preTable[PreOpCode::RLC_H]     = std::bind(&Cpu::RotateLeftCarry, thisPtr, GetSetH);
		preTable[PreOpCode::RLC_L]     = std::bind(&Cpu::RotateLeftCarry, thisPtr, GetSetL);
		preTable[PreOpCode::RLC_HL_NI] = std::bind(&Cpu::RotateLeftCarry_Addr, thisPtr, RegisterHL);
		preTable[PreOpCode::RLC_A]     = std::bind(&Cpu::RotateLeftCarry, thisPtr, GetSetA);
		preTable[PreOpCode::RRC_B]     = std::bind(&Cpu::RotateRightCarry, thisPtr, GetSetB);
		preTable[PreOpCode::RRC_C]     = std::bind(&Cpu::RotateRightCarry, thisPtr, GetSetC);
		preTable[PreOpCode::RRC_D]     = std::bind(&Cpu::RotateRightCarry, thisPtr, GetSetD);
		preTable[PreOpCode::RRC_E]     = std::bind(&Cpu::RotateRightCarry, thisPtr, GetSetE);
		preTable[PreOpCode::RRC_H]     = std::bind(&Cpu::RotateRightCarry, thisPtr, GetSetH);
		preTable[PreOpCode::RRC_L]     = std::bind(&Cpu::RotateRightCarry, thisPtr, GetSetL);
		preTable[PreOpCode::RRC_HL_NI] = std::bind(&Cpu::RotateRightCarry_Addr, thisPtr, RegisterHL);
		preTable[PreOpCode::RRC_A]     = std::bind(&Cpu::RotateRightCarry, thisPtr, GetSetA);

		// 0x1-
		preTable[PreOpCode::RL_B]     = std::bind(&Cpu::RotateLeft, thisPtr, GetSetB);
		preTable[PreOpCode::RL_C]     = std::bind(&Cpu::RotateLeft, thisPtr, GetSetC);
		preTable[PreOpCode::RL_D]     = std::bind(&Cpu::RotateLeft, thisPtr, GetSetD);
		preTable[PreOpCode::RL_E]     = std::bind(&Cpu::RotateLeft, thisPtr, GetSetE);
		preTable[PreOpCode::RL_H]     = std::bind(&Cpu::RotateLeft, thisPtr, GetSetH);
		preTable[PreOpCode::RL_L]     = std::bind(&Cpu::RotateLeft, thisPtr, GetSetL);
		preTable[PreOpCode::RL_HL_NI] = std::bind(&Cpu::RotateLeft_Addr, thisPtr, RegisterHL);
		preTable[PreOpCode::RL_A]     = std::bind(&Cpu::RotateLeft, thisPtr, GetSetA);
		preTable[PreOpCode::RR_B]     = std::bind(&Cpu::RotateRight, thisPtr, GetSetB);
		preTable[PreOpCode::RR_C]     = std::bind(&Cpu::RotateRight, thisPtr, GetSetC);
		preTable[PreOpCode::RR_D]     = std::bind(&Cpu::RotateRight, thisPtr, GetSetD);
		preTable[PreOpCode::RR_E]     = std::bind(&Cpu::RotateRight, thisPtr, GetSetE);
		preTable[PreOpCode::RR_H]     = std::bind(&Cpu::RotateRight, thisPtr, GetSetH);
		preTable[PreOpCode::RR_L]     = std::bind(&Cpu::RotateRight, thisPtr, GetSetL);
		preTable[PreOpCode::RR_HL_NI] = std::bind(&Cpu::RotateRight_Addr, thisPtr, RegisterHL);
		preTable[PreOpCode::RR_A]     = std::bind(&Cpu::RotateRight, thisPtr, GetSetA);

		// 0x2-
		preTable[PreOpCode::SLA_B]     = std::bind(&Cpu::ShiftLeftArithmetic, thisPtr, GetSetB);
		preTable[PreOpCode::SLA_C]     = std::bind(&Cpu::ShiftLeftArithmetic, thisPtr, GetSetC);
		preTable[PreOpCode::SLA_D]     = std::bind(&Cpu::ShiftLeftArithmetic, thisPtr, GetSetD);
		preTable[PreOpCode::SLA_E]     = std::bind(&Cpu::ShiftLeftArithmetic, thisPtr, GetSetE);
		preTable[PreOpCode::SLA_H]     = std::bind(&Cpu::ShiftLeftArithmetic, thisPtr, GetSetH);
		preTable[PreOpCode::SLA_L]     = std::bind(&Cpu::ShiftLeftArithmetic, thisPtr, GetSetL);
		preTable[PreOpCode::SLA_HL_NI] = std::bind(&Cpu::ShiftLeftArithmetic_Addr, thisPtr, RegisterHL);
		preTable[PreOpCode::SLA_A]     = std::bind(&Cpu::ShiftLeftArithmetic, thisPtr, GetSetA);
		preTable[PreOpCode::SRA_B]     = std::bind(&Cpu::ShiftRightArithmetic, thisPtr, GetSetB);
		preTable[PreOpCode::SRA_C]     = std::bind(&Cpu::ShiftRightArithmetic, thisPtr, GetSetC);
		preTable[PreOpCode::SRA_D]     = std::bind(&Cpu::ShiftRightArithmetic, thisPtr, GetSetD);
		preTable[PreOpCode::SRA_E]     = std::bind(&Cpu::ShiftRightArithmetic, thisPtr, GetSetE);
		preTable[PreOpCode::SRA_H]     = std::bind(&Cpu::ShiftRightArithmetic, thisPtr, GetSetH);
		preTable[PreOpCode::SRA_L]     = std::bind(&Cpu::ShiftRightArithmetic, thisPtr, GetSetL);
		preTable[PreOpCode::SRA_HL_NI] = std::bind(&Cpu::ShiftRightArithmetic_Addr, thisPtr, RegisterHL);
		preTable[PreOpCode::SRA_A]     = std::bind(&Cpu::ShiftRightArithmetic, thisPtr, GetSetA);

		// 0x3-
		preTable[PreOpCode::SWAP_B]     = std::bind(&Cpu::Swap, thisPtr, GetSetB);
		preTable[PreOpCode::SWAP_C]     = std::bind(&Cpu::Swap, thisPtr, GetSetC);
		preTable[PreOpCode::SWAP_D]     = std::bind(&Cpu::Swap, thisPtr, GetSetD);
		preTable[PreOpCode::SWAP_E]     = std::bind(&Cpu::Swap, thisPtr, GetSetE);
		preTable[PreOpCode::SWAP_H]     = std::bind(&Cpu::Swap, thisPtr, GetSetH);
		preTable[PreOpCode::SWAP_L]     = std::bind(&Cpu::Swap, thisPtr, GetSetL);
		preTable[PreOpCode::SWAP_HL_NI] = std::bind(&Cpu::Swap_Addr, thisPtr, RegisterHL);
		preTable[PreOpCode::SWAP_A]     = std::bind(&Cpu::Swap, thisPtr, GetSetA);
		preTable[PreOpCode::SRL_B]      = std::bind(&Cpu::ShiftRightLogical, thisPtr, GetSetB);
		preTable[PreOpCode::SRL_C]      = std::bind(&Cpu::ShiftRightLogical, thisPtr, GetSetC);
		preTable[PreOpCode::SRL_D]      = std::bind(&Cpu::ShiftRightLogical, thisPtr, GetSetD);
		preTable[PreOpCode::SRL_E]      = std::bind(&Cpu::ShiftRightLogical, thisPtr, GetSetE);
		preTable[PreOpCode::SRL_H]      = std::bind(&Cpu::ShiftRightLogical, thisPtr, GetSetH);
		preTable[PreOpCode::SRL_L]      = std::bind(&Cpu::ShiftRightLogical, thisPtr, GetSetL);
		preTable[PreOpCode::SRL_HL_NI]  = std::bind(&Cpu::ShiftRightLogical_Addr, thisPtr, RegisterHL);
		preTable[PreOpCode::SRL_A]      = std::bind(&Cpu::ShiftRightLogical, thisPtr, GetSetA);

		// 0x4-
		preTable[PreOpCode::BIT_0_B]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit0, GetB);
		preTable[PreOpCode::BIT_0_C]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit0, GetC);
		preTable[PreOpCode::BIT_0_D]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit0, GetD);
		preTable[PreOpCode::BIT_0_E]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit0, GetE);
		preTable[PreOpCode::BIT_0_H]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit0, GetH);
		preTable[PreOpCode::BIT_0_L]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit0, GetL);
		preTable[PreOpCode::BIT_0_HL_NI] = std::bind(&Cpu::BitComplementToZeroFlag_Addr, thisPtr, Bit0, RegisterHL);
		preTable[PreOpCode::BIT_0_A]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit0, GetA);
		preTable[PreOpCode::BIT_1_B]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit1, GetB);
		preTable[PreOpCode::BIT_1_C]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit1, GetC);
		preTable[PreOpCode::BIT_1_D]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit1, GetD);
		preTable[PreOpCode::BIT_1_E]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit1, GetE);
		preTable[PreOpCode::BIT_1_H]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit1, GetH);
		preTable[PreOpCode::BIT_1_L]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit1, GetL);
		preTable[PreOpCode::BIT_1_HL_NI] = std::bind(&Cpu::BitComplementToZeroFlag_Addr, thisPtr, Bit1, RegisterHL);
		preTable[PreOpCode::BIT_1_A]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit1, GetA);

		// 0x5-
		preTable[PreOpCode::BIT_2_B]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit2, GetB);
		preTable[PreOpCode::BIT_2_C]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit2, GetC);
		preTable[PreOpCode::BIT_2_D]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit2, GetD);
		preTable[PreOpCode::BIT_2_E]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit2, GetE);
		preTable[PreOpCode::BIT_2_H]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit2, GetH);
		preTable[PreOpCode::BIT_2_L]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit2, GetL);
		preTable[PreOpCode::BIT_2_HL_NI] = std::bind(&Cpu::BitComplementToZeroFlag_Addr, thisPtr, Bit2, RegisterHL);
		preTable[PreOpCode::BIT_2_A]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit2, GetA);
		preTable[PreOpCode::BIT_3_B]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit3, GetB);
		preTable[PreOpCode::BIT_3_C]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit3, GetC);
		preTable[PreOpCode::BIT_3_D]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit3, GetD);
		preTable[PreOpCode::BIT_3_E]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit3, GetE);
		preTable[PreOpCode::BIT_3_H]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit3, GetH);
		preTable[PreOpCode::BIT_3_L]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit3, GetL);
		preTable[PreOpCode::BIT_3_HL_NI] = std::bind(&Cpu::BitComplementToZeroFlag_Addr, thisPtr, Bit3, RegisterHL);
		preTable[PreOpCode::BIT_3_A]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit3, GetA);

		// 0x6-
		preTable[PreOpCode::BIT_4_B]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit4, GetB);
		preTable[PreOpCode::BIT_4_C]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit4, GetC);
		preTable[PreOpCode::BIT_4_D]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit4, GetD);
		preTable[PreOpCode::BIT_4_E]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit4, GetE);
		preTable[PreOpCode::BIT_4_H]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit4, GetH);
		preTable[PreOpCode::BIT_4_L]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit4, GetL);
		preTable[PreOpCode::BIT_4_HL_NI] = std::bind(&Cpu::BitComplementToZeroFlag_Addr, thisPtr, Bit4, RegisterHL);
		preTable[PreOpCode::BIT_4_A]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit4, GetA);
		preTable[PreOpCode::BIT_5_B]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit5, GetB);
		preTable[PreOpCode::BIT_5_C]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit5, GetC);
		preTable[PreOpCode::BIT_5_D]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit5, GetD);
		preTable[PreOpCode::BIT_5_E]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit5, GetE);
		preTable[PreOpCode::BIT_5_H]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit5, GetH);
		preTable[PreOpCode::BIT_5_L]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit5, GetL);
		preTable[PreOpCode::BIT_5_HL_NI] = std::bind(&Cpu::BitComplementToZeroFlag_Addr, thisPtr, Bit5, RegisterHL);
		preTable[PreOpCode::BIT_5_A]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit5, GetA);

		// 0x7-
		preTable[PreOpCode::BIT_6_B]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit6, GetB);
		preTable[PreOpCode::BIT_6_C]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit6, GetC);
		preTable[PreOpCode::BIT_6_D]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit6, GetD);
		preTable[PreOpCode::BIT_6_E]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit6, GetE);
		preTable[PreOpCode::BIT_6_H]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit6, GetH);
		preTable[PreOpCode::BIT_6_L]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit6, GetL);
		preTable[PreOpCode::BIT_6_HL_NI] = std::bind(&Cpu::BitComplementToZeroFlag_Addr, thisPtr, Bit6, RegisterHL);
		preTable[PreOpCode::BIT_6_A]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit6, GetA);
		preTable[PreOpCode::BIT_7_B]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit7, GetB);
		preTable[PreOpCode::BIT_7_C]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit7, GetC);
		preTable[PreOpCode::BIT_7_D]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit7, GetD);
		preTable[PreOpCode::BIT_7_E]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit7, GetE);
		preTable[PreOpCode::BIT_7_H]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit7, GetH);
		preTable[PreOpCode::BIT_7_L]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit7, GetL);
		preTable[PreOpCode::BIT_7_HL_NI] = std::bind(&Cpu::BitComplementToZeroFlag_Addr, thisPtr, Bit7, RegisterHL);
		preTable[PreOpCode::BIT_7_A]     = std::bind(&Cpu::BitComplementToZeroFlag, thisPtr, Bit7, GetA);

		// 0x8-
		preTable[PreOpCode::RES_0_B]     = std::bind(&Cpu::BitReset, thisPtr, Bit0, GetSetB);
		preTable[PreOpCode::RES_0_C]     = std::bind(&Cpu::BitReset, thisPtr, Bit0, GetSetC);
		preTable[PreOpCode::RES_0_D]     = std::bind(&Cpu::BitReset, thisPtr, Bit0, GetSetD);
		preTable[PreOpCode::RES_0_E]     = std::bind(&Cpu::BitReset, thisPtr, Bit0, GetSetE);
		preTable[PreOpCode::RES_0_H]     = std::bind(&Cpu::BitReset, thisPtr, Bit0, GetSetH);
		preTable[PreOpCode::RES_0_L]     = std::bind(&Cpu::BitReset, thisPtr, Bit0, GetSetL);
		preTable[PreOpCode::RES_0_HL_NI] = std::bind(&Cpu::BitReset_Addr, thisPtr, Bit0, RegisterHL);
		preTable[PreOpCode::RES_0_A]     = std::bind(&Cpu::BitReset, thisPtr, Bit0, GetSetA);
		preTable[PreOpCode::RES_1_B]     = std::bind(&Cpu::BitReset, thisPtr, Bit1, GetSetB);
		preTable[PreOpCode::RES_1_C]     = std::bind(&Cpu::BitReset, thisPtr, Bit1, GetSetC);
		preTable[PreOpCode::RES_1_D]     = std::bind(&Cpu::BitReset, thisPtr, Bit1, GetSetD);
		preTable[PreOpCode::RES_1_E]     = std::bind(&Cpu::BitReset, thisPtr, Bit1, GetSetE);
		preTable[PreOpCode::RES_1_H]     = std::bind(&Cpu::BitReset, thisPtr, Bit1, GetSetH);
		preTable[PreOpCode::RES_1_L]     = std::bind(&Cpu::BitReset, thisPtr, Bit1, GetSetL);
		preTable[PreOpCode::RES_1_HL_NI] = std::bind(&Cpu::BitReset_Addr, thisPtr, Bit1, RegisterHL);
		preTable[PreOpCode::RES_1_A]     = std::bind(&Cpu::BitReset, thisPtr, Bit1, GetSetA);

		// 0x9-
		preTable[PreOpCode::RES_2_B]     = std::bind(&Cpu::BitReset, thisPtr, Bit2, GetSetB);
		preTable[PreOpCode::RES_2_C]     = std::bind(&Cpu::BitReset, thisPtr, Bit2, GetSetC);
		preTable[PreOpCode::RES_2_D]     = std::bind(&Cpu::BitReset, thisPtr, Bit2, GetSetD);
		preTable[PreOpCode::RES_2_E]     = std::bind(&Cpu::BitReset, thisPtr, Bit2, GetSetE);
		preTable[PreOpCode::RES_2_H]     = std::bind(&Cpu::BitReset, thisPtr, Bit2, GetSetH);
		preTable[PreOpCode::RES_2_L]     = std::bind(&Cpu::BitReset, thisPtr, Bit2, GetSetL);
		preTable[PreOpCode::RES_2_HL_NI] = std::bind(&Cpu::BitReset_Addr, thisPtr, Bit2, RegisterHL);
		preTable[PreOpCode::RES_2_A]     = std::bind(&Cpu::BitReset, thisPtr, Bit2, GetSetA);
		preTable[PreOpCode::RES_3_B]     = std::bind(&Cpu::BitReset, thisPtr, Bit3, GetSetB);
		preTable[PreOpCode::RES_3_C]     = std::bind(&Cpu::BitReset, thisPtr, Bit3, GetSetC);
		preTable[PreOpCode::RES_3_D]     = std::bind(&Cpu::BitReset, thisPtr, Bit3, GetSetD);
		preTable[PreOpCode::RES_3_E]     = std::bind(&Cpu::BitReset, thisPtr, Bit3, GetSetE);
		preTable[PreOpCode::RES_3_H]     = std::bind(&Cpu::BitReset, thisPtr, Bit3, GetSetH);
		preTable[PreOpCode::RES_3_L]     = std::bind(&Cpu::BitReset, thisPtr, Bit3, GetSetL);
		preTable[PreOpCode::RES_3_HL_NI] = std::bind(&Cpu::BitReset_Addr, thisPtr, Bit3, RegisterHL);
		preTable[PreOpCode::RES_3_A]     = std::bind(&Cpu::BitReset, thisPtr, Bit3, GetSetA);

		// 0xA-
		preTable[PreOpCode::RES_4_B]     = std::bind(&Cpu::BitReset, thisPtr, Bit4, GetSetB);
		preTable[PreOpCode::RES_4_C]     = std::bind(&Cpu::BitReset, thisPtr, Bit4, GetSetC);
		preTable[PreOpCode::RES_4_D]     = std::bind(&Cpu::BitReset, thisPtr, Bit4, GetSetD);
		preTable[PreOpCode::RES_4_E]     = std::bind(&Cpu::BitReset, thisPtr, Bit4, GetSetE);
		preTable[PreOpCode::RES_4_H]     = std::bind(&Cpu::BitReset, thisPtr, Bit4, GetSetH);
		preTable[PreOpCode::RES_4_L]     = std::bind(&Cpu::BitReset, thisPtr, Bit4, GetSetL);
		preTable[PreOpCode::RES_4_HL_NI] = std::bind(&Cpu::BitReset_Addr, thisPtr, Bit4, RegisterHL);
		preTable[PreOpCode::RES_4_A]     = std::bind(&Cpu::BitReset, thisPtr, Bit4, GetSetA);
		preTable[PreOpCode::RES_5_B]     = std::bind(&Cpu::BitReset, thisPtr, Bit5, GetSetB);
		preTable[PreOpCode::RES_5_C]     = std::bind(&Cpu::BitReset, thisPtr, Bit5, GetSetC);
		preTable[PreOpCode::RES_5_D]     = std::bind(&Cpu::BitReset, thisPtr, Bit5, GetSetD);
		preTable[PreOpCode::RES_5_E]     = std::bind(&Cpu::BitReset, thisPtr, Bit5, GetSetE);
		preTable[PreOpCode::RES_5_H]     = std::bind(&Cpu::BitReset, thisPtr, Bit5, GetSetH);
		preTable[PreOpCode::RES_5_L]     = std::bind(&Cpu::BitReset, thisPtr, Bit5, GetSetL);
		preTable[PreOpCode::RES_5_HL_NI] = std::bind(&Cpu::BitReset_Addr, thisPtr, Bit5, RegisterHL);
		preTable[PreOpCode::RES_5_A]     = std::bind(&Cpu::BitReset, thisPtr, Bit5, GetSetA);

		// 0xB-
		// 0xC-
		// 0xD-
		// 0xE-
		// 0xF-	
	}
}
