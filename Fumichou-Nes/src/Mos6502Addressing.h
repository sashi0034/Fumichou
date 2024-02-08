#pragma once

#include "Mos6502Forward.h"

namespace Nes
{
	class Mmu;

	namespace Mos6502Addressing
	{
		addr16 Immediate(const Mos6502AddressingArgs& args);
		addr16 ZeroPage(const Mos6502AddressingArgs& args);
		addr16 ZeroPageX(const Mos6502AddressingArgs& args);
		addr16 ZeroPageY(const Mos6502AddressingArgs& args);
		addr16 Indirect(const Mos6502AddressingArgs& args); // Only used by JMP
		addr16 IndirectX(const Mos6502AddressingArgs& args);
		addr16 IndirectY(const Mos6502AddressingArgs& args);
		addr16 Accumulator(const Mos6502AddressingArgs& args); // Same as Implied
		addr16 Absolute(const Mos6502AddressingArgs& args);
		addr16 AbsoluteX(const Mos6502AddressingArgs& args);
		addr16 AbsoluteY(const Mos6502AddressingArgs& args);
		addr16 Relative(const Mos6502AddressingArgs& args);
		addr16 Implied(const Mos6502AddressingArgs& args);
	}
}
