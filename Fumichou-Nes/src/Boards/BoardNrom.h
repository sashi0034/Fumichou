#pragma once
#include "BoardBase.h"

namespace Nes
{
	class BoardNrom;
	std::unique_ptr<BoardBase> CreateBoardNrom(const RomData& rom);
}
