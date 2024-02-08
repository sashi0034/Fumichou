#pragma once
#include "Forward.h"

namespace Nes
{
	constexpr addr16 ResetVector_0xFFFC = 0xFFFC;

	class Mos6502;
	class Mmu;

	struct Mos6502OpArgs
	{
		std::reference_wrapper<Mos6502> mos6502;
		std::reference_wrapper<const Mmu> mmu;
		std::reference_wrapper<addr16> additionalCycles;
	};

	struct Mos6502AddressingArgs
	{
		std::reference_wrapper<addr16> pc;
		std::reference_wrapper<addr16> additionalCycles;
		std::reference_wrapper<const Hardware> hw;
	};
}
