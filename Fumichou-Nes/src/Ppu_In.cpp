﻿#include "stdafx.h"
#include "Ppu_In.h"

#include "Hardware.h"
#include "Logger.h"
#include "Mos6502_In.h"

using namespace Nes;

namespace
{
	constexpr PpuCycle cyclesPerLine_341 = 341;
	constexpr int maxLines_262 = 262;
	constexpr StringView mirrorDesc = U"Mirrors of PPU Register";
	constexpr StringView unsupportedDesc = U"Unsupported PPU Address";
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

	MappedRead Ppu::In::MapReadPrg(const Hardware& hw, addr16 addr)
	{
		auto& ppu = hw.GetPpu();
		if (not AddrRange<addr16>(0x2000, 0x3FFF).IsBetween(addr)) Logger::Abort();

		const bool isMirror = addr >= 0x2008;
		switch (0x2000 | (addr & 0x7))
		{
		case 0x2000:
			return MappedRead::Unsupported(MappingType::Cpu, isMirror ? mirrorDesc : unsupportedDesc);
		case 0x2001:
			return MappedRead::Unsupported(MappingType::Cpu, isMirror ? mirrorDesc : unsupportedDesc);
		case 0x2002:
			return MappedRead{
				.desc = isMirror ? mirrorDesc : U"PPU Status Register"_sv,
				.ctx = &ppu,
				.func = [](const void* ctx, addr16)
				{
					auto& ppu = *static_cast<const Ppu*>(ctx);
					const uint8 temp = (ppu.m_unstable.status & 0xE0) | (ppu.m_unstable.openBus & 0x1F);
					ppu.m_unstable.status.VBlank().Set(false);
					ppu.m_unstable.writeToggle = false;
					return temp;
				}
			};
		case 0x2003:
			return MappedRead::Unsupported(MappingType::Cpu, isMirror ? mirrorDesc : unsupportedDesc);
		case 0x2004:
			return MappedRead{
				.desc = isMirror ? mirrorDesc : U"OAM Data Port"_sv,
				.ctx = &ppu,
				.func = [](const void* ctx, addr16)
				{
					auto& ppu = *static_cast<const Ppu*>(ctx);
					return reinterpret_cast<const uint8*>(ppu.m_oam.data())[ppu.m_regs.OamAddr];
				}
			};
		case 0x2005:
			return MappedRead::Unsupported(MappingType::Cpu, isMirror ? mirrorDesc : unsupportedDesc);
		case 0x2006:
			return MappedRead::Unsupported(MappingType::Cpu, isMirror ? mirrorDesc : unsupportedDesc);
		case 0x2007:
			return MappedRead{
				.desc = isMirror ? mirrorDesc : U"PPU Data Port"_sv,
				.ctx = &hw,
				.func = [](const void* ctx, addr16)
				{
					// FIXME: 要検証
					auto& hw = *static_cast<const Hardware*>(ctx);
					auto& ppu = hw.GetPpu();

					uint8 data = hw.GetMmu().ReadChr8(ppu.m_unstable.vramAddr);

					if (ppu.m_unstable.vramAddr < 0x3F00)
					{
						std::swap(data, ppu.m_unstable.openBus);
					}

					const uint8 inc = PpuControl8(ppu.m_regs.control).VramIncrementMode() ? 32 : 1;
					ppu.m_unstable.vramAddr = ppu.m_unstable.vramAddr + inc;

					return data;
				}
			};
		default: break;
		}

		return MappedRead::Invalid(MappingType::Cpu);
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
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& ppu = *static_cast<Ppu*>(ctx);
					ppu.m_regs.control = value;
					ppu.m_regs.tempAddr.NameTableAddr().Set(ppu.m_regs.control.BaseNameTableAddr());
				}
			};
		case 0x2001:
			return MappedWrite{
				.desc = isMirror ? mirrorDesc : U"PPU Mask Register"_sv,
				.ctx = &ppu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& ppu = *static_cast<Ppu*>(ctx);
					ppu.m_regs.mask = value;
				}
			};
		case 0x2002:
			return MappedWrite::Unsupported(MappingType::Cpu, isMirror ? mirrorDesc : unsupportedDesc);
		case 0x2003:
			return MappedWrite{
				.desc = isMirror ? mirrorDesc : U"OAM Address Port"_sv,
				.ctx = &ppu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& ppu = *static_cast<Ppu*>(ctx);
					ppu.m_regs.OamAddr = value;
				}
			};
		case 0x2004:
			return MappedWrite{
				.desc = isMirror ? mirrorDesc : U"OAM Data Port"_sv,
				.ctx = &ppu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& ppu = *static_cast<Ppu*>(ctx);
					reinterpret_cast<uint8*>(ppu.m_oam.data())[ppu.m_regs.OamAddr] = value;
				}
			};
		case 0x2005:
			return MappedWrite{
				.desc = isMirror ? mirrorDesc : U"PPU Scrolling Position"_sv,
				.ctx = &ppu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& ppu = *static_cast<Ppu*>(ctx);
					if (not ppu.m_unstable.writeToggle)
					{
						// 1回目の書き込み
						ppu.m_regs.fineX = GetBits<0, 2>(value);
						ppu.m_regs.tempAddr.CoarseX().Set(GetBits<3, 7>(value));
						ppu.m_unstable.writeToggle = true;
					}
					else
					{
						// 2回目の書き込み
						ppu.m_regs.tempAddr.FineY().Set(GetBits<0, 2>(value));
						ppu.m_regs.tempAddr.CoarseY().Set(GetBits<3, 7>(value));
						ppu.m_unstable.writeToggle = false;
					}
				}
			};
		case 0x2006:
			return MappedWrite{
				.desc = isMirror ? mirrorDesc : U"PPU Address Register"_sv,
				.ctx = &ppu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& ppu = *static_cast<Ppu*>(ctx);
					if (not ppu.m_unstable.writeToggle)
					{
						// 1回目の書き込み
						ppu.m_regs.tempAddr = SetBits<8, 15, uint16>(ppu.m_regs.tempAddr, value & 0x3F);
						ppu.m_unstable.writeToggle = true;
					}
					else
					{
						// 2回目の書き込み
						ppu.m_regs.tempAddr = SetBits<0, 7, uint16>(ppu.m_regs.tempAddr, value);
						ppu.m_unstable.vramAddr = ppu.m_regs.tempAddr;
						ppu.m_unstable.writeToggle = false;
					}
				}
			};
		case 0x2007:
			return MappedWrite{
				.desc = isMirror ? mirrorDesc : U"PPU Data Port"_sv,
				.ctx = &hw,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& hw = *static_cast<Hardware*>(ctx);
					hw.GetMmu().WriteChr8(hw.GetPpu().m_unstable.vramAddr, value);
					const auto inc = hw.GetPpu().m_regs.control.VramIncrementMode() ? 32 : 1;
					hw.GetPpu().m_unstable.vramAddr = hw.GetPpu().m_unstable.vramAddr + inc;
				}
			};
		default:
			break;
		}

		return MappedWrite::Invalid(MappingType::Cpu);
	}
}
