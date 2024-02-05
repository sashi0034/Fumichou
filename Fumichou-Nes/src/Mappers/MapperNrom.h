#pragma once
#include "IMapper.h"

namespace Nes
{
	class MapperNrom final : public IMapper
	{
	public:
		MappedRead MapReadPrg(addr16 addr) override;
		MappedWrite MapWritePrg(addr16 addr) override;
		MappedRead MapReadChr(addr16 addr) override;
		MappedWrite MapPrgWriteChr(addr16 addr) override;
	};
}
