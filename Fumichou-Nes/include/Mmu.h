#pragma once

#include "Forward.h"

namespace Nes
{
	struct MappedRead
	{
		using func_type = uint8(*)(const Hardware& hw, addr16 addr);

		std::string_view desc;
		func_type func;
	};

	struct MappedWrite
	{
		using func_type = void (*)(Hardware& hw, addr16 addr, uint8 value);

		std::string_view desc;
		func_type func;
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
