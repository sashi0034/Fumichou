#pragma once
#include "BoardBase.h"

namespace Nes
{
	class MapperUnrom512;
	std::unique_ptr<BoardBase> CreateBoardUnrom512(const RomData& rom);
}
