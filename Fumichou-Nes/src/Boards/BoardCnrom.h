#pragma once
#include "BoardBase.h"

namespace Nes
{
	class BoardCnrom;
	std::unique_ptr<BoardBase> CreateBoardCnrom(const RomData& rom);
}
