﻿#pragma once
#include "Logger.h"
#include "Mmu.h"
#include "Mos6502Forward.h"
#include "Mos6502_In.h"

// 命令資料
// https://www.nesdev.org/obelisk-6502-guide/reference.html

// サイクル数資料
// https://www.nesdev.org/wiki/6502_cycle_times

class Nes::Mos6502::In::Op
{
public:
	static void ADC(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		const uint16 m = args.mmu.get().ReadPrg8(args.srcAddr);
		const uint16 a = mos6502.m_regs.a + m + mos6502.m_flags.c;
		mos6502.m_flags.c = a & 0x100;
		mos6502.m_flags.v = (mos6502.m_regs.a ^ a) & (m ^ a) & 0x80; // 符号変化が 正+正->負 か 負+負->正 になったらオーバーフロー
		setZN(mos6502, a);
		mos6502.m_regs.a = a;
		args.consumedCycles += args.pageBoundary;
	}

	static void AND(const Mos6502OpArgs& args)
	{
		auto& mos6502 = args.mos6502.get();
		mos6502.m_regs.a &= args.mmu.get().ReadPrg8(args.srcAddr);
		setZN(mos6502, mos6502.m_regs.a);
	}

	template <bool acc>
	static void ASL(const Mos6502OpArgs& args)
	{
		shiftLeft<acc, false>(args);
	}

	static void BIT(const Mos6502OpArgs& args)
	{
		auto& mos6502 = args.mos6502.get();
		const uint8 data = args.mmu.get().ReadPrg8(args.srcAddr);
		mos6502.m_flags.z = not(mos6502.m_regs.a & data);
		mos6502.m_flags.v = data & 0x40;
		mos6502.m_flags.n = data & 0x80;
	}

	static void BRK(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.pc++;
		handleInterrupt(args.mos6502, args.mmu, InterruptKind::BRK);
	}

	static void BCC(const Mos6502OpArgs& args)
	{
		if (not args.mos6502.get().m_flags.c) branchConditional(args);
	}

	static void BCS(const Mos6502OpArgs& args)
	{
		if (args.mos6502.get().m_flags.c) branchConditional(args);
	}

	static void BEQ(const Mos6502OpArgs& args)
	{
		if (args.mos6502.get().m_flags.z) branchConditional(args);
	}

	static void BMI(const Mos6502OpArgs& args)
	{
		if (args.mos6502.get().m_flags.n) branchConditional(args);
	}

	static void BNE(const Mos6502OpArgs& args)
	{
		if (not args.mos6502.get().m_flags.z) branchConditional(args);
	}

	static void BPL(const Mos6502OpArgs& args)
	{
		if (not args.mos6502.get().m_flags.n) branchConditional(args);
	}

	static void BVC(const Mos6502OpArgs& args)
	{
		if (not args.mos6502.get().m_flags.v) branchConditional(args);
	}

	static void BVS(const Mos6502OpArgs& args)
	{
		if (args.mos6502.get().m_flags.v) branchConditional(args);
	}

	static void CLC(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_flags.c = false;
	}

	static void CLD(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_flags.d = false;
	}

	static void CLI(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_flags.i = false;
	}

	static void CLV(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_flags.v = false;
	}

	static void CMP(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		const uint16 compare = mos6502.m_regs.a - args.mmu.get().ReadPrg8(args.srcAddr);
		mos6502.m_flags.c = not(compare & 0x100);
		setZN(mos6502, compare);
		args.consumedCycles += args.pageBoundary;
	}

	static void CPX(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		const uint16 compare = mos6502.m_regs.x - args.mmu.get().ReadPrg8(args.srcAddr);
		mos6502.m_flags.c = not(compare & 0x100);
		setZN(mos6502, compare);
	}

	static void CPY(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		const uint16 compare = mos6502.m_regs.y - args.mmu.get().ReadPrg8(args.srcAddr);
		mos6502.m_flags.c = not(compare & 0x100);
		setZN(mos6502, compare);
	}

	static void DEC(const Mos6502OpArgs& args)
	{
		const uint8 data = args.mmu.get().ReadPrg8(args.srcAddr) - 1;
		setZN(args.mos6502, data);
		args.mmu.get().WritePrg8(args.srcAddr, data);
	}

	static void DEX(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.x--;
		setZN(args.mos6502, args.mos6502.get().m_regs.x);
	}

