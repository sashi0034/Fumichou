#pragma once

#include "Forward.h"

namespace Nes
{
	class Mmu
	{
	public:
		using read_t = uint8(*)(const Hardware& nw, addr16 addr);
		using write_t = void (*)(Hardware& nw, addr16 addr, uint8 value);

		Mmu();

	private:
		std::array<read_t, AddrSize_0x10000> m_readMap{};
		std::array<write_t, AddrSize_0x10000> m_writeMap{};
	};
}
