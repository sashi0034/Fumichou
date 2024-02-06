#pragma once

#include "Mmu.h"
#include "RomData.h"

namespace Nes
{
	class IMapper
	{
	public:
		virtual ~IMapper() = default;

		virtual void Initialize(const RomData& rom) = 0;

		virtual MappedRead MapReadPrg(const RomData& rom, addr16 addr) = 0;
		virtual MappedWrite MapWritePrg(RomData& rom, addr16 addr) = 0;

		virtual MappedRead MapReadChr(const RomData& rom, addr16 addr) = 0;
		virtual MappedWrite MapPrgWriteChr(RomData& rom, addr16 addr) = 0;
	};
}