	static void DEY(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.y--;
		setZN(args.mos6502, args.mos6502.get().m_regs.y);
	}

	static void INC(const Mos6502OpArgs& args)
	{
		const uint8 data = args.mmu.get().ReadPrg8(args.srcAddr) + 1;
		setZN(args.mos6502, data);
		args.mmu.get().WritePrg8(args.srcAddr, data);
	}

	static void INX(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.x++;
		setZN(args.mos6502, args.mos6502.get().m_regs.x);
	}

	static void INY(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.y++;
		setZN(args.mos6502, args.mos6502.get().m_regs.y);
	}

	static void JMP(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.pc = args.srcAddr;
	}

	static void JSR(const Mos6502OpArgs& args)
	{
		pushStack16(args.mos6502, args.mmu, args.mos6502.get().m_regs.pc - 1);
		args.mos6502.get().m_regs.pc = args.srcAddr;
	}

	static void LDA(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.a = args.mmu.get().ReadPrg8(args.srcAddr);
		setZN(args.mos6502, args.mos6502.get().m_regs.a);
		args.consumedCycles += args.pageBoundary;
	}

	static void LDX(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.x = args.mmu.get().ReadPrg8(args.srcAddr);
		setZN(args.mos6502, args.mos6502.get().m_regs.x);
		args.consumedCycles += args.pageBoundary;
	}

	static void LDY(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.y = args.mmu.get().ReadPrg8(args.srcAddr);
		setZN(args.mos6502, args.mos6502.get().m_regs.y);
		args.consumedCycles += args.pageBoundary;
	}

	template <bool acc>
	static void LSR(const Mos6502OpArgs& args)
	{
		shiftRight<acc, false>(args);
	}

	static void NOP(const Mos6502OpArgs& args)
	{
	}

	static void EOR(const Mos6502OpArgs& args)
	{
		auto& mos6502 = args.mos6502.get();
		mos6502.m_regs.a ^= args.mmu.get().ReadPrg8(args.srcAddr);
		setZN(mos6502, mos6502.m_regs.a);
	}

	static void ORA(const Mos6502OpArgs& args)
	{
		auto& mos6502 = args.mos6502.get();
		mos6502.m_regs.a |= args.mmu.get().ReadPrg8(args.srcAddr);
		setZN(mos6502, mos6502.m_regs.a);
	}

	static void PHA(const Mos6502OpArgs& args)
	{
		pushStack8(args.mos6502, args.mmu, args.mos6502.get().GetRegs().a);
	}

	static void PHP(const Mos6502OpArgs& args)
	{
		const auto& flags = args.mos6502.get().m_flags;
		CpuStatus8 status{};
		status.Negative().Set(flags.n);
		status.Overflow().Set(flags.v);
		status.Break().Set(true);
		status.Unused().Set(true);
		status.Decimal().Set(flags.d);
		status.Interrupt().Set(flags.i);
		status.Zero().Set(flags.z);
		status.Carry().Set(flags.c);
		pushStack8(args.mos6502, args.mmu, status);
	}

	static void PLA(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.a = popStack8(args.mos6502, args.mmu);
		setZN(args.mos6502, args.mos6502.get().m_regs.a);
	}

	static void PLP(const Mos6502OpArgs& args)
	{
		CpuStatus8 status = popStack8(args.mos6502, args.mmu);
		auto& flags = args.mos6502.get().m_flags;
		flags.n = status.Negative();
		flags.v = status.Overflow();
		flags.d = status.Decimal();
		flags.i = status.Interrupt();
		flags.z = status.Zero();
		flags.c = status.Carry();
	}

	template <bool acc>
	static void ROL(const Mos6502OpArgs& args)
	{
		shiftLeft<acc, true>(args);
	}

	template <bool acc>
	static void ROR(const Mos6502OpArgs& args)
	{
		shiftRight<acc, true>(args);
	}

	// 割り込みから戻る
	static void RTI(const Mos6502OpArgs& args)
	{
		auto& mos6502 = args.mos6502.get();
		CpuStatus8 status = popStack8(mos6502, args.mmu);
		mos6502.m_regs.pc = popStack16(mos6502, args.mmu);

		mos6502.m_flags.n = status.Negative();
		mos6502.m_flags.v = status.Overflow();
		mos6502.m_flags.d = status.Decimal();
		mos6502.m_flags.i = status.Interrupt();
		mos6502.m_flags.z = status.Zero();
		mos6502.m_flags.c = status.Carry();
	}

