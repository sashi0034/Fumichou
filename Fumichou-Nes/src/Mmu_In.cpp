#include "stdafx.h"
#include "Mmu_In.h"

#include "Hardware.h"

struct Nes::Mmu::In::Impl
{
	static void MapWholeAddr(Hardware& hw)
	{
		mapCpuRead(hw, hw.GetMmu().m_cpuRead);
		mapCpuWrite(hw, hw.GetMmu().m_cpuWrite);
	}

private:
	static void mapCpuRead(Hardware& hw, MappedReaderArray& cpuRead)
	{
		auto&& ram = hw.GetRam();

		for (const auto addr : Range(0, 0x7FF))
		{
			cpuRead[addr] = {
				.desc = U"Internal RAM",
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
				.desc = U"Mirrors of internal RAM",
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
				.desc = U"Extra RAM on cartridge",
				.ctx = &ram,
				.func = [](const void* ctx, addr16 addr)
				{
					auto&& ram = *static_cast<const Ram*>(ctx);
					return ram.GetExternalRam()[addr - 0x6000];
				},
			};
		}

		for (const auto addr : Range(0x8000, 0xFFFF))
		{
			cpuRead[addr] = hw.GetCartridge().GetBoard().MapReadPrg(addr);
		}
	}

	static void mapCpuWrite(Hardware& hw, MappedWriterArray& cpuWrite)
	{
		auto&& ram = hw.GetRam();

		for (const auto addr : Range(0, 0x7FF))
		{
			cpuWrite[addr] = {
				.desc = U"Internal RAM",
				.ctx = &ram,
				.func = [](void* ctx, addr16 addr, uint8 value)
				{
					auto&& ram = *static_cast<Ram*>(ctx);
					ram.GetInternalRam()[addr] = value;
				},
			};
		}

		for (const auto addr : Range(0x800, 0x1FFF))
		{
			cpuWrite[addr] = {
				.desc = U"Mirrors of internal RAM",
				.ctx = &ram,
				.func = [](void* ctx, addr16 addr, uint8 value)
				{
					auto&& ram = *static_cast<Ram*>(ctx);
					ram.GetInternalRam()[addr & 0x7FF] = value;
				},
			};
		}

		for (const auto addr : Range(0x6000, 0x7FFF))
		{
			// FIXME: 外部RAMを持っていないカートリッジもある
			cpuWrite[addr] = {
				.desc = U"Extra RAM on cartridge",
				.ctx = &ram,
				.func = [](void* ctx, addr16 addr, uint8 value)
				{
					auto&& ram = *static_cast<Ram*>(ctx);
					ram.GetExternalRam()[addr - 0x6000] = value;
				},
			};
		}

		for (const auto addr : Range(0x8000, 0xFFFF))
		{
			cpuWrite[addr] = hw.GetCartridge().GetBoard().MapWritePrg(addr);
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
