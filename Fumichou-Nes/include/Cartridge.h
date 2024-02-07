#pragma once
#include "BoardBase.h"

namespace Nes
{
	class Cartridge
	{
	public:
		class In;

		const RomData& GetRomData() const { return m_mapper->GetRom(); }

	private:
		std::unique_ptr<BoardBase> m_mapper{};
	};
}
