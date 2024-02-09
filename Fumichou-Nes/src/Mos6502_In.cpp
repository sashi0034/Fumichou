#include "stdafx.h"
#include "Mos6502_In.h"

#include "Hardware.h"
#include "Mos6502Forward.h"
#include "Mos6502Instruction.h"

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

	static CpuCycle Step(Hardware& hw)
	{
		auto&& mos6502 = hw.GetMos6502();
		auto&& mmu = hw.GetMmu();

		// 命令フェッチ
		const uint8 fetchedOpcode = mmu.ReadPrg8(mos6502.GetRegs().pc);
		mos6502.m_regs.pc++;
		auto&& fetchedInstr = GetMos6502Instruction(fetchedOpcode);
		CpuCycle consumedCycles = GetMos6502OperationCycleCount(fetchedOpcode);
		CpuCycle pageBoundary{};

		// アドレッシング
		const addr16 srcAddr = fetchedInstr.addressing(Mos6502AddressingArgs{
			.pc = std::ref(mos6502.m_regs.pc),
			.pageBoundary = std::ref(pageBoundary),
			.hw = hw
		});

		// 命令実行
		fetchedInstr.operation(Mos6502OpArgs{
			.mos6502 = mos6502,
			.mmu = mmu,
			.consumedCycles = std::ref(consumedCycles),
			.srcAddr = srcAddr,
			.pageBoundary = pageBoundary,
		});

		return consumedCycles;
	}
};

namespace Nes
{
	void Mos6502::In::Reset(Hardware& hw)
	{
		Impl::Reset(hw);
	}

	CpuCycle Mos6502::In::Step(Hardware& hw)
	{
		return Impl::Step(hw);
	}

	void Mos6502::In::pushStack8(Mos6502& self, const Mmu& mmu, uint8 value)
	{
		mmu.WritePrg8(0x100 | self.m_regs.sp, value);
		self.m_regs.sp--;
	}

	void Mos6502::In::pushStack16(Mos6502& self, const Mmu& mmu, uint16 value)
	{
		pushStack8(self, mmu, value >> 8);
		pushStack8(self, mmu, value & 0xFF);
	}
}
