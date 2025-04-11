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

#include <inttypes.h>

namespace GameBoy
{
	/**
	 * @brief A gameboy register :)
	 * 
	 * @tparam Based on the register operations can be made hidden because the stack pointer for examle does not support 8bit operations. 
	 */
	class Register
	{

		/**
		 * @brief Internal layout of the registers, all registers are basically 16 bits,
		 *        but some operations can be done on either the low or high byte of a register.
		 *        By having a union with an unnamed struct makes us able to share the memory and
		 *        easily access the high and low byte of the register without any bitshift shenanigans.
		 */
		union __attribute__((packed)) InternalRegisterLayout
		{
			struct {
				uint8_t low;
				uint8_t high;
			} byte;
			uint16_t value;
		};

		InternalRegisterLayout internalRegister;

	public:

		/**
		 * @brief -.
		 * @param initialValue 
		 */
		Register(uint16_t initialValue = 0x0000);

		/**
		 * @brief -.
		 * @param value 
		 */
		void SetLowByte(uint8_t value);

		/**
		 * @brief -.
		 * @param value 
		 */
		void SetHighByte(uint8_t value);

		/**
		 * @brief -.
		 * @param value 
		 */
		void SetValue(uint16_t value);

		/**
		 * @brief -.
		 * @return uint8_t 
		 */
		uint8_t LowByte() const;

		/**
		 * @brief -.
		 * @return uint8_t 
		 */
		uint8_t HighByte() const;

		/**
		 * @brief -.
		 * @return uint16_t 
		 */
		uint16_t Value() const;

		/**
		 * @brief -.
		 * @return uint8_t 
		 */
		uint8_t& LowByteRef();

		/**
		 * @brief -.
		 * @return uint8_t 
		 */
		uint8_t& HighByteRef();

		/**
		 * @brief -.
		 * @return uint16_t 
		 */
		uint16_t& ValueRef();

		// Postfix increment 
		Register operator++(int)
		{
			Register temp = *this;
			internalRegister.value++;
			return (temp);
		}

	};


} /* namespace gameboy */
