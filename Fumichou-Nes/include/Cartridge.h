#pragma once
#include "BoardBase.h"

namespace Nes
{
	class Cartridge
	{
	public:
		class In;

		BoardBase& GetBoard() { return *m_board; }
		const BoardBase& GetBoard() const { return *m_board; }

		const RomData& GetRomData() const { return m_board->GetRom(); }

	private:
		std::unique_ptr<BoardBase> m_board{};
	};
}
