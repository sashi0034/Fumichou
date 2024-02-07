#include "stdafx.h"
#include "Mmu_In.h"

#include "Hardware.h"

struct Nes::Mmu::In::Impl
{
	static void MapWholeAddr(Hardware& hw)
	{
		auto&& cpuRead = hw.GetMmu().m_cpuRead;

		mapCpuRead(hw, cpuRead);
	}

private:
	static void mapCpuRead(Hardware& hw, std::array<MappedRead, AddrSize_0x10000>& cpuRead)
	{
		auto&& ram = hw.GetRam();

		for (const auto addr : Range(0, 0x7FF))
		{
			cpuRead[addr] = {
				.desc = "Internal RAM",
				.ctx = &ram,
				.func = [](const void* ctx, addr16 addr)
				{
					auto&& ram = *static_cast<const Ram*>(ctx);
					return ram.GetInternalRam()[addr];
				},
			};
		}

		for (const auto addr : Range(0x800, 0x1FFF))
		{
			cpuRead[addr] = {
				.desc = "Mirrors of internal RAM",
				.ctx = &ram,
				.func = [](const void* ctx, addr16 addr)
				{
					auto&& ram = *static_cast<const Ram*>(ctx);
					return ram.GetInternalRam()[addr & 0x7FF];
				},
			};
		}

		for (const auto addr : Range(0x6000, 0x7FFF))
		{
			// FIXME: 外部RAMを持っていないカートリッジもある
			cpuRead[addr] = {
				.desc = "Extra RAM on cartridge",
				.ctx = &ram,
				.func = [](const void* ctx, addr16 addr)
				{
					auto&& ram = *static_cast<const Ram*>(ctx);
					return ram.GetExternalRam()[addr - 0x6000];
				},
			};
		}
	}
};

namespace Nes
{
	void Mmu::In::MapWholeAddr(Hardware& hw)
	{
		Impl::MapWholeAddr(hw);
	}
}
