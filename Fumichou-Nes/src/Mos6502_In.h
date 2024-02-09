#pragma once
#include "Mos6502.h"

namespace Nes
{
	class Mmu;

	class Mos6502::In
	{
	public:
		static void Reset(Hardware& hw);
		static CpuCycle Step(Hardware& hw);

		class Op;

	private:
		static void pushStack8(Mos6502& self, const Mmu& mmu, uint8 value);
		static void pushStack16(Mos6502& self, const Mmu& mmu, uint16 value);

		class Impl;
	};
}
