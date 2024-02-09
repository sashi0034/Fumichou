#pragma once
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
		Logger::Abort();
	}

	static void AND(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void ASL(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void BIT(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void BRK(const Mos6502OpArgs& args)
	{
		Logger::Abort();
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
		Logger::Abort();
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
		Logger::Abort();
	}

	static void CPX(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void CPY(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void DEC(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void DEX(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void DEY(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void INC(const Mos6502OpArgs& args)
	{
		Logger::Abort();
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

	static void LSR(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void NOP(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void EOR(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void ORA(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void PHA(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void PHP(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void PLA(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void PLP(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void ROL(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void ROR(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void RTI(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void RTS(const Mos6502OpArgs& args)
	{
		Logger::Abort();
	}

	static void SBC(const Mos6502OpArgs& args)
	{
		Logger::Abort();
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
		mos6502.m_flags.z = value != 0;
		mos6502.m_flags.n = value & 0x80;
	}

	static void branchConditional(const Mos6502OpArgs& args)
	{
		args.consumedCycles += 1 + args.pageBoundary; // 分岐すると {1, 2} サイクル遅れる
		args.mos6502.get().m_regs.pc = args.srcAddr;
	}
};
