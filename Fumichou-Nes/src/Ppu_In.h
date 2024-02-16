#pragma once
#include "Ppu.h"

namespace Nes
{
	class Ppu::In
	{
	public:
		class Io;
		class Mm;
		static void Step(Hardware& hw, PpuCycle cycle);
		static void UpdateMirroring(Ppu& ppu, NameTableMirror mirror);

	private:
		class Renderer;
		class Impl;
	};
}
