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

	class Mmu
	{
	public:
		class In;

		using read_array = std::array<MappedRead, AddrSize_0x10000>;
		using write_array = std::array<MappedWrite, AddrSize_0x10000>;

		Mmu();

		const read_array& GetCpuRead() const { return m_cpuRead; }
		const write_array& GetCpuWrite() const { return m_cpuWrite; }
		const read_array& GetPpuRead() const { return m_ppuRead; }
		const write_array& GetPpuWrite() const { return m_ppuWrite; }

	private:
		read_array m_cpuRead{};
		write_array m_cpuWrite{};

		read_array m_ppuRead{};
		write_array m_ppuWrite{};
	};
}
