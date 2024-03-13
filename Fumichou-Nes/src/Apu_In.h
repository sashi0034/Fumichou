#pragma once
#include "Apu.h"
#include "Mmu.h"

namespace Nes
{
	class Apu::In
	{
	public:
		static MappedWrite MapWritePrg(Hardware& hw, addr16 addr);
		static void Reset(Apu& apu);
		static void Step(Hardware& hw, CpuCycle cycle);
	};
}
