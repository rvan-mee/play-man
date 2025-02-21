
#include "Register.hpp"

template <CombinedRegisterName CombinedRegisterName>
Register::Register(uint16_t initialValue)
{
	internalRegister.value = initialValue;
}
