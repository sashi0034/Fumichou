#include "stdafx.h"
#include "Ppu_In.h"

#include "Hardware.h"
#include "Logger.h"

using namespace Nes;

namespace
{
	constexpr PpuCycle cyclesPerLine_341 = 341;
	constexpr int maxLines_262 = 262;
	constexpr StringView mirrorDesc = U"Mirrors of PPU Register";
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
		switch (scanLine)
		{
		case 241: {
			// TODO: VBLANKフラグとNMI割り込み
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

	MappedRead Ppu::In::MapReadPrg(const Hardware& hw, addr16 addr)
	{
		auto& ppu = hw.GetPpu();
		if (not AddrRange<addr16>(0x2000, 0x3FFF).IsBetween(addr)) Logger::Abort();

		const bool isMirror = addr >= 0x2008;
		switch (0x2000 | (addr & 0x7))
		{
		case 0x2000:
			return MappedRead{
				.desc = isMirror ? mirrorDesc : U"PPU Control Register"_sv,
				.ctx = nullptr,
				.func = [](const void* ctx, addr16 addr)
				{
					return uint8();
				}
			};
		default: break;
		}

		return MappedRead::Invalid();
	}

	MappedWrite Ppu::In::MapWritePrg(Hardware& hw, addr16 addr)
	{
		auto& ppu = hw.GetPpu();
		if (not AddrRange<addr16>(0x2000, 0x3FFF).IsBetween(addr)) Logger::Abort();

		const bool isMirror = addr >= 0x2008;
		switch (0x2000 | (addr & 0x7))
		{
		case 0x2000:
			return MappedWrite{
				.desc = isMirror ? mirrorDesc : U"PPU Control Register"_sv,
				.ctx = &ppu,
				.func = [](void* ctx, addr16 addr, uint8 value)
				{
					auto& ppu = *static_cast<Ppu*>(ctx);
					ppu.m_regs.control = value;
					ppu.m_regs.tempAddr.NameTableAddr().Set(ppu.m_regs.control.BaseNameTableAddr());
				}
			};
		default: break;
		}

		return MappedWrite::Invalid();
	}
}