	static void RTS(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_regs.pc = popStack16(args.mos6502, args.mmu);
		args.mos6502.get().m_regs.pc++;
	}

	static void SBC(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		const uint16 m = args.mmu.get().ReadPrg8(args.srcAddr);
		const uint16 a = mos6502.m_regs.a - m - (not mos6502.m_flags.c);
		mos6502.m_flags.c = not(a & 0x100);
		mos6502.m_flags.v = (mos6502.m_regs.a ^ a) & (~m ^ a) & 0x80; // 符号変化が 正+(-負)->負 か 負+(-正)->正 になったらオーバーフロー
		setZN(mos6502, a);
		mos6502.m_regs.a = a;
		args.consumedCycles += args.pageBoundary;
	}

	static void SEC(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_flags.c = true;
	}

	static void SED(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_flags.d = true;
	}

	static void SEI(const Mos6502OpArgs& args)
	{
		args.mos6502.get().m_flags.i = true;
	}

	static void STA(const Mos6502OpArgs& args)
	{
		args.mmu.get().WritePrg8(args.srcAddr, args.mos6502.get().GetRegs().a);
	}

	static void STX(const Mos6502OpArgs& args)
	{
		args.mmu.get().WritePrg8(args.srcAddr, args.mos6502.get().GetRegs().x);
	}

	static void STY(const Mos6502OpArgs& args)
	{
		args.mmu.get().WritePrg8(args.srcAddr, args.mos6502.get().GetRegs().y);
	}

	static void TAX(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		mos6502.m_regs.x = mos6502.m_regs.a;
		setZN(mos6502, mos6502.m_regs.x);
	}

	static void TAY(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		mos6502.m_regs.y = mos6502.m_regs.a;
		setZN(mos6502, mos6502.m_regs.y);
	}

	static void TSX(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		mos6502.m_regs.x = mos6502.m_regs.sp;
		setZN(mos6502, mos6502.m_regs.x);
	}

	static void TXA(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		mos6502.m_regs.a = mos6502.m_regs.x;
		setZN(mos6502, mos6502.m_regs.a);
	}

	static void TXS(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		mos6502.m_regs.sp = mos6502.m_regs.x;
	}

	static void TYA(const Mos6502OpArgs& args)
	{
		auto&& mos6502 = args.mos6502.get();
		mos6502.m_regs.a = mos6502.m_regs.y;
		setZN(mos6502, mos6502.m_regs.a);
	}

private:
	static void setZN(Mos6502& mos6502, uint8 value)
	{
		mos6502.m_flags.z = value == 0;
		mos6502.m_flags.n = value & 0x80;
	}

	static void branchConditional(const Mos6502OpArgs& args)
	{
		args.consumedCycles += 1 + args.pageBoundary; // 分岐すると {1, 2} サイクル遅れる
		args.mos6502.get().m_regs.pc = args.srcAddr;
	}

	template <bool acc, bool rotate>
	static void shiftLeft(const Mos6502OpArgs& args)
	{
		auto& mos6502 = args.mos6502.get();
		auto& mmu = args.mmu.get();

		uint8 data;
		if constexpr (acc) data = mos6502.m_regs.a;
		else data = mmu.ReadPrg8(args.srcAddr);

		const bool prevC = mos6502.m_flags.c;
		mos6502.m_flags.c = data & 0x80;
		data = data << 1;
		if constexpr (rotate) data |= prevC;
		setZN(mos6502, data);

		if constexpr (acc) mos6502.m_regs.a = data;
		else mmu.WritePrg8(args.srcAddr, data);
	}

	template <bool acc, bool rotate>
	static void shiftRight(const Mos6502OpArgs& args)
	{
		auto& mos6502 = args.mos6502.get();
		auto& mmu = args.mmu.get();

		uint8 data;
		if constexpr (acc) data = mos6502.m_regs.a;
		else data = mmu.ReadPrg8(args.srcAddr);

		const bool prevC = mos6502.m_flags.c;
		mos6502.m_flags.c = data & 1;
		data >>= 1;
		if constexpr (rotate) data |= prevC << 7;
		setZN(mos6502, data);

		if constexpr (acc) mos6502.m_regs.a = data;
		else mmu.WritePrg8(args.srcAddr, data);
	}
};
