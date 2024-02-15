#pragma once

#include "Mmu.h"
#include "Ppu_In.h"

namespace Nes
{
	class Ppu::In::Mm
	{
	public:
		static MappedRead MapReadChr(const Hardware& hw, addr16 addr);
		static MappedWrite MapWriteChr(Hardware& hw, addr16 addr);
	};
}
