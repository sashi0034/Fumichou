﻿#include "stdafx.h"
#include "BoardNrom.h"

#include "BoardBase.h"

#include "Logger.h"
#include "PatternTableMemory.h"

// https://www.nesdev.org/wiki/Nm_rom
// https://github.com/amhndu/SimpleNES/blob/d7b8865edac693f348070674e07a7b173bb6662f/src/MapperNm_rom.cpp

class Nes::BoardNrom final : public BoardBase
{
public:
	using BoardBase::BoardBase;
	~BoardNrom() override = default;

	void Initialize() override
	{
		m_oneBank = m_rom.GetPrg().size() == 0x4000;

		if (hasChrRam()) m_patternTable = PatternTableMemory(0x2000);
		else m_patternTable = PatternTableMemory(m_rom.GetChr());
	}

	MappedRead MapReadPrg(addr16 addr) const override
	{
		if (m_oneBank)
		{
			if (AddrRange(0x8000, 0xBFFF).IsBetween(addr))
			{
				return MappedRead{
					.desc = U"First 16 KiB of ROM",
					.ctx = &m_rom,
					.func = [](const void* ctx, addr16 addr)
					{
						auto& m_rom = *static_cast<const RomData*>(ctx);
						return m_rom.GetPrg()[addr - 0x8000];
					}
				};
			}
			if (AddrRange(0xC000, 0xFFFF).IsBetween(addr))
			{
				return MappedRead{
					.desc = U"Mirrored first 16 KiB of ROM",
					.ctx = &m_rom,
					.func = [](const void* ctx, addr16 addr)
					{
						auto& m_rom = *static_cast<const RomData*>(ctx);
						return m_rom.GetPrg()[addr - 0xC000];
					}
				};
			}
		}
		else
		{
			if (AddrRange(0x8000, 0xFFFF).IsBetween(addr))
			{
				return MappedRead{
					.desc = U"PRG-ROM",
					.ctx = &m_rom,
					.func = [](const void* ctx, addr16 addr)
					{
						auto& m_rom = *static_cast<const RomData*>(ctx);
						return m_rom.GetPrg()[addr - 0x8000];
					}
				};
			}
		}

		Logger::Abort();
		return MappedRead::Invalid(MappingType::Cpu);
	}

	MappedWrite MapWritePrg(addr16 addr) override
	{
		return MappedWrite::Invalid(MappingType::Cpu);
	}

	MappedRead MapReadChr(addr16 addr) const override
	{
		return MappedRead{
			.desc = hasChrRam() ? U"CHR-RAM"_sv : U"CHR-ROM"_sv,
			.ctx = &m_patternTable,
			.func = [](const void* ctx, addr16 addr)
			{
				auto& patternTable = *static_cast<const decltype(m_patternTable)*>(ctx);
				return patternTable.Read8(addr);
			}
		};
	}

	MappedWrite MapWriteChr(addr16 addr) override
	{
		if (hasChrRam())
		{
			return MappedWrite{
				.desc = U"CHR-RAM",
				.ctx = &m_patternTable,
				.func = [](void* ctx, addr16 addr, uint8 value)
				{
					auto& patternTable = *static_cast<decltype(m_patternTable)*>(ctx);
					patternTable.Write8(addr, value);
				}
			};
		}

		return MappedWrite::None();
	}

	[[nodiscard]] std::array<uint16, 8> TilePageOffsets() const override
	{
		return std::array<uint16, 8>{
			0x000, 0x040, 0x080, 0x0C0,
			0x100, 0x140, 0x180, 0x1C0,
		};
	}

	[[nodiscard]] const s3d::Image& PatternTableImage() const override
	{
		return m_patternTable.Image();
	}

	[[nodiscard]] const s3d::Texture& PatternTableTexture() const override
	{
		return m_patternTable.Texture();
	}

private:
	PatternTableMemory m_patternTable{};
	bool m_oneBank{};

	bool hasChrRam() const
	{
		return m_rom.GetChr().size() == 0;
	}
};

std::unique_ptr<Nes::BoardBase> Nes::CreateBoardNrom(const RomData& rom)
{
	return std::make_unique<BoardNrom>(rom);
}
