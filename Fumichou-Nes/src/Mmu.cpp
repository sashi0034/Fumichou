#include "stdafx.h"
#include "Mmu.h"

#include "Logger.h"

namespace Nes
{
	Mmu::Mmu()
	{
		m_readMap.fill([](const Hardware& nw, addr16 addr)
		{
			Logger::Abort();
			return uint8();
		});

		m_writeMap.fill([](Hardware& nw, addr16 addr, uint8 value)
		{
			Logger::Abort();
		});
	}
}
