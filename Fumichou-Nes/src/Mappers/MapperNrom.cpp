#include "stdafx.h"
#include "MapperNrom.h"

#include "Logger.h"

namespace Nes
{
	// https://www.nesdev.org/wiki/NROM
	// https://github.com/amhndu/SimpleNES/blob/d7b8865edac693f348070674e07a7b173bb6662f/src/MapperNROM.cpp

	void MapperNrom::Initialize(const RomData& rom)
	{
		m_oneBank = rom.GetPrg().size() == 0x4000;

		if (rom.GetChr().size() == 0) m_chrRam.resize(0x2000);
	}

	MappedRead MapperNrom::MapReadPrg(const RomData& rom, addr16 addr)
	{
		if (m_oneBank)
		{
			if (AddrRange(0x8000, 0xBFFF).IsBetween(addr))
			{
				return MappedRead{
					.desc = "First 16 KiB of ROM",
					.ctx = &rom,
					.func = [](const void* ctx, addr16 addr)
					{
						auto& rom = *static_cast<const RomData*>(ctx);
						return rom.GetPrg()[addr - 0x8000];
					}
				};
			}
			if (AddrRange(0xC000, 0xFFFF).IsBetween(addr))
			{
				return MappedRead{
					.desc = "Mirrored first 16 KiB of ROM",
					.ctx = &rom,
					.func = [](const void* ctx, addr16 addr)
					{
						auto& rom = *static_cast<const RomData*>(ctx);
						return rom.GetPrg()[addr - 0xC000];
					}
				};
			}
		}
		else
		{
			if (AddrRange(0x8000, 0xFFFF).IsBetween(addr))
			{
				return MappedRead{
					.desc = "PRG ROM",
					.ctx = &rom,
					.func = [](const void* ctx, addr16 addr)
					{
						auto& rom = *static_cast<const RomData*>(ctx);
						return rom.GetPrg()[addr - 0x8000];
					}
				};
			}
		}

		Logger::Abort();
		return MappedRead::Invalid();
	}

	MappedWrite MapperNrom::MapWritePrg(RomData& rom, addr16 addr)
	{
		return MappedWrite::Invalid();
	}

	MappedRead MapperNrom::MapReadChr(const RomData& rom, addr16 addr)
	{
		if (m_chrRam.empty())
		{
			return MappedRead{
				.desc = "CHR ROM",
				.ctx = &rom,
				.func = [](const void* ctx, addr16 addr)
				{
					auto& rom = *static_cast<const RomData*>(ctx);
					return rom.GetChr()[addr];
				}
			};
		}
		else
		{
			return MappedRead{
				.desc = "CHR RAM",
				.ctx = this,
				.func = [](const void* ctx, addr16 addr)
				{
					auto& self = *static_cast<const MapperNrom*>(ctx);
					return self.m_chrRam[addr];
				}
			};
		}
	}

	MappedWrite MapperNrom::MapPrgWriteChr(RomData& rom, addr16 addr)
	{
		if (not m_chrRam.empty())
		{
			return MappedWrite{
				.desc = "CHR RAM",
				.ctx = this,
				.func = [](void* ctx, addr16 addr, uint8 value)
				{
					auto& self = *static_cast<MapperNrom*>(ctx);
					self.m_chrRam[addr] = value;
				}
			};
		}
		return MappedWrite::Invalid();
	}
}
