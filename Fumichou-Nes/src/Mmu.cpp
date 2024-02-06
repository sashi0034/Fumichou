#include "stdafx.h"
#include "Mmu.h"

#include "Hardware.h"
#include "Logger.h"

namespace Nes
{
	MappedRead MappedRead::Invalid()
	{
		return {
			.desc = "Invalid read access",
			.ctx = nullptr,
			.func = [](const void* ctx, addr16 addr)
			{
				Logger::Abort();
				return uint8();
			},
		};
	}

	MappedWrite MappedWrite::Invalid()
	{
		return {
			.desc = "Invalid write access",
			.ctx = nullptr,
			.func = [](void* hw, addr16 addr, uint8 value)
			{
				Logger::Abort();
			},
		};
	}

	Mmu::Mmu()
	{
		m_readMap.fill({
			.desc = "(Unmapped)",
			.ctx = nullptr,
			.func = [](const void* ctx, addr16 addr)
			{
				Logger::Abort();
				return uint8();
			},
		});

		m_writeMap.fill({
			.desc = "(Unmapped)",
			.ctx = nullptr,
			.func = [](void* hw, addr16 addr, uint8 value)
			{
				Logger::Abort();
			},
		});
	}
}
