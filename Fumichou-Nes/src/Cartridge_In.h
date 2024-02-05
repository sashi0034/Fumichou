#pragma once
#include "Cartridge.h"

namespace Nes
{
	class Cartridge::In
	{
	public:
		static bool LoadRomFile(Cartridge& self, FilePathView romPath);

	private:
		struct Impl;
	};
}
