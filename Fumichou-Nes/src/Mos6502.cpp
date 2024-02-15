#include "stdafx.h"
#include "Mos6502.h"

namespace Nes
{
	StringView StringifyInterrupt(InterruptKind interruptKind)
	{
		switch (interruptKind)
		{
		case InterruptKind::None:
			return U"None";
		case InterruptKind::NMI:
			return U"NMI";
		case InterruptKind::BRK:
			return U"BRK";
		case InterruptKind::IRQ:
			return U"IRQ";
		default: ;
			assert(false);
			return U"";
		}
	}
}
