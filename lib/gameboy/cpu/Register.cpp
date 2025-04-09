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

#include <play-man/gameboy/cpu/Register.hpp>

namespace GameBoy
{
	Register::Register(uint16_t initialValue)
	{
		internalRegister.value = initialValue;
	}

	void Register::SetLowByte(uint8_t value)
	{
		internalRegister.byte.low = value;
	}

	void Register::SetHighByte(uint8_t value)
	{
		internalRegister.byte.high = value;
	}

	void Register::SetValue(uint16_t value)
	{
		internalRegister.value = value;
	}

	uint8_t Register::LowByte() const
	{
		return internalRegister.byte.low;
	}

	uint8_t Register::HighByte() const
	{
		return internalRegister.byte.high;
	}

	uint16_t Register::Value() const
	{
		return internalRegister.value;
	}
	
}
