#pragma once

#include "Mmu.h"
#include "Ppu_In.h"

namespace Nes
{
	class Ppu::In::Io
	{
	public:
		static MappedRead MapReadPrg(const Hardware& hw, addr16 addr);
		static MappedWrite MapWritePrg(Hardware& hw, addr16 addr);
		static MappedWrite MapWritePrg_0x4014(Hardware& hw);
	};
}
