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
		Mmu();

	private:
		std::array<MappedRead, AddrSize_0x10000> m_cpuRead{};
		std::array<MappedWrite, AddrSize_0x10000> m_cpuWrite{};

		std::array<MappedRead, AddrSize_0x10000> m_ppuRead{};
		std::array<MappedWrite, AddrSize_0x10000> m_ppuWrite{};
	};
}
