#pragma once
#include "Apu.h"
#include "Cartridge.h"
#include "Ram.h"
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

		Ram& GetRam() { return m_ram; }
		const Ram& GetRam() const { return m_ram; }

		Cartridge& GetCartridge() { return m_cartridge; }
		const Cartridge& GetCartridge() const { return m_cartridge; }

		Mos6502& GetMos6502() { return m_mos6502; }
		const Mos6502& GetMos6502() const { return m_mos6502; }

		Ppu& GetPpu() { return m_ppu; }
		const Ppu& GetPpu() const { return m_ppu; }

		Apu& GetApu() { return m_apu; }
		const Apu& GetApu() const { return m_apu; }

	private:
		Mmu m_mmu{};
		Ram m_ram{};
		Cartridge m_cartridge{};
		Mos6502 m_mos6502{};
		Ppu m_ppu{};
		Apu m_apu{};
	};
}
