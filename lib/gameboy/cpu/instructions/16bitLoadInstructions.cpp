#include <play-man/gameboy/cpu/Cpu.hpp>

namespace GameBoy
{

	size_t Cpu::Load_16bit_ImmediateData(Register& reg)
	{
		reg.SetLowByte(FetchPcAddress());
		reg.SetHighByte(FetchPcAddress());

		constexpr auto numberOfCycles = 3;
		return numberOfCycles;
	}

} /* namespace GameBoy */
