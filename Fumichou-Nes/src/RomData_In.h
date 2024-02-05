#pragma once
#include "RomData.h"

namespace Nes
{
	class RomData::In
	{
	public:
		static bool LoadRomFile(RomData& self, FilePathView romPath);
	};
}
