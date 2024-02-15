#pragma once
#include "Ppu.h"

namespace Nes
{
	class Ppu::In
	{
	public:
		class Io;
		static void Step(Hardware& hw, PpuCycle cycle);

	private:
		class Impl;
	};
}
