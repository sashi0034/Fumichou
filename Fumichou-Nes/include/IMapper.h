#pragma once

#include "Mmu.h"

namespace Nes
{
	class IMapper
	{
	public:
		virtual ~IMapper() = default;

		virtual MappedRead MapReadPrg(addr16 addr) = 0;
		virtual MappedWrite MapWritePrg(addr16 addr) = 0;

		virtual MappedRead MapReadChr(addr16 addr) = 0;
		virtual MappedWrite MapPrgWriteChr(addr16 addr) = 0;
	};
}
