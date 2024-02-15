#pragma once
#include "Forward.h"

namespace Nes
{
	enum class InterruptKind
	{
		None = 0,
		NMI,
		BRK,
		IRQ,
	};

	StringView StringifyInterrupt(InterruptKind interruptKind);

	// https://www.nesdev.org/wiki/CPU_registers
	struct CpuRegs
	{
		addr16 pc;
		uint8 sp;
		uint8 a;
		uint8 x;
		uint8 y;
	};

	// https://www.nesdev.org/wiki/Status_flags
	struct CpuFlags
	{
		bool c;
		bool z;
		bool i;
		bool d;
		bool v;
		bool n;
	};

	class Mos6502
	{
	public:
		class In;

		const CpuRegs& GetRegs() const { return m_regs; }
		const CpuFlags& GetFlags() const { return m_flags; }

	private:
		InterruptKind m_pendingInterrupt{InterruptKind::None};
		CpuRegs m_regs{};
		CpuFlags m_flags{};
	};
}
