#include "stdafx.h"
#include "Mos6502Addressing.h"

#include "Hardware.h"
#include "Mmu.h"

namespace Nes::Mos6502Addressing
{
	addr16 Immediate(const Mos6502AddressingArgs& args)
	{
		const addr16 addr = args.pc;
		args.pc++;
		return addr;
	}

	addr16 ZeroPage(const Mos6502AddressingArgs& args)
	{
		const addr16 addr = args.pc;
		args.pc++;
		auto& hw = args.hw.get();
		return hw.GetMmu().ReadPrg8(addr);
	}

	addr16 ZeroPageX(const Mos6502AddressingArgs& args)
	{
		const addr16 addr = args.pc;
		args.pc++;
		auto& hw = args.hw.get();
		return (hw.GetMmu().ReadPrg8(addr) + hw.GetMos6502().GetRegs().x) & 0xFF;
	}

	addr16 ZeroPageY(const Mos6502AddressingArgs& args)
	{
		const addr16 addr = args.pc;
		args.pc++;
		auto& hw = args.hw.get();
		return (hw.GetMmu().ReadPrg8(addr) + hw.GetMos6502().GetRegs().y) & 0xFF;
	}

	addr16 Indirect(const Mos6502AddressingArgs& args)
	{
		auto& hw = args.hw.get();
		const addr16 temp0 = hw.GetMmu().ReadPrg16(args.pc);
		args.pc += 2;

		// 6502のバグで、連続2バイトがページの端をまたぐとき、2バイト目が次のページの最初ではなく同じページの最初のバイトになるらしい
		// 例えば 10FF のとき (10FF, 1100) を読むのではなく (10FF, 1000) になる
		const addr16 temp1 = (temp0 & 0xFF00) | ((temp0 + 1) & 0xFF);

		return hw.GetMmu().ReadPrg8(temp0) | (hw.GetMmu().ReadPrg8(temp1) << 8);
	}

	addr16 IndirectX(const Mos6502AddressingArgs& args)
	{
		auto& hw = args.hw.get();
		const uint8 temp0 = hw.GetMmu().ReadPrg8(args.pc);
		args.pc++;
		const uint16 tempX = temp0 + hw.GetMos6502().GetRegs().x;
		// オペランドとXレジスタの和より、ゼロページからアドレスを求める
		return hw.GetMmu().ReadPrg8(tempX & 0xFF) | (hw.GetMmu().ReadPrg8((tempX + 1) & 0xFF) << 8);
	}

	addr16 IndirectY(const Mos6502AddressingArgs& args)
	{
		auto& hw = args.hw.get();
		const uint8 temp0 = hw.GetMmu().ReadPrg8(args.pc);
		args.pc++;
		// メモリから間接指定に使用するアドレスをロード
		const uint16 loaded = hw.GetMmu().ReadPrg8(temp0 & 0xFF) | (hw.GetMmu().ReadPrg8((temp0 + 1) & 0xFF) << 8);

		// https://www.nesdev.org/wiki/6502_cycle_times
		// IND,Y は A と X と違ってサイクル遅れが発生する可能性あり
		args.pageBoundary.get() = temp0 == 0xFF;

		// ロードしたアドレスとYを用いる
		return loaded + hw.GetMos6502().GetRegs().y;
	}

	addr16 Accumulator(const Mos6502AddressingArgs& args)
	{
		return 0;
	}

	addr16 Absolute(const Mos6502AddressingArgs& args)
	{
		auto& hw = args.hw.get();
		const addr16 addr = hw.GetMmu().ReadPrg16(args.pc);
		args.pc += 2;
		return addr;
	}

	addr16 AbsoluteX(const Mos6502AddressingArgs& args)
	{
		auto& hw = args.hw.get();
		const addr16 addr0 = hw.GetMmu().ReadPrg16(args.pc);
		args.pc += 2;
		const addr16 addrX = addr0 + hw.GetMos6502().GetRegs().x;
		args.pageBoundary.get() = (addr0 & 0xFF00) != (addrX & 0xFF00);
		return addrX;
	}

	addr16 AbsoluteY(const Mos6502AddressingArgs& args)
	{
		auto& hw = args.hw.get();
		const addr16 addr0 = hw.GetMmu().ReadPrg16(args.pc);
		args.pc += 2;
		const addr16 addrY = addr0 + hw.GetMos6502().GetRegs().y;
		args.pageBoundary.get() = (addr0 & 0xFF00) != (addrY & 0xFF00);
		return addrY;
	}

	addr16 Relative(const Mos6502AddressingArgs& args)
	{
		auto& hw = args.hw.get();
		const sint8 value = hw.GetMmu().ReadPrg8(args.pc);
		args.pc++;
		const uint16 offset = static_cast<sint16>(value);
		return args.pc + offset;
	}

	addr16 Implied(const Mos6502AddressingArgs& args)
	{
		return 0;
	}
}
