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
	void Cpu::ExecuteInstruction(OpCode opCode)
	{
		switch (opCode)
		{
			case OpCode::NOP: 
			{
				// Only advances the program counter by 1.
		  		// Performs no other operations that would have an effect.
				cyclesPassed++;
				break;
			}
			case OpCode::LD_BC_n16: 
			{
				// Load the 2 bytes of immediate data into register pair BC.
				// 
				// The first byte of immediate data is the lower byte (i.e., bits 0-7),
				// and the second byte of immediate data is the higher byte (i.e., bits 8-15).
				// BC.SetLowByte();
				// BC.SetHighByte();
				cyclesPassed += 3;
				break;
			}
			case OpCode::LD_BC_NI_A: 
			{

				break;
			}
			case OpCode::INC_BC: 
			{

				break;
			}
			case OpCode::INC_B: 
			{

				break;
			}
			case OpCode::DEC_B: 
			{

				break;
			}
			case OpCode::LD_B_n8: 
			{

				break;
			}
			case OpCode::RLCA: 
			{

				break;
			}
			case OpCode::LD_a16_NI_SP: 
			{

				break;
			}
			case OpCode::ADD_HL_BC: 
			{

				break;
			}
			case OpCode::LD_A_BC_NI: 
			{

				break;
			}
			case OpCode::DEC_BC: 
			{

				break;
			}
			case OpCode::INC_C: 
			{

				break;
			}
			case OpCode::DEC_C: 
			{

				break;
			}
			case OpCode::LD_C_n8: 
			{

				break;
			}
			case OpCode::RRCA: 
			{

				break;
			}
			case OpCode::STOP_n8: 
			{

				break;
			}
			case OpCode::LD_DE_n16: 
			{

				break;
			}
			case OpCode::LD_DE_NI_A: 
			{

				break;
			}
			case OpCode::INC_DE: 
			{

				break;
			}
			case OpCode::INC_D: 
			{

				break;
			}
			case OpCode::DEC_D: 
			{

				break;
			}
			case OpCode::LD_D_n8: 
			{

				break;
			}
			case OpCode::RLA: 
			{

				break;
			}
			case OpCode::JR_e8: 
			{

				break;
			}
			case OpCode::ADD_HL_DE: 
			{

				break;
			}
			case OpCode::LD_A_DE_NI: 
			{

				break;
			}
			case OpCode::DEC_DE: 
			{

				break;
			}
			case OpCode::INC_E: 
			{

				break;
			}
			case OpCode::DEC_E: 
			{

				break;
			}
			case OpCode::LD_E_n8: 
			{

				break;
			}
			case OpCode::RRA: 
			{

				break;
			}
			case OpCode::JR_NZ_e8: 
			{

				break;
			}
			case OpCode::LD_HL_n16: 
			{

				break;
			}
			case OpCode::LD_HL_INC_NI_A: 
			{

				break;
			}
			case OpCode::INC_HL: 
			{

				break;
			}
			case OpCode::INC_H: 
			{

				break;
			}
			case OpCode::DEC_H: 
			{

				break;
			}
			case OpCode::LD_H_n8: 
			{

				break;
			}
			case OpCode::DAA: 
			{

				break;
			}
			case OpCode::JR_Z_e8: 
			{

				break;
			}
			case OpCode::ADD_HL_HL: 
			{

				break;
			}
			case OpCode::LD_A_HL_INC_NI: 
			{

				break;
			}
			case OpCode::DEC_HL: 
			{

				break;
			}
			case OpCode::INC_L: 
			{

				break;
			}
			case OpCode::DEC_L: 
			{

				break;
			}
			case OpCode::LD_L_n8: 
			{

				break;
			}
			case OpCode::CPL: 
			{

				break;
			}
			case OpCode::JR_NC_e8: 
			{

				break;
			}
			case OpCode::LD_SP_n16: 
			{

				break;
			}
			case OpCode::LD_HL_DEC_NI_A: 
			{

				break;
			}
			case OpCode::INC_SP: 
			{

				break;
			}
			case OpCode::INC_HL_NI: 
			{

				break;
			}
			case OpCode::DEC_HL_NI: 
			{

				break;
			}
			case OpCode::LD_HL_NI_n8: 
			{

				break;
			}
			case OpCode::SCF: 
			{

				break;
			}
			case OpCode::JR_C_e8: 
			{

				break;
			}
			case OpCode::ADD_HL_SP: 
			{

				break;
			}
			case OpCode::LD_A_HL_DEC_NI: 
			{

				break;
			}
			case OpCode::DEC_SP: 
			{

				break;
			}
			case OpCode::INC_A: 
			{

				break;
			}
			case OpCode::DEC_A: 
			{

				break;
			}
			case OpCode::LD_A_n8: 
			{

				break;
			}
			case OpCode::CCF: 
			{

				break;
			}
			case OpCode::LD_B_B: 
			{

				break;
			}
			case OpCode::LD_B_C: 
			{

				break;
			}
			case OpCode::LD_B_D: 
			{

				break;
			}
			case OpCode::LD_B_E: 
			{

				break;
			}
			case OpCode::LD_B_H: 
			{

				break;
			}
			case OpCode::LD_B_L: 
			{

				break;
			}
			case OpCode::LD_B_HL_NI: 
			{

				break;
			}
			case OpCode::LD_B_A: 
			{

				break;
			}
			case OpCode::LD_C_B: 
			{

				break;
			}
			case OpCode::LD_C_C: 
			{

				break;
			}
			case OpCode::LD_C_D: 
			{

				break;
			}
			case OpCode::LD_C_E: 
			{

				break;
			}
			case OpCode::LD_C_H: 
			{

				break;
			}
			case OpCode::LD_C_L: 
			{

				break;
			}
			case OpCode::LD_C_HL_NI: 
			{

				break;
			}
			case OpCode::LD_C_A: 
			{

				break;
			}
			case OpCode::LD_D_B: 
			{

				break;
			}
			case OpCode::LD_D_C: 
			{

				break;
			}
			case OpCode::LD_D_D: 
			{

				break;
			}
			case OpCode::LD_D_E: 
			{

				break;
			}
			case OpCode::LD_D_H: 
			{

				break;
			}
			case OpCode::LD_D_L: 
			{

				break;
			}
			case OpCode::LD_D_HL_NI: 
			{

				break;
			}
			case OpCode::LD_D_A: 
			{

				break;
			}
			case OpCode::LD_E_B: 
			{

				break;
			}
			case OpCode::LD_E_C: 
			{

				break;
			}
			case OpCode::LD_E_D: 
			{

				break;
			}
			case OpCode::LD_E_E: 
			{

				break;
			}
			case OpCode::LD_E_H: 
			{

				break;
			}
			case OpCode::LD_E_L: 
			{

				break;
			}
			case OpCode::LD_E_HL_NI: 
			{

				break;
			}
			case OpCode::LD_E_A: 
			{

				break;
			}
			case OpCode::LD_H_B: 
			{

				break;
			}
			case OpCode::LD_H_C: 
			{

				break;
			}
			case OpCode::LD_H_D: 
			{

				break;
			}
			case OpCode::LD_H_E: 
			{

				break;
			}
			case OpCode::LD_H_H: 
			{

				break;
			}
			case OpCode::LD_H_L: 
			{

				break;
			}
			case OpCode::LD_H_HL_NI: 
			{

				break;
			}
			case OpCode::LD_H_A: 
			{

				break;
			}
			case OpCode::LD_L_B: 
			{

				break;
			}
			case OpCode::LD_L_C: 
			{

				break;
			}
			case OpCode::LD_L_D: 
			{

				break;
			}
			case OpCode::LD_L_E: 
			{

				break;
			}
			case OpCode::LD_L_H: 
			{

				break;
			}
			case OpCode::LD_L_L: 
			{

				break;
			}
			case OpCode::LD_L_HL_NI: 
			{

				break;
			}
			case OpCode::LD_L_A: 
			{

				break;
			}
			case OpCode::LD_HL_NI_B: 
			{

				break;
			}
			case OpCode::LD_HL_NI_C: 
			{

				break;
			}
			case OpCode::LD_HL_NI_D: 
			{

				break;
			}
			case OpCode::LD_HL_NI_E: 
			{

				break;
			}
			case OpCode::LD_HL_NI_H: 
			{

				break;
			}
			case OpCode::LD_HL_NI_L: 
			{

				break;
			}
			case OpCode::HALT: 
			{

				break;
			}
			case OpCode::LD_HL_NI_A: 
			{

				break;
			}
			case OpCode::LD_A_B: 
			{

				break;
			}
			case OpCode::LD_A_C: 
			{

				break;
			}
			case OpCode::LD_A_D: 
			{

				break;
			}
			case OpCode::LD_A_E: 
			{

				break;
			}
			case OpCode::LD_A_H: 
			{

				break;
			}
			case OpCode::LD_A_L: 
			{

				break;
			}
			case OpCode::LD_A_HL_NI: 
			{

				break;
			}
			case OpCode::LD_A_A: 
			{

				break;
			}
			case OpCode::ADD_A_B: 
			{

				break;
			}
			case OpCode::ADD_A_C: 
			{

				break;
			}
			case OpCode::ADD_A_D: 
			{

				break;
			}
			case OpCode::ADD_A_E: 
			{

				break;
			}
			case OpCode::ADD_A_H: 
			{

				break;
			}
			case OpCode::ADD_A_L: 
			{

				break;
			}
			case OpCode::ADD_A_HL_NI: 
			{

				break;
			}
			case OpCode::ADD_A_A: 
			{

				break;
			}
			case OpCode::ADC_A_B: 
			{

				break;
			}
			case OpCode::ADC_A_C: 
			{

				break;
			}
			case OpCode::ADC_A_D: 
			{

				break;
			}
			case OpCode::ADC_A_E: 
			{

				break;
			}
			case OpCode::ADC_A_H: 
			{

				break;
			}
			case OpCode::ADC_A_L: 
			{

				break;
			}
			case OpCode::ADC_A_HL_NI: 
			{

				break;
			}
			case OpCode::ADC_A_A: 
			{

				break;
			}
			case OpCode::SUB_A_B: 
			{

				break;
			}
			case OpCode::SUB_A_C: 
			{

				break;
			}
			case OpCode::SUB_A_D: 
			{

				break;
			}
			case OpCode::SUB_A_E: 
			{

				break;
			}
			case OpCode::SUB_A_H: 
			{

				break;
			}
			case OpCode::SUB_A_L: 
			{

				break;
			}
			case OpCode::SUB_A_HL_NI: 
			{

				break;
			}
			case OpCode::SUB_A_A: 
			{

				break;
			}
			case OpCode::SBC_A_B: 
			{

				break;
			}
			case OpCode::SBC_A_C: 
			{

				break;
			}
			case OpCode::SBC_A_D: 
			{

				break;
			}
			case OpCode::SBC_A_E: 
			{

				break;
			}
			case OpCode::SBC_A_H: 
			{

				break;
			}
			case OpCode::SBC_A_L: 
			{

				break;
			}
			case OpCode::SBC_A_HL_NI: 
			{

				break;
			}
			case OpCode::SBC_A_A: 
			{

				break;
			}
			case OpCode::AND_A_B: 
			{

				break;
			}
			case OpCode::AND_A_C: 
			{

				break;
			}
			case OpCode::AND_A_D: 
			{

				break;
			}
			case OpCode::AND_A_E: 
			{

				break;
			}
			case OpCode::AND_A_H: 
			{

				break;
			}
			case OpCode::AND_A_L: 
			{

				break;
			}
			case OpCode::AND_A_HL_NI: 
			{

				break;
			}
			case OpCode::AND_A_A: 
			{

				break;
			}
			case OpCode::XOR_A_B: 
			{

				break;
			}
			case OpCode::XOR_A_C: 
			{

				break;
			}
			case OpCode::XOR_A_D: 
			{

				break;
			}
			case OpCode::XOR_A_E: 
			{

				break;
			}
			case OpCode::XOR_A_H: 
			{

				break;
			}
			case OpCode::XOR_A_L: 
			{

				break;
			}
			case OpCode::XOR_A_HL_NI: 
			{

				break;
			}
			case OpCode::XOR_A_A: 
			{

				break;
			}
			case OpCode::OR_A_B: 
			{

				break;
			}
			case OpCode::OR_A_C: 
			{

				break;
			}
			case OpCode::OR_A_D: 
			{

				break;
			}
			case OpCode::OR_A_E: 
			{

				break;
			}
			case OpCode::OR_A_H: 
			{

				break;
			}
			case OpCode::OR_A_L: 
			{

				break;
			}
			case OpCode::OR_A_HL_NI: 
			{

				break;
			}
			case OpCode::OR_A_A: 
			{

				break;
			}
			case OpCode::CP_A_B: 
			{

				break;
			}
			case OpCode::CP_A_C: 
			{

				break;
			}
			case OpCode::CP_A_D: 
			{

				break;
			}
			case OpCode::CP_A_E: 
			{

				break;
			}
			case OpCode::CP_A_H: 
			{

				break;
			}
			case OpCode::CP_A_L: 
			{

				break;
			}
			case OpCode::CP_A_HL_NI: 
			{

				break;
			}
			case OpCode::CP_A_A: 
			{

				break;
			}
			case OpCode::RET_NZ: 
			{

				break;
			}
			case OpCode::POP_BC: 
			{

				break;
			}
			case OpCode::JP_NZ_a16: 
			{

				break;
			}
			case OpCode::JP_a16: 
			{

				break;
			}
			case OpCode::CALL_NZ_a16: 
			{

				break;
			}
			case OpCode::PUSH_BC: 
			{

				break;
			}
			case OpCode::ADD_A_n8: 
			{

				break;
			}
			case OpCode::RST_00: 
			{

				break;
			}
			case OpCode::RET_Z: 
			{

				break;
			}
			case OpCode::RET: 
			{

				break;
			}
			case OpCode::JP_Z_a16: 
			{

				break;
			}
			case OpCode::PREFIX: 
			{

				break;
			}
			case OpCode::CALL_Z_a16: 
			{

				break;
			}
			case OpCode::CALL_a16: 
			{

				break;
			}
			case OpCode::ADC_A_n8: 
			{

				break;
			}
			case OpCode::RST_08: 
			{

				break;
			}
			case OpCode::RET_NC: 
			{

				break;
			}
			case OpCode::POP_DE: 
			{

				break;
			}
			case OpCode::JP_NC_a16: 
			{

				break;
			}
			case OpCode::ILLEGAL_D3: 
			{

				break;
			}
			case OpCode::CALL_NC_a16: 
			{

				break;
			}
			case OpCode::PUSH_DE: 
			{

				break;
			}
			case OpCode::SUB_A_n8: 
			{

				break;
			}
			case OpCode::RST_10: 
			{

				break;
			}
			case OpCode::RET_C: 
			{

				break;
			}
			case OpCode::RETI: 
			{

				break;
			}
			case OpCode::JP_C_a16: 
			{

				break;
			}
			case OpCode::ILLEGAL_DB: 
			{

				break;
			}
			case OpCode::CALL_C_a16: 
			{

				break;
			}
			case OpCode::ILLEGAL_DD: 
			{

				break;
			}
			case OpCode::SBC_A_n8: 
			{

				break;
			}
			case OpCode::RST_18: 
			{

				break;
			}
			case OpCode::LDH_a8_NI_A: 
			{

				break;
			}
			case OpCode::POP_HL: 
			{

				break;
			}
			case OpCode::LDH_C_NI_A: 
			{

				break;
			}
			case OpCode::ILLEGAL_E3: 
			{

				break;
			}
			case OpCode::ILLEGAL_E4: 
			{

				break;
			}
			case OpCode::PUSH_HL: 
			{

				break;
			}
			case OpCode::AND_A_n8: 
			{

				break;
			}
			case OpCode::RST_20: 
			{

				break;
			}
			case OpCode::ADD_SP_e8: 
			{

				break;
			}
			case OpCode::JP_HL: 
			{

				break;
			}
			case OpCode::LD_a16_NI_A: 
			{

				break;
			}
			case OpCode::ILLEGAL_EB: 
			{

				break;
			}
			case OpCode::ILLEGAL_EC: 
			{

				break;
			}
			case OpCode::ILLEGAL_ED: 
			{

				break;
			}
			case OpCode::XOR_A_n8: 
			{

				break;
			}
			case OpCode::RST_28: 
			{

				break;
			}
			case OpCode::LDH_A_a8_NI: 
			{

				break;
			}
			case OpCode::POP_AF: 
			{

				break;
			}
			case OpCode::LDH_A_C_NI: 
			{

				break;
			}
			case OpCode::DI: 
			{

				break;
			}
			case OpCode::ILLEGAL_F4: 
			{

				break;
			}
			case OpCode::PUSH_AF: 
			{

				break;
			}
			case OpCode::OR_A_n8: 
			{

				break;
			}
			case OpCode::RST_30: 
			{

				break;
			}
			case OpCode::LD_HL_SP_INC_e8: 
			{

				break;
			}
			case OpCode::LD_SP_HL: 
			{

				break;
			}
			case OpCode::LD_A_a16_NI: 
			{

				break;
			}
			case OpCode::EI: 
			{

				break;
			}
			case OpCode::ILLEGAL_FC: 
			{

				break;
			}
			case OpCode::ILLEGAL_FD: 
			{

				break;
			}
			case OpCode::CP_A_n8: 
			{

				break;
			}
			case OpCode::RST_38: 
			{

				break;
			}
			default:
			{
				// Something went horribly wrong, we probalby want to make this a setissue when its complete though.
				abort();
			}
		}
	}

	void Cpu::ExecuteInstruction(PrefixedOpCode opCode)
	{
		switch (opCode)
		{
			case PrefixedOpCode::RLC_B: 
			{

				break;
			}
			case PrefixedOpCode::RLC_C: 
			{

				break;
			}
			case PrefixedOpCode::RLC_D: 
			{

				break;
			}
			case PrefixedOpCode::RLC_E: 
			{

				break;
			}
			case PrefixedOpCode::RLC_H: 
			{

				break;
			}
			case PrefixedOpCode::RLC_L: 
			{

				break;
			}
			case PrefixedOpCode::RLC_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RLC_A: 
			{

				break;
			}
			case PrefixedOpCode::RRC_B: 
			{

				break;
			}
			case PrefixedOpCode::RRC_C: 
			{

				break;
			}
			case PrefixedOpCode::RRC_D: 
			{

				break;
			}
			case PrefixedOpCode::RRC_E: 
			{

				break;
			}
			case PrefixedOpCode::RRC_H: 
			{

				break;
			}
			case PrefixedOpCode::RRC_L: 
			{

				break;
			}
			case PrefixedOpCode::RRC_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RRC_A: 
			{

				break;
			}
			case PrefixedOpCode::RL_B: 
			{

				break;
			}
			case PrefixedOpCode::RL_C: 
			{

				break;
			}
			case PrefixedOpCode::RL_D: 
			{

				break;
			}
			case PrefixedOpCode::RL_E: 
			{

				break;
			}
			case PrefixedOpCode::RL_H: 
			{

				break;
			}
			case PrefixedOpCode::RL_L: 
			{

				break;
			}
			case PrefixedOpCode::RL_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RL_A: 
			{

				break;
			}
			case PrefixedOpCode::RR_B: 
			{

				break;
			}
			case PrefixedOpCode::RR_C: 
			{

				break;
			}
			case PrefixedOpCode::RR_D: 
			{

				break;
			}
			case PrefixedOpCode::RR_E: 
			{

				break;
			}
			case PrefixedOpCode::RR_H: 
			{

				break;
			}
			case PrefixedOpCode::RR_L: 
			{

				break;
			}
			case PrefixedOpCode::RR_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RR_A: 
			{

				break;
			}
			case PrefixedOpCode::SLA_B: 
			{

				break;
			}
			case PrefixedOpCode::SLA_C: 
			{

				break;
			}
			case PrefixedOpCode::SLA_D: 
			{

				break;
			}
			case PrefixedOpCode::SLA_E: 
			{

				break;
			}
			case PrefixedOpCode::SLA_H: 
			{

				break;
			}
			case PrefixedOpCode::SLA_L: 
			{

				break;
			}
			case PrefixedOpCode::SLA_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SLA_A: 
			{

				break;
			}
			case PrefixedOpCode::SRA_B: 
			{

				break;
			}
			case PrefixedOpCode::SRA_C: 
			{

				break;
			}
			case PrefixedOpCode::SRA_D: 
			{

				break;
			}
			case PrefixedOpCode::SRA_E: 
			{

				break;
			}
			case PrefixedOpCode::SRA_H: 
			{

				break;
			}
			case PrefixedOpCode::SRA_L: 
			{

				break;
			}
			case PrefixedOpCode::SRA_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SRA_A: 
			{

				break;
			}
			case PrefixedOpCode::SWAP_B: 
			{

				break;
			}
			case PrefixedOpCode::SWAP_C: 
			{

				break;
			}
			case PrefixedOpCode::SWAP_D: 
			{

				break;
			}
			case PrefixedOpCode::SWAP_E: 
			{

				break;
			}
			case PrefixedOpCode::SWAP_H: 
			{

				break;
			}
			case PrefixedOpCode::SWAP_L: 
			{

				break;
			}
			case PrefixedOpCode::SWAP_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SWAP_A: 
			{

				break;
			}
			case PrefixedOpCode::SRL_B: 
			{

				break;
			}
			case PrefixedOpCode::SRL_C: 
			{

				break;
			}
			case PrefixedOpCode::SRL_D: 
			{

				break;
			}
			case PrefixedOpCode::SRL_E: 
			{

				break;
			}
			case PrefixedOpCode::SRL_H: 
			{

				break;
			}
			case PrefixedOpCode::SRL_L: 
			{

				break;
			}
			case PrefixedOpCode::SRL_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SRL_A: 
			{

				break;
			}
			case PrefixedOpCode::BIT_0_B: 
			{

				break;
			}
			case PrefixedOpCode::BIT_0_C: 
			{

				break;
			}
			case PrefixedOpCode::BIT_0_D: 
			{

				break;
			}
			case PrefixedOpCode::BIT_0_E: 
			{

				break;
			}
			case PrefixedOpCode::BIT_0_H: 
			{

				break;
			}
			case PrefixedOpCode::BIT_0_L: 
			{

				break;
			}
			case PrefixedOpCode::BIT_0_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::BIT_0_A: 
			{

				break;
			}
			case PrefixedOpCode::BIT_1_B: 
			{

				break;
			}
			case PrefixedOpCode::BIT_1_C: 
			{

				break;
			}
			case PrefixedOpCode::BIT_1_D: 
			{

				break;
			}
			case PrefixedOpCode::BIT_1_E: 
			{

				break;
			}
			case PrefixedOpCode::BIT_1_H: 
			{

				break;
			}
			case PrefixedOpCode::BIT_1_L: 
			{

				break;
			}
			case PrefixedOpCode::BIT_1_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::BIT_1_A: 
			{

				break;
			}
			case PrefixedOpCode::BIT_2_B: 
			{

				break;
			}
			case PrefixedOpCode::BIT_2_C: 
			{

				break;
			}
			case PrefixedOpCode::BIT_2_D: 
			{

				break;
			}
			case PrefixedOpCode::BIT_2_E: 
			{

				break;
			}
			case PrefixedOpCode::BIT_2_H: 
			{

				break;
			}
			case PrefixedOpCode::BIT_2_L: 
			{

				break;
			}
			case PrefixedOpCode::BIT_2_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::BIT_2_A: 
			{

				break;
			}
			case PrefixedOpCode::BIT_3_B: 
			{

				break;
			}
			case PrefixedOpCode::BIT_3_C: 
			{

				break;
			}
			case PrefixedOpCode::BIT_3_D: 
			{

				break;
			}
			case PrefixedOpCode::BIT_3_E: 
			{

				break;
			}
			case PrefixedOpCode::BIT_3_H: 
			{

				break;
			}
			case PrefixedOpCode::BIT_3_L: 
			{

				break;
			}
			case PrefixedOpCode::BIT_3_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::BIT_3_A: 
			{

				break;
			}
			case PrefixedOpCode::BIT_4_B: 
			{

				break;
			}
			case PrefixedOpCode::BIT_4_C: 
			{

				break;
			}
			case PrefixedOpCode::BIT_4_D: 
			{

				break;
			}
			case PrefixedOpCode::BIT_4_E: 
			{

				break;
			}
			case PrefixedOpCode::BIT_4_H: 
			{

				break;
			}
			case PrefixedOpCode::BIT_4_L: 
			{

				break;
			}
			case PrefixedOpCode::BIT_4_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::BIT_4_A: 
			{

				break;
			}
			case PrefixedOpCode::BIT_5_B: 
			{

				break;
			}
			case PrefixedOpCode::BIT_5_C: 
			{

				break;
			}
			case PrefixedOpCode::BIT_5_D: 
			{

				break;
			}
			case PrefixedOpCode::BIT_5_E: 
			{

				break;
			}
			case PrefixedOpCode::BIT_5_H: 
			{

				break;
			}
			case PrefixedOpCode::BIT_5_L: 
			{

				break;
			}
			case PrefixedOpCode::BIT_5_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::BIT_5_A: 
			{

				break;
			}
			case PrefixedOpCode::BIT_6_B: 
			{

				break;
			}
			case PrefixedOpCode::BIT_6_C: 
			{

				break;
			}
			case PrefixedOpCode::BIT_6_D: 
			{

				break;
			}
			case PrefixedOpCode::BIT_6_E: 
			{

				break;
			}
			case PrefixedOpCode::BIT_6_H: 
			{

				break;
			}
			case PrefixedOpCode::BIT_6_L: 
			{

				break;
			}
			case PrefixedOpCode::BIT_6_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::BIT_6_A: 
			{

				break;
			}
			case PrefixedOpCode::BIT_7_B: 
			{

				break;
			}
			case PrefixedOpCode::BIT_7_C: 
			{

				break;
			}
			case PrefixedOpCode::BIT_7_D: 
			{

				break;
			}
			case PrefixedOpCode::BIT_7_E: 
			{

				break;
			}
			case PrefixedOpCode::BIT_7_H: 
			{

				break;
			}
			case PrefixedOpCode::BIT_7_L: 
			{

				break;
			}
			case PrefixedOpCode::BIT_7_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::BIT_7_A: 
			{

				break;
			}
			case PrefixedOpCode::RES_0_B: 
			{

				break;
			}
			case PrefixedOpCode::RES_0_C: 
			{

				break;
			}
			case PrefixedOpCode::RES_0_D: 
			{

				break;
			}
			case PrefixedOpCode::RES_0_E: 
			{

				break;
			}
			case PrefixedOpCode::RES_0_H: 
			{

				break;
			}
			case PrefixedOpCode::RES_0_L: 
			{

				break;
			}
			case PrefixedOpCode::RES_0_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RES_0_A: 
			{

				break;
			}
			case PrefixedOpCode::RES_1_B: 
			{

				break;
			}
			case PrefixedOpCode::RES_1_C: 
			{

				break;
			}
			case PrefixedOpCode::RES_1_D: 
			{

				break;
			}
			case PrefixedOpCode::RES_1_E: 
			{

				break;
			}
			case PrefixedOpCode::RES_1_H: 
			{

				break;
			}
			case PrefixedOpCode::RES_1_L: 
			{

				break;
			}
			case PrefixedOpCode::RES_1_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RES_1_A: 
			{

				break;
			}
			case PrefixedOpCode::RES_2_B: 
			{

				break;
			}
			case PrefixedOpCode::RES_2_C: 
			{

				break;
			}
			case PrefixedOpCode::RES_2_D: 
			{

				break;
			}
			case PrefixedOpCode::RES_2_E: 
			{

				break;
			}
			case PrefixedOpCode::RES_2_H: 
			{

				break;
			}
			case PrefixedOpCode::RES_2_L: 
			{

				break;
			}
			case PrefixedOpCode::RES_2_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RES_2_A: 
			{

				break;
			}
			case PrefixedOpCode::RES_3_B: 
			{

				break;
			}
			case PrefixedOpCode::RES_3_C: 
			{

				break;
			}
			case PrefixedOpCode::RES_3_D: 
			{

				break;
			}
			case PrefixedOpCode::RES_3_E: 
			{

				break;
			}
			case PrefixedOpCode::RES_3_H: 
			{

				break;
			}
			case PrefixedOpCode::RES_3_L: 
			{

				break;
			}
			case PrefixedOpCode::RES_3_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RES_3_A: 
			{

				break;
			}
			case PrefixedOpCode::RES_4_B: 
			{

				break;
			}
			case PrefixedOpCode::RES_4_C: 
			{

				break;
			}
			case PrefixedOpCode::RES_4_D: 
			{

				break;
			}
			case PrefixedOpCode::RES_4_E: 
			{

				break;
			}
			case PrefixedOpCode::RES_4_H: 
			{

				break;
			}
			case PrefixedOpCode::RES_4_L: 
			{

				break;
			}
			case PrefixedOpCode::RES_4_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RES_4_A: 
			{

				break;
			}
			case PrefixedOpCode::RES_5_B: 
			{

				break;
			}
			case PrefixedOpCode::RES_5_C: 
			{

				break;
			}
			case PrefixedOpCode::RES_5_D: 
			{

				break;
			}
			case PrefixedOpCode::RES_5_E: 
			{

				break;
			}
			case PrefixedOpCode::RES_5_H: 
			{

				break;
			}
			case PrefixedOpCode::RES_5_L: 
			{

				break;
			}
			case PrefixedOpCode::RES_5_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RES_5_A: 
			{

				break;
			}
			case PrefixedOpCode::RES_6_B: 
			{

				break;
			}
			case PrefixedOpCode::RES_6_C: 
			{

				break;
			}
			case PrefixedOpCode::RES_6_D: 
			{

				break;
			}
			case PrefixedOpCode::RES_6_E: 
			{

				break;
			}
			case PrefixedOpCode::RES_6_H: 
			{

				break;
			}
			case PrefixedOpCode::RES_6_L: 
			{

				break;
			}
			case PrefixedOpCode::RES_6_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RES_6_A: 
			{

				break;
			}
			case PrefixedOpCode::RES_7_B: 
			{

				break;
			}
			case PrefixedOpCode::RES_7_C: 
			{

				break;
			}
			case PrefixedOpCode::RES_7_D: 
			{

				break;
			}
			case PrefixedOpCode::RES_7_E: 
			{

				break;
			}
			case PrefixedOpCode::RES_7_H: 
			{

				break;
			}
			case PrefixedOpCode::RES_7_L: 
			{

				break;
			}
			case PrefixedOpCode::RES_7_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::RES_7_A: 
			{

				break;
			}
			case PrefixedOpCode::SET_0_B: 
			{

				break;
			}
			case PrefixedOpCode::SET_0_C: 
			{

				break;
			}
			case PrefixedOpCode::SET_0_D: 
			{

				break;
			}
			case PrefixedOpCode::SET_0_E: 
			{

				break;
			}
			case PrefixedOpCode::SET_0_H: 
			{

				break;
			}
			case PrefixedOpCode::SET_0_L: 
			{

				break;
			}
			case PrefixedOpCode::SET_0_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SET_0_A: 
			{

				break;
			}
			case PrefixedOpCode::SET_1_B: 
			{

				break;
			}
			case PrefixedOpCode::SET_1_C: 
			{

				break;
			}
			case PrefixedOpCode::SET_1_D: 
			{

				break;
			}
			case PrefixedOpCode::SET_1_E: 
			{

				break;
			}
			case PrefixedOpCode::SET_1_H: 
			{

				break;
			}
			case PrefixedOpCode::SET_1_L: 
			{

				break;
			}
			case PrefixedOpCode::SET_1_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SET_1_A: 
			{

				break;
			}
			case PrefixedOpCode::SET_2_B: 
			{

				break;
			}
			case PrefixedOpCode::SET_2_C: 
			{

				break;
			}
			case PrefixedOpCode::SET_2_D: 
			{

				break;
			}
			case PrefixedOpCode::SET_2_E: 
			{

				break;
			}
			case PrefixedOpCode::SET_2_H: 
			{

				break;
			}
			case PrefixedOpCode::SET_2_L: 
			{

				break;
			}
			case PrefixedOpCode::SET_2_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SET_2_A: 
			{

				break;
			}
			case PrefixedOpCode::SET_3_B: 
			{

				break;
			}
			case PrefixedOpCode::SET_3_C: 
			{

				break;
			}
			case PrefixedOpCode::SET_3_D: 
			{

				break;
			}
			case PrefixedOpCode::SET_3_E: 
			{

				break;
			}
			case PrefixedOpCode::SET_3_H: 
			{

				break;
			}
			case PrefixedOpCode::SET_3_L: 
			{

				break;
			}
			case PrefixedOpCode::SET_3_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SET_3_A: 
			{

				break;
			}
			case PrefixedOpCode::SET_4_B: 
			{

				break;
			}
			case PrefixedOpCode::SET_4_C: 
			{

				break;
			}
			case PrefixedOpCode::SET_4_D: 
			{

				break;
			}
			case PrefixedOpCode::SET_4_E: 
			{

				break;
			}
			case PrefixedOpCode::SET_4_H: 
			{

				break;
			}
			case PrefixedOpCode::SET_4_L: 
			{

				break;
			}
			case PrefixedOpCode::SET_4_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SET_4_A: 
			{

				break;
			}
			case PrefixedOpCode::SET_5_B: 
			{

				break;
			}
			case PrefixedOpCode::SET_5_C: 
			{

				break;
			}
			case PrefixedOpCode::SET_5_D: 
			{

				break;
			}
			case PrefixedOpCode::SET_5_E: 
			{

				break;
			}
			case PrefixedOpCode::SET_5_H: 
			{

				break;
			}
			case PrefixedOpCode::SET_5_L: 
			{

				break;
			}
			case PrefixedOpCode::SET_5_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SET_5_A: 
			{

				break;
			}
			case PrefixedOpCode::SET_6_B: 
			{

				break;
			}
			case PrefixedOpCode::SET_6_C: 
			{

				break;
			}
			case PrefixedOpCode::SET_6_D: 
			{

				break;
			}
			case PrefixedOpCode::SET_6_E: 
			{

				break;
			}
			case PrefixedOpCode::SET_6_H: 
			{

				break;
			}
			case PrefixedOpCode::SET_6_L: 
			{

				break;
			}
			case PrefixedOpCode::SET_6_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SET_6_A: 
			{

				break;
			}
			case PrefixedOpCode::SET_7_B: 
			{

				break;
			}
			case PrefixedOpCode::SET_7_C: 
			{

				break;
			}
			case PrefixedOpCode::SET_7_D: 
			{

				break;
			}
			case PrefixedOpCode::SET_7_E: 
			{

				break;
			}
			case PrefixedOpCode::SET_7_H: 
			{

				break;
			}
			case PrefixedOpCode::SET_7_L: 
			{

				break;
			}
			case PrefixedOpCode::SET_7_HL_NI: 
			{

				break;
			}
			case PrefixedOpCode::SET_7_A: 
			{

				break;
			}
			default:
			{
				// Something went horribly wrong, we probalby want to make this a setissue when its complete though.
				abort();
			}
		}
	}

	void Cpu::ExecuteInstruction()
	{
		if (opcodeIsPrefixed)
		{
			ExecuteInstruction(static_cast<PrefixedOpCode>(currentOpcode));
		}
		else
		{
			ExecuteInstruction(static_cast<OpCode>(currentOpcode));
		}
		// TODO: handle instruction timing
	}

	void Cpu::FetchInstruction()
	{
		currentOpcode = memoryBus.ReadByte(PC++);
		if (currentOpcode == opcodePrefix)
		{
			opcodeIsPrefixed = true;
			currentOpcode = memoryBus.ReadByte(PC++);
		}
		else
		{
			opcodeIsPrefixed = false;
		}
	}

	uint8_t	Cpu::GetInterruptRegister()
	{
		return (IE);
	}

} /* namespace Gameboy */
