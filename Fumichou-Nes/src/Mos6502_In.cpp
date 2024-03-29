﻿#include "stdafx.h"
#include "Mos6502_In.h"

#include "DebugParameter.h"
#include "Hardware.h"
#include "Logger.h"
#include "Mos6502Forward.h"
#include "Mos6502Instruction.h"

namespace
{
	using namespace Nes;

	constexpr addr16 NmiVector_0xFFFA = 0xFFFA;
	constexpr addr16 ResetVector_0xFFFC = 0xFFFC;
	constexpr addr16 IrqVector_0xFFFE = 0xFFFE;
}

class Mos6502::In::Impl
{
public:
	static CpuCycle Step(Hardware& hw)
	{
		auto&& mos6502 = hw.GetMos6502();
		auto&& mmu = hw.GetMmu();

		// デバッグ用
		if (auto&& watcher = DebugParameter::Instance().watchPc[mos6502.m_regs.pc]) watcher();

		if (mos6502.m_pendingInterrupt == InterruptKind::NMI)
		{
			// マスク不可割り込み
			handleInterrupt(mos6502, mmu, InterruptKind::NMI);
			mos6502.m_pendingInterrupt = InterruptKind::None;
			mos6502.m_cycles += 7;
			return 7;
		}

		if (mos6502.m_stalls > 0)
		{
			// DMA転送中
			const CpuCycle cycles = mos6502.m_stalls;
			mos6502.m_stalls = 0;
			return cycles;
		}

		// 命令フェッチ
		const uint8 fetchedOpcode = mmu.ReadPrg8(mos6502.GetRegs().pc);
		auto&& fetchedInstr = GetMos6502Instruction(fetchedOpcode);
		CpuCycle consumedCycles = GetMos6502OperationCycleCount(fetchedOpcode);

		Logger::Trace(TraceCpuOperation{.pc = mos6502.m_regs.pc, .opcode = fetchedOpcode});
		mos6502.m_regs.pc++;

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

		mos6502.m_cycles += consumedCycles;
		return consumedCycles;
	}
};

namespace Nes
{
	void Mos6502::In::Reset(Mos6502& mos6502, const Mmu& mmu)
	{
		mos6502.m_regs = {};
		mos6502.m_regs.pc = mmu.ReadPrg16(ResetVector_0xFFFC);
		mos6502.m_regs.sp = 0xFD;

		mos6502.m_flags = {};
	}

	CpuCycle Mos6502::In::Step(Hardware& hw)
	{
		return Impl::Step(hw);
	}

	void Mos6502::In::IncrementStalls(Mos6502& self, CpuCycle stalls)
	{
		self.m_stalls += stalls;
	}

	void Mos6502::In::InvokeNmi(Mos6502& self)
	{
		self.m_pendingInterrupt = InterruptKind::NMI;
	}

	void Mos6502::In::InvokeIrq(Mos6502& self)
	{
		if (not self.m_flags.i) self.m_pendingInterrupt = InterruptKind::IRQ;
	}

	void Mos6502::In::handleInterrupt(Mos6502& self, const Mmu& mmu, InterruptKind interrupt)
	{
		Logger::Trace(TraceCpuInterrupt{.pc = self.m_regs.pc, .interrupt = interrupt});

		if (auto&& watcher = DebugParameter::Instance().watchInterrupt) watcher();

		CpuStatus8 status{};
		status.Carry().Set(self.m_flags.c);
		status.Zero().Set(self.m_flags.z);
		status.Interrupt().Set(self.m_flags.i);
		status.Decimal().Set(self.m_flags.d);
		status.Break().Set(interrupt == InterruptKind::BRK);
		status.Unused().Set(true);
		status.Overflow().Set(self.m_flags.v);
		status.Negative().Set(self.m_flags.n);

		pushStack16(self, mmu, self.m_regs.pc);
		pushStack8(self, mmu, status);

		self.m_flags.i = true;

		self.m_regs.pc = mmu.ReadPrg16(interrupt == InterruptKind::NMI
			                               ? NmiVector_0xFFFA
			                               : IrqVector_0xFFFE);
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

	uint8 Mos6502::In::popStack8(Mos6502& self, const Mmu& mmu)
	{
		self.m_regs.sp++;
		return mmu.ReadPrg8(0x100 | self.m_regs.sp);
	}

	uint16 Mos6502::In::popStack16(Mos6502& self, const Mmu& mmu)
	{
		const uint16 lower = popStack8(self, mmu);
		const uint16 higher = popStack8(self, mmu);
		return lower | (higher << 8);
	}
}
