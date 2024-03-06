#pragma once

#include "Apu_Impl.h"
#include "Mmu.h"

namespace Nes
{
	class Apu::Impl::Io
	{
	public:
		// static MappedRead MapReadPrg(const Hardware& hw, addr16 addr);
		static MappedWrite MapWritePrg(Hardware& hw, addr16 addr);
	};
}
