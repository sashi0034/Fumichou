#pragma once
#include "Memory.h"
#include "Mmu.h"
#include "Mos6502.h"
#include "Ppu.h"

namespace Nes
{
	class Hardware
	{
	public:
		Mmu& GetMmu() { return m_mmu; }
		const Mmu& GetMmu() const { return m_mmu; }

		Memory& GetMemory() { return m_memory; }
		const Memory& GetMemory() const { return m_memory; }

		Mos6502& GetMos6502() { return m_mos6502; }
		const Mos6502& GetMos6502() const { return m_mos6502; }

		Ppu& GetPpu() { return m_ppu; }
		const Ppu& GetPpu() const { return m_ppu; }

	private:
		Mmu m_mmu{};
		Memory m_memory{};
		Mos6502 m_mos6502{};
		Ppu m_ppu{};
	};
}
