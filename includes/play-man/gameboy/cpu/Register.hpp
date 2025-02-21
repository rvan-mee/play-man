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

#pragma once

#include <play-man/utility/EnumMacro.hpp>

#include <inttypes.h>

namespace gameboy
{

	#define REGISTER_NAMES_SEQ(X, n) \
		x(n, Flag, 0x00) \
		X(n, A, 0x01) \
		X(n, B, 0x02) \
		X(n, C, 0x03) \
		X(n, D, 0x04) \
		X(n, E, 0x05) \
		x(n, H, 0x06) \
		x(n, L, 0x07) \
		x(n, SP, 0x0a) \
		x(n, PC, 0x0b)

	/**
	 * @brief Enum describing the severity of the log.
	 */
	CREATE_ENUM_WITH_UTILS(REGISTER_NAMES_SEQ, RegisterName)
	#undef REGISTER_NAMES_SEQ

	#define SIXTEEN_BIT_REGISTER_NAMES_SEQ(X, n) \
		X(n, AF, RegisterName::Flag << 8 | RegisterName::A) \
		X(n, BC, RegisterName::B << 8 | RegisterName::C) \
		X(n, DE, RegisterName::D << 8 | RegisterName::E) \
		X(n, HL, RegisterName::H << 8 | RegisterName::L) \
		x(n, RegisterName::SP) \
		x(n, RegisterName::PC)

	CREATE_ENUM_WITH_UTILS(SIXTEEN_BIT_REGISTER_NAMES_SEQ, CombinedRegisterName)
	#undef SIXTEEN_BIT_REGISTER_NAMES_SEQ

	/**
	 * @brief Enum describing the severity of the log.
	 */
	CREATE_ENUM_WITH_UTILS REGISTER_NAMES_SEQ, RegisterName)
	#undef REGISTER_NAMES_SEQ

	/**
	 * @brief A gameboy register :)
	 * 
	 * @tparam Based on the register operations can be made hidden because the stack pointer for examle does not support 8bit operations. 
	 */
	template <CombinedRegisterName CombinedRegister>
	class Register
	{

		/**
		 * @brief Internal layout of the resgisters, all registers are basically 16 bits,
		 *        but some operations can be done on either the low or high byte of a regisers.
		 *        By having a union with an unnamed struct makes us able to share the memory and
		 *        easily access the high and low byte of the register without any bitshift shenanigans.
		 */
		union InternalRegisterLayout
		{
			struct {
				uint8_t lowByte;
				uint8_t highByte;
			}
			uint16_t value;
		};

		InternalRegisterLayout internalRegister;

	public:

		Register(uint16_t initialValue = 0x0000);


	};

	#include "Register.ipp"

} /* namespace gameboy */
