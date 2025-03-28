
#pragma once

#include <play-man/utility/EnumMacro.hpp>

namespace Gameboy
{


#define GAMEBOY_OPCODES_SEQ(X, n) \
X(n, NOP, 0x00)				\
X(n, LD_BC_n16, 0x01)		\
X(n, LD_BC_NI_A, 0x02)		\
X(n, INC_BC, 0x03)			\
X(n, INC_B, 0x04)			\
X(n, DEC_B, 0x05)			\
X(n, LD_B_n8, 0x06)			\
X(n, RLCA, 0x07)			\
X(n, LD_a16_NI_SP, 0x08)	\
X(n, ADD_HL_BC, 0x09)		\
X(n, LD_A_BC_NI, 0x0A)		\
X(n, DEC_BC, 0x0B)			\
X(n, INC_C, 0x0C)			\
X(n, DEC_C, 0x0D)			\
X(n, LD_C_n8, 0x0E)			\
X(n, RRCA, 0x0F)			\
X(n, STOP_n8, 0x10)			\
X(n, LD_DE_n16, 0x11)		\
X(n, LD_DE_NI_A, 0x12)		\
X(n, INC_DE, 0x13)			\
X(n, INC_D, 0x14)			\
X(n, DEC_D, 0x15)			\
X(n, LD_D_n8, 0x16)			\
X(n, RLA, 0x17)				\
X(n, JR_e8, 0x18)			\
X(n, ADD_HL_DE, 0x19)		\
X(n, LD_A_DE_NI, 0x1A)		\
X(n, DEC_DE, 0x1B)			\
X(n, INC_E, 0x1C)			\
X(n, DEC_E, 0x1D)			\
X(n, LD_E_n8, 0x1E)			\
X(n, RRA, 0x1F)				\
X(n, JR_NZ_e8, 0x20)		\
X(n, LD_HL_n16, 0x21)		\
X(n, LD_HL_INC_NI_A, 0x22)	\
X(n, INC_HL, 0x23)			\
X(n, INC_H, 0x24)			\
X(n, DEC_H, 0x25)			\
X(n, LD_H_n8, 0x26)			\
X(n, DAA, 0x27)				\
X(n, JR_Z_e8, 0x28)			\
X(n, ADD_HL_HL, 0x29)		\
X(n, LD_A_HL_INC_NI, 0x2A)	\
X(n, DEC_HL, 0x2B)			\
X(n, INC_L, 0x2C)			\
X(n, DEC_L, 0x2D)			\
X(n, LD_L_n8, 0x2E)			\
X(n, CPL, 0x2F)				\
X(n, JR_NC_e8, 0x30)		\
X(n, LD_SP_n16, 0x31)		\
X(n, LD_HL_DEC_NI_A, 0x32)	\
X(n, INC_SP, 0x33)			\
X(n, INC_HL_NI, 0x34)		\
X(n, DEC_HL_NI, 0x35)		\
X(n, LD_HL_NI_n8, 0x36)		\
X(n, SCF, 0x37)				\
X(n, JR_C_e8, 0x38)			\
X(n, ADD_HL_SP, 0x39)		\
X(n, LD_A_HL_DEC_NI, 0x3A)	\
X(n, DEC_SP, 0x3B)			\
X(n, INC_A, 0x3C)			\
X(n, DEC_A, 0x3D)			\
X(n, LD_A_n8, 0x3E)			\
X(n, CCF, 0x3F)				\
X(n, LD_B_B, 0x40)			\
X(n, LD_B_C, 0x41)			\
X(n, LD_B_D, 0x42)			\
X(n, LD_B_E, 0x43)			\
X(n, LD_B_H, 0x44)			\
X(n, LD_B_L, 0x45)			\
X(n, LD_B_HL_NI, 0x46)		\
X(n, LD_B_A, 0x47)			\
X(n, LD_C_B, 0x48)			\
X(n, LD_C_C, 0x49)			\
X(n, LD_C_D, 0x4A)			\
X(n, LD_C_E, 0x4B)			\
X(n, LD_C_H, 0x4C)			\
X(n, LD_C_L, 0x4D)			\
X(n, LD_C_HL_NI, 0x4E)		\
X(n, LD_C_A, 0x4F)			\
X(n, LD_D_B, 0x50)			\
X(n, LD_D_C, 0x51)			\
X(n, LD_D_D, 0x52)			\
X(n, LD_D_E, 0x53)			\
X(n, LD_D_H, 0x54)			\
X(n, LD_D_L, 0x55)			\
X(n, LD_D_HL_NI, 0x56)		\
X(n, LD_D_A, 0x57)			\
X(n, LD_E_B, 0x58)			\
X(n, LD_E_C, 0x59)			\
X(n, LD_E_D, 0x5A)			\
X(n, LD_E_E, 0x5B)			\
X(n, LD_E_H, 0x5C)			\
X(n, LD_E_L, 0x5D)			\
X(n, LD_E_HL_NI, 0x5E)		\
X(n, LD_E_A, 0x5F)			\
X(n, LD_H_B, 0x60)			\
X(n, LD_H_C, 0x61)			\
X(n, LD_H_D, 0x62)			\
X(n, LD_H_E, 0x63)			\
X(n, LD_H_H, 0x64)			\
X(n, LD_H_L, 0x65)			\
X(n, LD_H_HL_NI, 0x66)		\
X(n, LD_H_A, 0x67)			\
X(n, LD_L_B, 0x68)			\
X(n, LD_L_C, 0x69)			\
X(n, LD_L_D, 0x6A)			\
X(n, LD_L_E, 0x6B)			\
X(n, LD_L_H, 0x6C)			\
X(n, LD_L_L, 0x6D)			\
X(n, LD_L_HL_NI, 0x6E)		\
X(n, LD_L_A, 0x6F)			\
X(n, LD_HL_NI_B, 0x70)		\
X(n, LD_HL_NI_C, 0x71)		\
X(n, LD_HL_NI_D, 0x72)		\
X(n, LD_HL_NI_E, 0x73)		\
X(n, LD_HL_NI_H, 0x74)		\
X(n, LD_HL_NI_L, 0x75)		\
X(n, HALT, 0x76)			\
X(n, LD_HL_NI_A, 0x77)		\
X(n, LD_A_B, 0x78)			\
X(n, LD_A_C, 0x79)			\
X(n, LD_A_D, 0x7A)			\
X(n, LD_A_E, 0x7B)			\
X(n, LD_A_H, 0x7C)			\
X(n, LD_A_L, 0x7D)			\
X(n, LD_A_HL_NI, 0x7E)		\
X(n, LD_A_A, 0x7F)			\
X(n, ADD_A_B, 0x80)			\
X(n, ADD_A_C, 0x81)			\
X(n, ADD_A_D, 0x82)			\
X(n, ADD_A_E, 0x83)			\
X(n, ADD_A_H, 0x84)			\
X(n, ADD_A_L, 0x85)			\
X(n, ADD_A_HL_NI, 0x86)		\
X(n, ADD_A_A, 0x87)			\
X(n, ADC_A_B, 0x88)			\
X(n, ADC_A_C, 0x89)			\
X(n, ADC_A_D, 0x8A)			\
X(n, ADC_A_E, 0x8B)			\
X(n, ADC_A_H, 0x8C)			\
X(n, ADC_A_L, 0x8D)			\
X(n, ADC_A_HL_NI, 0x8E)		\
X(n, ADC_A_A, 0x8F)			\
X(n, SUB_A_B, 0x90)			\
X(n, SUB_A_C, 0x91)			\
X(n, SUB_A_D, 0x92)			\
X(n, SUB_A_E, 0x93)			\
X(n, SUB_A_H, 0x94)			\
X(n, SUB_A_L, 0x95)			\
X(n, SUB_A_HL_NI, 0x96)		\
X(n, SUB_A_A, 0x97)			\
X(n, SBC_A_B, 0x98)			\
X(n, SBC_A_C, 0x99)			\
X(n, SBC_A_D, 0x9A)			\
X(n, SBC_A_E, 0x9B)			\
X(n, SBC_A_H, 0x9C)			\
X(n, SBC_A_L, 0x9D)			\
X(n, SBC_A_HL_NI, 0x9E)		\
X(n, SBC_A_A, 0x9F)			\
X(n, AND_A_B, 0xA0)			\
X(n, AND_A_C, 0xA1)			\
X(n, AND_A_D, 0xA2)			\
X(n, AND_A_E, 0xA3)			\
X(n, AND_A_H, 0xA4)			\
X(n, AND_A_L, 0xA5)			\
X(n, AND_A_HL_NI, 0xA6)		\
X(n, AND_A_A, 0xA7)			\
X(n, XOR_A_B, 0xA8)			\
X(n, XOR_A_C, 0xA9)			\
X(n, XOR_A_D, 0xAA)			\
X(n, XOR_A_E, 0xAB)			\
X(n, XOR_A_H, 0xAC)			\
X(n, XOR_A_L, 0xAD)			\
X(n, XOR_A_HL_NI, 0xAE)		\
X(n, XOR_A_A, 0xAF)			\
X(n, OR_A_B, 0xB0)			\
X(n, OR_A_C, 0xB1)			\
X(n, OR_A_D, 0xB2)			\
X(n, OR_A_E, 0xB3)			\
X(n, OR_A_H, 0xB4)			\
X(n, OR_A_L, 0xB5)			\
X(n, OR_A_HL_NI, 0xB6)		\
X(n, OR_A_A, 0xB7)			\
X(n, CP_A_B, 0xB8)			\
X(n, CP_A_C, 0xB9)			\
X(n, CP_A_D, 0xBA)			\
X(n, CP_A_E, 0xBB)			\
X(n, CP_A_H, 0xBC)			\
X(n, CP_A_L, 0xBD)			\
X(n, CP_A_HL_NI, 0xBE)		\
X(n, CP_A_A, 0xBF)			\
X(n, RET_NZ, 0xC0)			\
X(n, POP_BC, 0xC1)			\
X(n, JP_NZ_a16, 0xC2)		\
X(n, JP_a16, 0xC3)			\
X(n, CALL_NZ_a16, 0xC4)		\
X(n, PUSH_BC, 0xC5)			\
X(n, ADD_A_n8, 0xC6)		\
X(n, RST_$00, 0xC7)			\
X(n, RET_Z, 0xC8)			\
X(n, RET, 0xC9)				\
X(n, JP_Z_a16, 0xCA)		\
X(n, PREFIX, 0xCB)			\
X(n, CALL_Z_a16, 0xCC)		\
X(n, CALL_a16, 0xCD)		\
X(n, ADC_A_n8, 0xCE)		\
X(n, RST_$08, 0xCF)			\
X(n, RET_NC, 0xD0)			\
X(n, POP_DE, 0xD1)			\
X(n, JP_NC_a16, 0xD2)		\
X(n, ILLEGAL_D3, 0xD3)		\
X(n, CALL_NC_a16, 0xD4)		\
X(n, PUSH_DE, 0xD5)			\
X(n, SUB_A_n8, 0xD6)		\
X(n, RST_$10, 0xD7)			\
X(n, RET_C, 0xD8)			\
X(n, RETI, 0xD9)			\
X(n, JP_C_a16, 0xDA)		\
X(n, ILLEGAL_DB, 0xDB)		\
X(n, CALL_C_a16, 0xDC)		\
X(n, ILLEGAL_DD, 0xDD)		\
X(n, SBC_A_n8, 0xDE)		\
X(n, RST_$18, 0xDF)			\
X(n, LDH_a8_NI_A, 0xE0)		\
X(n, POP_HL, 0xE1)			\
X(n, LDH_C_NI_A, 0xE2)		\
X(n, ILLEGAL_E3, 0xE3)		\
X(n, ILLEGAL_E4, 0xE4)		\
X(n, PUSH_HL, 0xE5)			\
X(n, AND_A_n8, 0xE6)		\
X(n, RST_$20, 0xE7)			\
X(n, ADD_SP_e8, 0xE8)		\
X(n, JP_HL, 0xE9)			\
X(n, LD_a16_NI_A, 0xEA)		\
X(n, ILLEGAL_EB, 0xEB)		\
X(n, ILLEGAL_EC, 0xEC)		\
X(n, ILLEGAL_ED, 0xED)		\
X(n, XOR_A_n8, 0xEE)		\
X(n, RST_$28, 0xEF)			\
X(n, LDH_A_a8_NI, 0xF0)		\
X(n, POP_AF, 0xF1)			\
X(n, LDH_A_C_NI, 0xF2)		\
X(n, DI, 0xF3)				\
X(n, ILLEGAL_F4, 0xF4)		\
X(n, PUSH_AF, 0xF5)			\
X(n, OR_A_n8, 0xF6)			\
X(n, RST_$30, 0xF7)			\
X(n, LD_HL_SP_INC_e8, 0xF8)	\
X(n, LD_SP_HL, 0xF9)		\
X(n, LD_A_a16_NI, 0xFA)		\
X(n, EI, 0xFB)				\
X(n, ILLEGAL_FC, 0xFC)		\
X(n, ILLEGAL_FD, 0xFD)		\
X(n, CP_A_n8, 0xFE)			\
X(n, RST_$38, 0xFF)

/**
 * @brief
 */
CREATE_ENUM_WITH_UTILS(GAMEBOY_OPCODES_SEQ, OpCode)
#undef GAMEBOY_OPCODES_SEQ

#define GAMEBOY_PREFIXED_OPCODES_SEQ(X, n) \
	X(n, RLC_B, 0x00)			\
	X(n, RLC_C, 0x01)			\
	X(n, RLC_D, 0x02)			\
	X(n, RLC_E, 0x03)			\
	X(n, RLC_H, 0x04)			\
	X(n, RLC_L, 0x05)			\
	X(n, RLC_HL_NI, 0x06)		\
	X(n, RLC_A, 0x07)			\
	X(n, RRC_B, 0x08)			\
	X(n, RRC_C, 0x09)			\
	X(n, RRC_D, 0x0A)			\
	X(n, RRC_E, 0x0B)			\
	X(n, RRC_H, 0x0C)			\
	X(n, RRC_L, 0x0D)			\
	X(n, RRC_HL_NI, 0x0E)		\
	X(n, RRC_A, 0x0F)			\
	X(n, RL_B, 0x10)			\
	X(n, RL_C, 0x11)			\
	X(n, RL_D, 0x12)			\
	X(n, RL_E, 0x13)			\
	X(n, RL_H, 0x14)			\
	X(n, RL_L, 0x15)			\
	X(n, RL_HL_NI, 0x16)		\
	X(n, RL_A, 0x17)			\
	X(n, RR_B, 0x18)			\
	X(n, RR_C, 0x19)			\
	X(n, RR_D, 0x1A)			\
	X(n, RR_E, 0x1B)			\
	X(n, RR_H, 0x1C)			\
	X(n, RR_L, 0x1D)			\
	X(n, RR_HL_NI, 0x1E)		\
	X(n, RR_A, 0x1F)			\
	X(n, SLA_B, 0x20)			\
	X(n, SLA_C, 0x21)			\
	X(n, SLA_D, 0x22)			\
	X(n, SLA_E, 0x23)			\
	X(n, SLA_H, 0x24)			\
	X(n, SLA_L, 0x25)			\
	X(n, SLA_HL_NI, 0x26)		\
	X(n, SLA_A, 0x27)			\
	X(n, SRA_B, 0x28)			\
	X(n, SRA_C, 0x29)			\
	X(n, SRA_D, 0x2A)			\
	X(n, SRA_E, 0x2B)			\
	X(n, SRA_H, 0x2C)			\
	X(n, SRA_L, 0x2D)			\
	X(n, SRA_HL_NI, 0x2E)		\
	X(n, SRA_A, 0x2F)			\
	X(n, SWAP_B, 0x30)			\
	X(n, SWAP_C, 0x31)			\
	X(n, SWAP_D, 0x32)			\
	X(n, SWAP_E, 0x33)			\
	X(n, SWAP_H, 0x34)			\
	X(n, SWAP_L, 0x35)			\
	X(n, SWAP_HL_NI, 0x36)		\
	X(n, SWAP_A, 0x37)			\
	X(n, SRL_B, 0x38)			\
	X(n, SRL_C, 0x39)			\
	X(n, SRL_D, 0x3A)			\
	X(n, SRL_E, 0x3B)			\
	X(n, SRL_H, 0x3C)			\
	X(n, SRL_L, 0x3D)			\
	X(n, SRL_HL_NI, 0x3E)		\
	X(n, SRL_A, 0x3F)			\
	X(n, BIT_0_B, 0x40)			\
	X(n, BIT_0_C, 0x41)			\
	X(n, BIT_0_D, 0x42)			\
	X(n, BIT_0_E, 0x43)			\
	X(n, BIT_0_H, 0x44)			\
	X(n, BIT_0_L, 0x45)			\
	X(n, BIT_0_HL_NI, 0x46)		\
	X(n, BIT_0_A, 0x47)			\
	X(n, BIT_1_B, 0x48)			\
	X(n, BIT_1_C, 0x49)			\
	X(n, BIT_1_D, 0x4A)			\
	X(n, BIT_1_E, 0x4B)			\
	X(n, BIT_1_H, 0x4C)			\
	X(n, BIT_1_L, 0x4D)			\
	X(n, BIT_1_HL_NI, 0x4E)		\
	X(n, BIT_1_A, 0x4F)			\
	X(n, BIT_2_B, 0x50)			\
	X(n, BIT_2_C, 0x51)			\
	X(n, BIT_2_D, 0x52)			\
	X(n, BIT_2_E, 0x53)			\
	X(n, BIT_2_H, 0x54)			\
	X(n, BIT_2_L, 0x55)			\
	X(n, BIT_2_HL_NI, 0x56)		\
	X(n, BIT_2_A, 0x57)			\
	X(n, BIT_3_B, 0x58)			\
	X(n, BIT_3_C, 0x59)			\
	X(n, BIT_3_D, 0x5A)			\
	X(n, BIT_3_E, 0x5B)			\
	X(n, BIT_3_H, 0x5C)			\
	X(n, BIT_3_L, 0x5D)			\
	X(n, BIT_3_HL_NI, 0x5E)		\
	X(n, BIT_3_A, 0x5F)			\
	X(n, BIT_4_B, 0x60)			\
	X(n, BIT_4_C, 0x61)			\
	X(n, BIT_4_D, 0x62)			\
	X(n, BIT_4_E, 0x63)			\
	X(n, BIT_4_H, 0x64)			\
	X(n, BIT_4_L, 0x65)			\
	X(n, BIT_4_HL_NI, 0x66)		\
	X(n, BIT_4_A, 0x67)			\
	X(n, BIT_5_B, 0x68)			\
	X(n, BIT_5_C, 0x69)			\
	X(n, BIT_5_D, 0x6A)			\
	X(n, BIT_5_E, 0x6B)			\
	X(n, BIT_5_H, 0x6C)			\
	X(n, BIT_5_L, 0x6D)			\
	X(n, BIT_5_HL_NI, 0x6E)		\
	X(n, BIT_5_A, 0x6F)			\
	X(n, BIT_6_B, 0x70)			\
	X(n, BIT_6_C, 0x71)			\
	X(n, BIT_6_D, 0x72)			\
	X(n, BIT_6_E, 0x73)			\
	X(n, BIT_6_H, 0x74)			\
	X(n, BIT_6_L, 0x75)			\
	X(n, BIT_6_HL_NI, 0x76)		\
	X(n, BIT_6_A, 0x77)			\
	X(n, BIT_7_B, 0x78)			\
	X(n, BIT_7_C, 0x79)			\
	X(n, BIT_7_D, 0x7A)			\
	X(n, BIT_7_E, 0x7B)			\
	X(n, BIT_7_H, 0x7C)			\
	X(n, BIT_7_L, 0x7D)			\
	X(n, BIT_7_HL_NI, 0x7E)		\
	X(n, BIT_7_A, 0x7F)			\
	X(n, RES_0_B, 0x80)			\
	X(n, RES_0_C, 0x81)			\
	X(n, RES_0_D, 0x82)			\
	X(n, RES_0_E, 0x83)			\
	X(n, RES_0_H, 0x84)			\
	X(n, RES_0_L, 0x85)			\
	X(n, RES_0_HL_NI, 0x86)		\
	X(n, RES_0_A, 0x87)			\
	X(n, RES_1_B, 0x88)			\
	X(n, RES_1_C, 0x89)			\
	X(n, RES_1_D, 0x8A)			\
	X(n, RES_1_E, 0x8B)			\
	X(n, RES_1_H, 0x8C)			\
	X(n, RES_1_L, 0x8D)			\
	X(n, RES_1_HL_NI, 0x8E)		\
	X(n, RES_1_A, 0x8F)			\
	X(n, RES_2_B, 0x90)			\
	X(n, RES_2_C, 0x91)			\
	X(n, RES_2_D, 0x92)			\
	X(n, RES_2_E, 0x93)			\
	X(n, RES_2_H, 0x94)			\
	X(n, RES_2_L, 0x95)			\
	X(n, RES_2_HL_NI, 0x96)		\
	X(n, RES_2_A, 0x97)			\
	X(n, RES_3_B, 0x98)			\
	X(n, RES_3_C, 0x99)			\
	X(n, RES_3_D, 0x9A)			\
	X(n, RES_3_E, 0x9B)			\
	X(n, RES_3_H, 0x9C)			\
	X(n, RES_3_L, 0x9D)			\
	X(n, RES_3_HL_NI, 0x9E)		\
	X(n, RES_3_A, 0x9F)			\
	X(n, RES_4_B, 0xA0)			\
	X(n, RES_4_C, 0xA1)			\
	X(n, RES_4_D, 0xA2)			\
	X(n, RES_4_E, 0xA3)			\
	X(n, RES_4_H, 0xA4)			\
	X(n, RES_4_L, 0xA5)			\
	X(n, RES_4_HL_NI, 0xA6)		\
	X(n, RES_4_A, 0xA7)			\
	X(n, RES_5_B, 0xA8)			\
	X(n, RES_5_C, 0xA9)			\
	X(n, RES_5_D, 0xAA)			\
	X(n, RES_5_E, 0xAB)			\
	X(n, RES_5_H, 0xAC)			\
	X(n, RES_5_L, 0xAD)			\
	X(n, RES_5_HL_NI, 0xAE)		\
	X(n, RES_5_A, 0xAF)			\
	X(n, RES_6_B, 0xB0)			\
	X(n, RES_6_C, 0xB1)			\
	X(n, RES_6_D, 0xB2)			\
	X(n, RES_6_E, 0xB3)			\
	X(n, RES_6_H, 0xB4)			\
	X(n, RES_6_L, 0xB5)			\
	X(n, RES_6_HL_NI, 0xB6)		\
	X(n, RES_6_A, 0xB7)			\
	X(n, RES_7_B, 0xB8)			\
	X(n, RES_7_C, 0xB9)			\
	X(n, RES_7_D, 0xBA)			\
	X(n, RES_7_E, 0xBB)			\
	X(n, RES_7_H, 0xBC)			\
	X(n, RES_7_L, 0xBD)			\
	X(n, RES_7_HL_NI, 0xBE)		\
	X(n, RES_7_A, 0xBF)			\
	X(n, SET_0_B, 0xC0)			\
	X(n, SET_0_C, 0xC1)			\
	X(n, SET_0_D, 0xC2)			\
	X(n, SET_0_E, 0xC3)			\
	X(n, SET_0_H, 0xC4)			\
	X(n, SET_0_L, 0xC5)			\
	X(n, SET_0_HL_NI, 0xC6)		\
	X(n, SET_0_A, 0xC7)			\
	X(n, SET_1_B, 0xC8)			\
	X(n, SET_1_C, 0xC9)			\
	X(n, SET_1_D, 0xCA)			\
	X(n, SET_1_E, 0xCB)			\
	X(n, SET_1_H, 0xCC)			\
	X(n, SET_1_L, 0xCD)			\
	X(n, SET_1_HL_NI, 0xCE)		\
	X(n, SET_1_A, 0xCF)			\
	X(n, SET_2_B, 0xD0)			\
	X(n, SET_2_C, 0xD1)			\
	X(n, SET_2_D, 0xD2)			\
	X(n, SET_2_E, 0xD3)			\
	X(n, SET_2_H, 0xD4)			\
	X(n, SET_2_L, 0xD5)			\
	X(n, SET_2_HL_NI, 0xD6)		\
	X(n, SET_2_A, 0xD7)			\
	X(n, SET_3_B, 0xD8)			\
	X(n, SET_3_C, 0xD9)			\
	X(n, SET_3_D, 0xDA)			\
	X(n, SET_3_E, 0xDB)			\
	X(n, SET_3_H, 0xDC)			\
	X(n, SET_3_L, 0xDD)			\
	X(n, SET_3_HL_NI, 0xDE)		\
	X(n, SET_3_A, 0xDF)			\
	X(n, SET_4_B, 0xE0)			\
	X(n, SET_4_C, 0xE1)			\
	X(n, SET_4_D, 0xE2)			\
	X(n, SET_4_E, 0xE3)			\
	X(n, SET_4_H, 0xE4)			\
	X(n, SET_4_L, 0xE5)			\
	X(n, SET_4_HL_NI, 0xE6)		\
	X(n, SET_4_A, 0xE7)			\
	X(n, SET_5_B, 0xE8)			\
	X(n, SET_5_C, 0xE9)			\
	X(n, SET_5_D, 0xEA)			\
	X(n, SET_5_E, 0xEB)			\
	X(n, SET_5_H, 0xEC)			\
	X(n, SET_5_L, 0xED)			\
	X(n, SET_5_HL_NI, 0xEE)		\
	X(n, SET_5_A, 0xEF)			\
	X(n, SET_6_B, 0xF0)			\
	X(n, SET_6_C, 0xF1)			\
	X(n, SET_6_D, 0xF2)			\
	X(n, SET_6_E, 0xF3)			\
	X(n, SET_6_H, 0xF4)			\
	X(n, SET_6_L, 0xF5)			\
	X(n, SET_6_HL_NI, 0xF6)		\
	X(n, SET_6_A, 0xF7)			\
	X(n, SET_7_B, 0xF8)			\
	X(n, SET_7_C, 0xF9)			\
	X(n, SET_7_D, 0xFA)			\
	X(n, SET_7_E, 0xFB)			\
	X(n, SET_7_H, 0xFC)			\
	X(n, SET_7_L, 0xFD)			\
	X(n, SET_7_HL_NI, 0xFE)		\
	X(n, SET_7_A, 0xFF)

/**
 * @brief
 */
CREATE_ENUM_WITH_UTILS(GAMEBOY_PREFIXED_OPCODES_SEQ, PrefixedOpCode)
#undef GAMEBOY_PREFIXED_OPCODES_SEQ

} /* namespace Gameboy */
