#include "stdafx.h"
#include "BoardCnrom.h"

#include "Logger.h"
#include "PatternTableMemory.h"

class Nes::BoardCnrom final : public BoardBase
{
public:
	using BoardBase::BoardBase;
	~BoardCnrom() override = default;

	void Initialize() override
	{
		m_oneBank = m_rom.GetPrg().size() == 0x4000;

		m_patternTable = PatternTableMemory(m_rom.GetChr());
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
		return MappedWrite{
			.desc = U"CHR Bank Select",
			.ctx = this,
			.func = [](void* ctx, addr16 addr, uint8 value)
			{
				auto& self = *static_cast<decltype(this)>(ctx);
				self.m_chrBank = value & 0x3;
			}
		};
	}

	MappedRead MapReadChr(addr16 addr) const override
	{
		return MappedRead{
			.desc = U"CHR-ROM"_sv,
			.ctx = this,
			.func = [](const void* ctx, addr16 addr)
			{
				const auto& self = *static_cast<decltype(this)>(ctx);
				const auto& patternTable = self.m_patternTable;
				return patternTable.Read8(addr | (self.m_chrBank << 13));
			}
		};
	}

	MappedWrite MapWriteChr(addr16 addr) override
	{
		return MappedWrite::None();
	}

	[[nodiscard]] std::array<uint16, 8> TilePageOffsets() const override
	{
		auto pages = std::array<uint16, 8>{
			0x000, 0x040, 0x080, 0x0C0,
			0x100, 0x140, 0x180, 0x1C0,
		};
		const uint16 offset = 0x200 * m_chrBank;
		for (int i = 0; i < pages.size(); ++i) pages[i] += offset;
		return pages;
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
	uint8 m_chrBank{}; // TODO: レンダリングのときに CHR-ROM を反映させる
};

namespace Nes
{
	std::unique_ptr<BoardBase> CreateBoardCnrom(const RomData& rom)
	{
		return std::make_unique<BoardCnrom>(rom);
	}
}
