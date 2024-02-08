#pragma once

#include "Forward.h"

namespace Nes
{
	struct MappedRead
	{
		using func_type = uint8(*)(const void* ctx, addr16 addr);

		std::string_view desc;
		const void* ctx;
		func_type func;

		static MappedRead Unmapped();
		static MappedRead Invalid();
		static MappedRead Unsupported(std::string_view desc);
	};

	struct MappedWrite
	{
		using func_type = void (*)(void* ctx, addr16 addr, uint8 value);

		std::string_view desc;
		void* ctx;
		func_type func;

		static MappedWrite Unmapped();
		static MappedWrite Invalid();
		static MappedWrite Unsupported(std::string_view desc);
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

	private:
		MappedReaderArray m_cpuRead{};
		MappedWriterArray m_cpuWrite{};

		MappedReaderArray m_ppuRead{};
		MappedWriterArray m_ppuWrite{};
	};
}
