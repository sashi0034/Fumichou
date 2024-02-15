#include "stdafx.h"
#include "Ppu_In_Mm.h"

#include "Hardware.h"

// https://www.nesdev.org/wiki/PPU_memory_map

namespace
{
	using namespace Nes;
}

namespace Nes
{
	MappedRead Ppu::In::Mm::MapReadChr(const Hardware& hw, addr16 addr)
	{
		return MappedRead::Invalid(MappingType::Ppu);
	}

	MappedWrite Ppu::In::Mm::MapWriteChr(Hardware& hw, addr16 addr)
	{
		auto& ppu = hw.GetPpu();

		if (AddrRange<addr16>(0x3F00, 0x3FFF).IsBetween(addr))
		{
			const bool isMirror = addr >= 0x3F20;
			return MappedWrite{
				.desc = isMirror ? U"Mirrors of Palette RAM"_sv : U"Palette RAM",
				.ctx = &ppu,
				.func = [](void* ctx, addr16 addr, uint8 value)
				{
					auto& ppu = *static_cast<Ppu*>(ctx);
					addr &= 0x001F;
					if (addr == 0x0010 || addr == 0x0014 || addr == 0x0018 || addr == 0x001C)
					{
						addr -= 0x0010;
					}
					ppu.m_palettes[addr] = value;
				}
			};
		}

		return MappedWrite::Invalid(MappingType::Ppu);
	}
}
