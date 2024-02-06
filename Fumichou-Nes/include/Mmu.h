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

		static MappedRead Invalid();
	};

	struct MappedWrite
	{
		using func_type = void (*)(void* ctx, addr16 addr, uint8 value);

		std::string_view desc;
		void* ctx;
		func_type func;

		static MappedWrite Invalid();
	};

	class Mmu
	{
	public:
		Mmu();

	private:
		std::array<MappedRead, AddrSize_0x10000> m_readMap{};
		std::array<MappedWrite, AddrSize_0x10000> m_writeMap{};
	};
}
