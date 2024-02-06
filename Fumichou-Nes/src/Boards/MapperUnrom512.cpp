#include "stdafx.h"
#include "MapperUnrom512.h"

// https://github.com/colinvella/EmuNes/blob/91b5839b7b728a3009417bf1937d9a93e9a5c89f/NesCore/Storage/CartridgeMapUnrom512.cs

class Nes::MapperUnrom512 final : public BoardBase
{
public:
	using BoardBase::BoardBase;

	void Initialize() override
	{
	}

	MappedRead MapReadPrg(addr16 addr) override
	{
		if (AddrRange(0x8000, 0xBFFF).IsBetween(addr))
		{
			return MappedRead{
				.desc = "16 KiB switchable PRG ROM",
				.ctx = this,
				.func = [](const void* ctx, addr16 addr)
				{
					auto& self = *static_cast<const MapperUnrom512*>(ctx);
					return self.m_rom.GetPrg()[addr - 0x8000];
				}
			};
		}

		return {};
	}

	MappedWrite MapWritePrg(addr16 addr) override
	{
		return {};
	}

	MappedRead MapReadChr(addr16 addr) override
	{
		return {};
	}

	MappedWrite MapPrgWriteChr(addr16 addr) override
	{
		return {};
	}

private:
	int m_prgBank{};
};
