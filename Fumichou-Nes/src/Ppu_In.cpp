#include "stdafx.h"
#include "Ppu_In.h"

#include "Hardware.h"
#include "Mos6502_In.h"

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
				Mos6502::In::Nmi(hw.GetMos6502(), hw.GetMmu());
			}
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
}
