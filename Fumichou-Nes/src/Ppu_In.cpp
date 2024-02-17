#include "stdafx.h"
#include "Ppu_In.h"

#include "Hardware.h"
#include "Logger.h"
#include "Mos6502_In.h"
#include "Ppu_In_Render.h"

using namespace Nes;

namespace
{
	constexpr PpuCycle cyclesPerLine_341 = 341;
	constexpr int maxLines_262 = 262;
}

class Ppu::In::Impl
{
public:
	static void Step(Hardware& hw, PpuCycle cycle)
	{
		auto&& ppu = hw.GetPpu();
		ppu.m_lineCycles += cycle;
		if (ppu.m_lineCycles >= cyclesPerLine_341)
		{
			ppu.m_lineCycles -= cycle;
			ppu.m_scanLine = (ppu.m_scanLine + 1) % maxLines_262;

			beginScanLine(hw, ppu.m_scanLine);
		}
	}

private:
	static void beginScanLine(Hardware& hw, uint32 scanLine)
	{
		auto& ppu = hw.GetPpu();
		switch (scanLine)
		{
		case 241: {
			// 垂直同期フラグ
			ppu.m_unstable.status.VBlank().Set(true);
			if (ppu.m_regs.control.NmiEnabled())
			{
				// NMI割り込み
				Mos6502::In::RequestNmi(hw.GetMos6502());
			}

			// ディスプレイ描画
			Renderer::Render({
				.ppu = hw.GetPpu(),
				.mmu = hw.GetMmu(),
				.board = hw.GetCartridge().GetBoard()
			});
			break;
		}
		default:
			break;
		}
	}
};

namespace Nes
{
	void Ppu::In::Step(Hardware& hw, PpuCycle cycle)
	{
		Impl::Step(hw, cycle);
	}

	// https://www.nesdev.org/wiki/Mirroring
	void Ppu::In::UpdateMirroring(Ppu& ppu, NameTableMirror mirror)
	{
		ppu.m_mirroring = mirror;
		switch (mirror)
		{
		case NameTableMirror::Horizontal:
			ppu.m_nametableOffset = {0x000, 0x000, 0x400, 0x400};
			break;
		case NameTableMirror::Vertical:
			ppu.m_nametableOffset = {0x000, 0x400, 0x000, 0x400};
			break;
		case NameTableMirror::SingleLower:
			ppu.m_nametableOffset = {0x000, 0x000, 0x000, 0x000};
			break;
		case NameTableMirror::SingleHigher:
			ppu.m_nametableOffset = {0x400, 0x400, 0x400, 0x400};
			break;
		case NameTableMirror::FourScreen:
			ppu.m_nametableOffset = {0x000, 0x400, 0x800, 0xC00};
			break;
		default: ;
			Logger::Abort();
		}
	}

	void Ppu::In::applyPaletteMirror(Ppu& ppu)
	{
		constexpr uint16 mirror = 0x0010;
		ppu.m_palettes[0x0010] = ppu.m_palettes[0x0010 - mirror];
		ppu.m_palettes[0x0014] = ppu.m_palettes[0x0014 - mirror];
		ppu.m_palettes[0x0018] = ppu.m_palettes[0x0018 - mirror];
		ppu.m_palettes[0x001C] = ppu.m_palettes[0x001C - mirror];
	}

	uint8 Ppu::In::readPalette(const Ppu& ppu, uint8 paletteIndex)
	{
		// TODO: 最適化
		if (paletteIndex == 0x0010 || paletteIndex == 0x0014 || paletteIndex == 0x0018 || paletteIndex == 0x001C)
		{
			paletteIndex -= 0x0010;
		}
		return ppu.m_palettes[paletteIndex];
	}

	void Ppu::In::writePalette(Ppu& ppu, uint8 paletteIndex, uint8 value)
	{
		// TODO: 最適化
		if (paletteIndex == 0x0010 || paletteIndex == 0x0014 || paletteIndex == 0x0018 || paletteIndex == 0x001C)
		{
			paletteIndex -= 0x0010;
		}
		ppu.m_palettes[paletteIndex] = value;
	}
}
