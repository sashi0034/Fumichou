#pragma once
#include "Forward.h"

namespace Nes
{
	constexpr addr16 ResetVector_0xFFFC = 0xFFFC;

	struct Mos6502AddressingArgs
	{
		std::reference_wrapper<addr16> pc;
		std::reference_wrapper<addr16> consumedCycles;
		std::reference_wrapper<const Hardware> hw;
	};
}
