#pragma once
#include "Logger.h"
#include "Mmu.h"
#include "Mos6502Forward.h"
#include "Mos6502_In.h"

namespace Nes
{
	// 命令資料
	// https://www.nesdev.org/obelisk-6502-guide/reference.html

	// サイクル数資料
	// https://www.nesdev.org/wiki/6502_cycle_times

	class Mos6502::In::Op
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
			Logger::Abort();
		}

		static void BCS(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void BEQ(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void BMI(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void BNE(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void BPL(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void BVC(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void BVS(const Mos6502OpArgs& args)
		{
			Logger::Abort();
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
			Logger::Abort();
		}

		static void INY(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void JMP(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void JSR(const Mos6502OpArgs& args)
		{
			Logger::Abort();
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
			Logger::Abort();
		}

		static void TAY(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void TSX(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void TXA(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void TXS(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

		static void TYA(const Mos6502OpArgs& args)
		{
			Logger::Abort();
		}

	private:
		static void setZN(Mos6502& mos6502, uint8 value)
		{
			mos6502.m_flags.z = value != 0;
			mos6502.m_flags.n = value & 0x80;
		}
	};
}
