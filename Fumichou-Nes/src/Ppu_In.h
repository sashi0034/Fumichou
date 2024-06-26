﻿#pragma once
#include "Ppu.h"

namespace Nes
{
	class Ppu::In
	{
	public:
		class Io;
		class Mm;
		static void Step(Hardware& hw, PpuCycle cycle);
		static void RenderVideo(Hardware& hw);
		static void UpdateMirroring(Ppu& ppu, NameTableMirror mirror);

	private:
		static uint8 readPalette(const Ppu& ppu, uint8 paletteIndex);
		static void writePalette(Ppu& ppu, uint8 paletteIndex, uint8 value);

		class Impl;
	};
}
