#include "stdafx.h"
#include "Mmu.h"

#include "Hardware.h"
#include "Logger.h"

namespace Nes
{
	MappedRead MappedRead::Unmapped()
	{
		return {
			.desc = "(Unmapped)",
			.ctx = nullptr,
			.func = [](const void* ctx, addr16 addr)
			{
				Logger::Abort();
				return uint8();
			},
		};
	}

	MappedRead MappedRead::Invalid()
	{
		return {
			.desc = "Invalid access",
			.ctx = nullptr,
			.func = [](const void* ctx, addr16 addr)
			{
				Logger::Abort();
				return uint8();
			},
		};
	}

	MappedRead MappedRead::Unsupported(std::string_view desc)
	{
		return {
			.desc = desc,
			.ctx = nullptr,
			.func = [](const void* ctx, addr16 addr)
			{
				Logger::Abort();
				return uint8();
			},
		};
	}

	MappedWrite MappedWrite::Unmapped()
	{
		return {
			.desc = "(Unmapped)",
			.ctx = nullptr,
			.func = [](void* ctx, addr16 addr, uint8 value)
			{
				Logger::Abort();
			},
		};
	}

	MappedWrite MappedWrite::Invalid()
	{
		return {
			.desc = "Invalid access",
			.ctx = nullptr,
			.func = [](void* ctx, addr16 addr, uint8 value)
			{
				Logger::Abort();
			},
		};
	}

	MappedWrite MappedWrite::Unsupported(std::string_view desc)
	{
		return {
			.desc = desc,
			.ctx = nullptr,
			.func = [](void* ctx, addr16 addr, uint8 value)
			{
				Logger::Abort();
			},
		};
	}

	Mmu::Mmu()
	{
		m_cpuRead.fill(MappedRead::Unmapped());
		m_cpuWrite.fill(MappedWrite::Unmapped());

		m_ppuRead.fill(MappedRead::Unmapped());
		m_ppuWrite.fill(MappedWrite::Unmapped());
	}

	uint8 Mmu::ReadPrg8(addr16 addr) const
	{
		return m_cpuRead[addr].func(m_cpuRead[addr].ctx, addr);
	}

	uint16 Mmu::ReadPrg16(addr16 addr) const
	{
		return ReadPrg8(addr) | (ReadPrg8(addr + 1) << 8);
	}
}
