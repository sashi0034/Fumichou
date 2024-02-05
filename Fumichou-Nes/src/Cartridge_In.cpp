#include "stdafx.h"
#include "Cartridge_In.h"

#include "RomData_In.h"

namespace Nes
{
	bool Cartridge::In::LoadRomFile(Cartridge& cart, FilePathView romPath)
	{
		if (not RomData::In::LoadRomFile(cart.m_romData, romPath)) return false;
		return true;
	}
}
