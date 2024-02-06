#pragma once
#include "IMapper.h"

namespace Nes
{
	class MapperEmpty final : public IMapper
	{
	public:
		void Initialize(const RomData& rom) override { return; };
		MappedRead MapReadPrg(const RomData& rom, addr16 addr) override { return {}; }
		MappedWrite MapWritePrg(RomData& rom, addr16 addr) override { return {}; }
		MappedRead MapReadChr(const RomData& rom, addr16 addr) override { return {}; }
		MappedWrite MapPrgWriteChr(RomData& rom, addr16 addr) override { return {}; }
	};
}
