#pragma once

#include "Forward.h"

namespace Nes
{
	enum class MappingType
	{
		Cpu,
		Ppu,
	};

	struct MappedRead
	{
		using func_type = uint8(*)(const void* ctx, addr16 addr);

		StringView desc;
		const void* ctx;
		func_type func;

		static MappedRead Unmapped(MappingType mapping);
		static MappedRead Invalid(MappingType mapping);
		static MappedRead Unsupported(MappingType mapping, StringView desc);
	};

	struct MappedWrite
	{
		using func_type = void (*)(void* ctx, addr16 addr, uint8 value);

		StringView desc;
		void* ctx;
		func_type func;

		static MappedWrite Unmapped(MappingType mapping);
		static MappedWrite Invalid(MappingType mapping);
		static MappedWrite Unsupported(MappingType mapping, StringView desc);
	};

	using MappedReaderArray = std::array<MappedRead, AddrSize_0x10000>;
	using MappedWriterArray = std::array<MappedWrite, AddrSize_0x10000>;

	class Mmu
	{
	public:
		class In;

		Mmu();

		const MappedReaderArray& GetCpuRead() const { return m_cpuRead; }
		const MappedWriterArray& GetCpuWrite() const { return m_cpuWrite; }
		const MappedReaderArray& GetPpuRead() const { return m_ppuRead; }
		const MappedWriterArray& GetPpuWrite() const { return m_ppuWrite; }

		[[nodiscard]] uint8 ReadPrg8(addr16 addr) const;
		[[nodiscard]] uint16 ReadPrg16(addr16 addr) const;

		void WritePrg8(addr16 addr, uint8 value) const;

		[[nodiscard]] uint8 ReadChr8(addr16 addr) const;

		void WriteChr8(addr16 addr, uint8 value) const;

	private:
		MappedReaderArray m_cpuRead{};
		MappedWriterArray m_cpuWrite{};

		MappedReaderArray m_ppuRead{};
		MappedWriterArray m_ppuWrite{};
	};
}
