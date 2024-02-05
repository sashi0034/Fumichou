#pragma once
#include "Cartridge.h"

namespace Nes
{
	class Cartridge::In
	{
	public:
		static bool LoadRomFile(Cartridge& cart, FilePathView romPath);
	};
}
