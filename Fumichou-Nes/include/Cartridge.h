#pragma once
#include "RomData.h"

namespace Nes
{
	class Cartridge
	{
	public:
		class In;

	private:
		RomData m_romData{};
	};
}
