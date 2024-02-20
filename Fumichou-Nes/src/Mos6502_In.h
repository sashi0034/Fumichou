#pragma once
#include "Mos6502.h"

namespace Nes
{
	class Mmu;

	class CpuStatus8
	{
	public:
		CpuStatus8(uint8 value = 0) : m_value(value) { return; }
		operator uint8() const { return m_value; }

		auto Carry() { return BitAccess<0>(m_value); }
		auto Zero() { return BitAccess<1>(m_value); }
		auto Interrupt() { return BitAccess<2>(m_value); }
		auto Decimal() { return BitAccess<3>(m_value); }
		auto Break() { return BitAccess<4>(m_value); }
		auto Unused() { return BitAccess<5>(m_value); }
		auto Overflow() { return BitAccess<6>(m_value); }
		auto Negative() { return BitAccess<7>(m_value); }

	private:
		uint8 m_value{};
	};

	class Mos6502::In
	{
	public:
		static void Reset(Hardware& hw);
		static CpuCycle Step(Hardware& hw);

		static void RequestNmi(Mos6502& self);
		static void StartDmaCycles(Mos6502& self);

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
