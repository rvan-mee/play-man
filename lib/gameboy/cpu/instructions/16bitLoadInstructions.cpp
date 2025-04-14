#include <play-man/gameboy/cpu/Cpu.hpp>

namespace GameBoy
{
	size_t Cpu::Load_16bit_ImmediateData(Register CpuCore::* reg)
	{
		Register& r = core.*reg;
		r.SetLowByte(FetchPcAddress());
		r.SetHighByte(FetchPcAddress());

		constexpr auto numberOfCycles = 3;
		return numberOfCycles;
	}

} /* namespace GameBoy */
