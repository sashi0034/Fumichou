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

		static void RequestNmi(Mos6502& self);

		class Op;

	private:
		static void handleInterrupt(Mos6502& self, const Mmu& mmu, InterruptKind interrupt);

		static void pushStack8(Mos6502& self, const Mmu& mmu, uint8 value);
		static void pushStack16(Mos6502& self, const Mmu& mmu, uint16 value);

		[[nodiscard]] static uint8 popStack8(Mos6502& self, const Mmu& mmu);
		[[nodiscard]] static uint16 popStack16(Mos6502& self, const Mmu& mmu);

		class Impl;
	};
}
