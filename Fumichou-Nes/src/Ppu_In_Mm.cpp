#include "stdafx.h"
#include "Ppu_In_Mm.h"

#include "Hardware.h"

// https://www.nesdev.org/wiki/PPU_memory_map

using namespace Nes;

class Ppu::In::Mm::Impl
{
public:
	template <int table>
	static MappedWrite mapWriteNametable(Ppu& ppu, bool isMirror)
	{
		constexpr std::array<StringView, 4> desc{
			U"Nametable #0", U"Nametable #1", U"Nametable #2", U"Nametable #3"
		};
		const static std::array<String, 4> mirrorDesc = [desc]()
		{
			std::array<String, 4> mirror{};
			for (int i = 0; i < mirror.size(); ++i) mirror[i] = U"Mirror of " + desc[i];
			return mirror;
		}();
		return MappedWrite{
			.desc = isMirror ? mirrorDesc[table] : desc[table],
			.ctx = &ppu,
			.func = [](void* ctx, addr16 addr, uint8 value)
			{
				auto& ppu = *static_cast<Ppu*>(ctx);
				addr &= 0x03FF;
				ppu.m_nametableData[addr + ppu.m_nametableOffset[table]] = value;
			}
		};
	}
};

namespace Nes
{
	MappedRead Ppu::In::Mm::MapReadChr(const Hardware& hw, addr16 addr)
	{
		if (AddrRange<addr16>(0x0000, 0x1FFF).IsBetween(addr))
		{
			return hw.GetCartridge().GetBoard().MapReadChr(addr);
		}

		return MappedRead::Invalid(MappingType::Ppu);
	}

	MappedWrite Ppu::In::Mm::MapWriteChr(Hardware& hw, addr16 addr)
	{
		auto& ppu = hw.GetPpu();

		if (AddrRange<addr16>(0x0000, 0x1FFF).IsBetween(addr))
		{
			return hw.GetCartridge().GetBoard().MapWriteChr(addr);
		}

		if (AddrRange<addr16>(0x2000, 0x3EFF).IsBetween(addr))
		{
			const bool isMirror = addr >= 0x3000;
			switch ((addr & 0xFFF) / 0x400)
			{
			case 0: return Impl::mapWriteNametable<0>(ppu, isMirror);
			case 1: return Impl::mapWriteNametable<1>(ppu, isMirror);
			case 2: return Impl::mapWriteNametable<2>(ppu, isMirror);
			case 3: return Impl::mapWriteNametable<3>(ppu, isMirror);
			default: break;
			}
		}

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
					writePalette(ppu, addr, value);
				}
			};
		}

		return MappedWrite::Invalid(MappingType::Ppu);
	}
}
