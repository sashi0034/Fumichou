#pragma once
#include "IMapper.h"

namespace Nes
{
	class MapperNrom final : public IMapper
	{
	public:
		void Initialize(const RomData& rom) override;
		MappedRead MapReadPrg(const RomData& rom, addr16 addr) override;
		MappedWrite MapWritePrg(RomData& rom, addr16 addr) override;
		MappedRead MapReadChr(const RomData& rom, addr16 addr) override;
		MappedWrite MapPrgWriteChr(RomData& rom, addr16 addr) override;

	private:
		bool m_oneBank{};
		Array<uint8> m_chrRam{};
	};
}
