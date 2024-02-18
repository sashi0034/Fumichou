#pragma once

#include "Apu_In.h"
#include "Mmu.h"

namespace Nes
{
	class Apu::In::Io
	{
	public:
		// static MappedRead MapReadPrg(const Hardware& hw, addr16 addr);
		static MappedWrite MapWritePrg(Hardware& hw, addr16 addr);
	};
}
