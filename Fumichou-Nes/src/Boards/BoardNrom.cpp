#include "stdafx.h"
#include "BoardNrom.h"

#include "BoardBase.h"

#include "Logger.h"

// https://www.nesdev.org/wiki/Nm_rom
// https://github.com/amhndu/SimpleNES/blob/d7b8865edac693f348070674e07a7b173bb6662f/src/MapperNm_rom.cpp

class Nes::BoardNrom final : public BoardBase
{
public:
	using BoardBase::BoardBase;

	void Initialize()
	{
		m_oneBank = m_rom.GetPrg().size() == 0x4000;

		if (m_rom.GetChr().size() == 0) m_chrRam.resize(0x2000);
	}

	MappedRead MapReadPrg(addr16 addr) override
	{
		if (m_oneBank)
		{
			if (AddrRange(0x8000, 0xBFFF).IsBetween(addr))
			{
				return MappedRead{
					.desc = "First 16 KiB of m_rom",
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
					.desc = "Mirrored first 16 KiB of m_rom",
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
					.desc = "PRG-m_rom",
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
		return MappedRead::Invalid();
	}

	MappedWrite MapWritePrg(addr16 addr) override
	{
		return MappedWrite::Invalid();
	}

	MappedRead MapReadChr(addr16 addr) override
	{
		if (m_chrRam.empty())
		{
			return MappedRead{
				.desc = "CHR-m_rom",
				.ctx = &m_rom,
				.func = [](const void* ctx, addr16 addr)
				{
					auto& m_rom = *static_cast<const RomData*>(ctx);
					return m_rom.GetChr()[addr];
				}
			};
		}
		else
		{
			return MappedRead{
				.desc = "CHR-RAM",
				.ctx = this,
				.func = [](const void* ctx, addr16 addr)
				{
					auto& self = *static_cast<const BoardNrom*>(ctx);
					return self.m_chrRam[addr];
				}
			};
		}
	}

	MappedWrite MapPrgWriteChr(addr16 addr) override
	{
		if (not m_chrRam.empty())
		{
			return MappedWrite{
				.desc = "CHR-RAM",
				.ctx = this,
				.func = [](void* ctx, addr16 addr, uint8 value)
				{
					auto& self = *static_cast<BoardNrom*>(ctx);
					self.m_chrRam[addr] = value;
				}
			};
		}

		return MappedWrite::Invalid();
	}

private:
	Array<uint8> m_chrRam{};
	bool m_oneBank{};
};

std::unique_ptr<Nes::BoardBase> Nes::CreateBoardNrom(const RomData& rom)
{
	return std::make_unique<BoardNrom>(rom);
}
