#pragma once
#include "IMapper.h"

namespace Nes
{
	class MapperEmpty final : public IMapper
	{
	public:
		MappedRead MapReadPrg(addr16 addr) override { return {}; }
		MappedWrite MapWritePrg(addr16 addr) override { return {}; }
		MappedRead MapReadChr(addr16 addr) override { return {}; }
		MappedWrite MapPrgWriteChr(addr16 addr) override { return {}; }
	};
}
