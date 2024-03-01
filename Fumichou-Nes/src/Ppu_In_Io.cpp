#include "stdafx.h"
#include "Ppu_In_Io.h"

#include "Hardware.h"
#include "Logger.h"
#include "Mos6502_In.h"

using namespace Nes;

namespace
{
	constexpr StringView mirrorDesc = U"Mirrors of PPU Register";
	constexpr StringView unsupportedDesc = U"Unsupported PPU Address";
}

namespace Nes
{
	MappedRead Ppu::In::Io::MapReadPrg(const Hardware& hw, addr16 addr)
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
					return ppu.m_oam.bytes[ppu.m_regs.OamAddr];
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

					if ((ppu.m_unstable.vramAddr & 0x3FFF) < 0x3F00)
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

	MappedWrite Ppu::In::Io::MapWritePrg(Hardware& hw, addr16 addr)
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
					ppu.m_regs.tempAddr.NameTableAddr().Set(ppu.m_regs.control.BaseNameTableAddrXY());
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
					ppu.m_oam.bytes[ppu.m_regs.OamAddr] = value;
					ppu.m_regs.OamAddr++; // FIXME: 違うかも
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
						ppu.m_scrollX = value;
						ppu.m_regs.fineX = GetBits<0, 2>(value);
						ppu.m_regs.tempAddr.CoarseX().Set(GetBits<3, 7>(value));
						ppu.m_unstable.writeToggle = true;
					}
					else
					{
						// 2回目の書き込み
						ppu.m_scrollY = value;
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

	MappedWrite Ppu::In::Io::MapWritePrg_0x4014(Hardware& hw)
	{
		// https://www.nesdev.org/wiki/DMA
		return MappedWrite{
			.desc = U"OAM DMA"_sv,
			.ctx = &hw,
			.func = [](void* ctx, addr16, uint8 value)
			{
				auto& hw = *static_cast<Hardware*>(ctx);

				Mos6502::In::StartDmaCycles(hw.GetMos6502());

				const addr16 high = value << 8;
				const uint16 oamAddr = hw.GetPpu().m_regs.OamAddr;
				for (uint16 low = 0; low < 256; ++low)
				{
					hw.GetPpu().m_oam.bytes[(oamAddr + low) & 0xFF] = hw.GetMmu().ReadPrg8(high | low);
				}
			}
		};
	}
}
