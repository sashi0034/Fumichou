#include "stdafx.h"
#include "Mmu.h"

#include "Hardware.h"
#include "Logger.h"

namespace
{
	using namespace Nes;

	enum class MapAbort
	{
		Unmapped,
		Invalid,
		Unsupported,
	};

	template <MappingType mapping>
	StringView getMappingName()
	{
		return mapping == MappingType::Cpu ? U"CPU"_sv : U"PPU"_sv;
	}

	template <MapAbort kind>
	auto getAbortText()
	{
		return []
		{
			if constexpr (kind == MapAbort::Unmapped) return U"unsupported"_sv;
			if constexpr (kind == MapAbort::Invalid) return U"inaccessible"_sv;
			return U"unsupported"_sv;
		}();
	}

	template <MappingType mapping, MapAbort kind>
	uint8 abortRead(const void* ctx, addr16 addr)
	{
		const auto mappingName = getMappingName<mapping>();
		const auto kindText = getAbortText<kind>();
		Logger::Abort(U"${:04X} ({}) is Read-{}"_fmt(addr, mappingName, kindText));
		return uint8();
	}

	template <MappingType mapping, MapAbort kind>
	void abortWrite(void* ctx, addr16 addr, uint8)
	{
		const auto mappingName = getMappingName<mapping>();
		const auto kindText = getAbortText<kind>();
		Logger::Abort(U"${:04X} ({}) is Write-{}"_fmt(addr, mappingName, kindText));
	}
}

namespace Nes
{
	MappedRead MappedRead::Unmapped(MappingType mapping)
	{
		return {
			.desc = U"(Unmapped)",
			.ctx = nullptr,
			.func = mapping == MappingType::Cpu
				        ? abortRead<MappingType::Cpu, MapAbort::Unmapped>
				        : abortRead<MappingType::Ppu, MapAbort::Unmapped>
		};
	}

	MappedRead MappedRead::Invalid(MappingType mapping)
	{
		return {
			.desc = U"Invalid Access",
			.ctx = nullptr,
			.func = mapping == MappingType::Cpu
				        ? abortRead<MappingType::Cpu, MapAbort::Invalid>
				        : abortRead<MappingType::Ppu, MapAbort::Invalid>,
		};
	}

	MappedRead MappedRead::Unsupported(MappingType mapping, StringView desc)
	{
		return {
			.desc = desc,
			.ctx = nullptr,
			.func = mapping == MappingType::Cpu
				        ? abortRead<MappingType::Cpu, MapAbort::Unsupported>
				        : abortRead<MappingType::Ppu, MapAbort::Unsupported>
		};
	}

	MappedRead MappedRead::None()
	{
		return {
			.desc = U"None",
			.ctx = nullptr,
			.func = [](const void* ctx, addr16 addr) { return uint8(); }
		};
	}

	MappedWrite MappedWrite::Unmapped(MappingType mapping)
	{
		return {
			.desc = U"(Unmapped)",
			.ctx = nullptr,
			.func = mapping == MappingType::Cpu
				        ? abortWrite<MappingType::Cpu, MapAbort::Unmapped>
				        : abortWrite<MappingType::Ppu, MapAbort::Unmapped>,
		};
	}

	MappedWrite MappedWrite::Invalid(MappingType mapping)
	{
		return {
			.desc = U"Invalid Access",
			.ctx = nullptr,
			.func = mapping == MappingType::Cpu
				        ? abortWrite<MappingType::Cpu, MapAbort::Invalid>
				        : abortWrite<MappingType::Ppu, MapAbort::Invalid>,
		};
	}

	MappedWrite MappedWrite::Unsupported(MappingType mapping, StringView desc)
	{
		return {
			.desc = desc,
			.ctx = nullptr,
			.func = mapping == MappingType::Cpu
				        ? abortWrite<MappingType::Cpu, MapAbort::Unsupported>
				        : abortWrite<MappingType::Ppu, MapAbort::Unsupported>,
		};
	}

	MappedWrite MappedWrite::None()
	{
		return MappedWrite{
			.desc = U"None",
			.ctx = nullptr,
			.func = [](void* ctx, addr16 addr, uint8 value) { return; }
		};
	}

	Mmu::Mmu()
	{
		m_cpuRead.fill(MappedRead::Unmapped(MappingType::Cpu));
		m_cpuWrite.fill(MappedWrite::Unmapped(MappingType::Cpu));

		m_ppuRead.fill(MappedRead::Unmapped(MappingType::Ppu));
		m_ppuWrite.fill(MappedWrite::Unmapped(MappingType::Ppu));
	}

	uint8 Mmu::ReadPrg8(addr16 addr) const
	{
		const uint8 value = m_cpuRead[addr].func(m_cpuRead[addr].ctx, addr);
		Logger::Trace(TraceMemoryRead{.addr = addr, .read = value});
		return value;
	}

	uint16 Mmu::ReadPrg16(addr16 addr) const
	{
		return ReadPrg8(addr) | (ReadPrg8(addr + 1) << 8);
	}

	void Mmu::WritePrg8(addr16 addr, uint8 value) const
	{
		m_cpuWrite[addr].func(m_cpuWrite[addr].ctx, addr, value);
		Logger::Trace(TraceMemoryWrite{.addr = addr, .wrote = value});
	}

	uint8 Mmu::ReadChr8(addr16 addr) const
	{
		const uint8 value = m_ppuRead[addr].func(m_ppuRead[addr].ctx, addr);
		return value;
	}

	void Mmu::WriteChr8(addr16 addr, uint8 value) const
	{
		m_ppuWrite[addr].func(m_ppuWrite[addr].ctx, addr, value);
	}
}
