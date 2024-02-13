#pragma once

#include "Forward.h"

namespace Nes
{
	class Ppu
	{
	public:
		class In;

	private:
		PpuCycle m_lineCycles{}; // [0, 341)
		uint32 m_scanLine{};
	};
}
