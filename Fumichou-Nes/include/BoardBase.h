#pragma once

#include "Mmu.h"
#include "RomData.h"

namespace Nes
{
	class BoardBase
	{
	public:
		BoardBase(const RomData& rom): m_rom(std::move(rom)) { return; }
		const RomData& GetRom() { return m_rom; }

		virtual ~BoardBase() = default;

		virtual void Initialize() =0;

		virtual MappedRead MapReadPrg(addr16 addr) =0;
		virtual MappedWrite MapWritePrg(addr16 addr) =0;

		virtual MappedRead MapReadChr(addr16 addr) =0;
		virtual MappedWrite MapPrgWriteChr(addr16 addr) =0;

	protected:
		RomData m_rom{};
	};
}
