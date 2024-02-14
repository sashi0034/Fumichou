#pragma once
#include "Mmu.h"
#include "Ppu.h"

namespace Nes
{
	class Ppu::In
	{
	public:
		static void Step(Hardware& hw, PpuCycle cycle);
		static MappedRead MapReadPrg(const Hardware& hw, addr16 addr);
		static MappedWrite MapWritePrg(Hardware& hw, addr16 addr);

	private:
		class Impl;
	};
}
