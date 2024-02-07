#include "stdafx.h"
#include "Mos6502_In.h"

#include "Hardware.h"
#include "Mos6502Forward.h"

class Nes::Mos6502::In::Impl
{
public:
	static void Reset(Hardware& hw)
	{
		auto&& mos6502 = hw.GetMos6502();

		mos6502.m_regs = {};
		mos6502.m_regs.pc = hw.GetMmu().ReadPrg16(ResetVector_0xFFFC);
		mos6502.m_regs.sp = 0xFD;

		mos6502.m_flags = {};
	}
};

namespace Nes
{
	void Mos6502::In::Reset(Hardware& hw)
	{
		Impl::Reset(hw);
	}

	CycleCount Mos6502::In::Step(Hardware& hw)
	{
		return {};
	}
}
