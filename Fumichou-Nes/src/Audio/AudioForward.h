#pragma once

#include "Forward.h"

namespace Nes
{
	inline constexpr std::array<uint8, 32> LengthTable{
		10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
		12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30,
	};
}
