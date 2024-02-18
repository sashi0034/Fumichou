#include "stdafx.h"
#include "Mos6502Instruction.h"

#include "Mos6502.h"
#include "Mos6502Addressing.h"
#include "Mos6502_In_Op.hpp"

namespace
{
	using namespace Nes;

	using InstructionSet = std::array<Mos6502Instruction, 256>;

	constexpr Mos6502Instruction invalidInstruction = {
		.operation = [](auto) { return; },
		.addressing = [](auto) { return addr16(); },
		.desc = U"INVALID"_sv
	};

	consteval InstructionSet createSet()
	{
		// https://www.makingnesgames.com/Instruction_Set.html
		InstructionSet set{};
		set.fill(invalidInstruction);

		using op = Mos6502::In::Op;
		namespace addr = Mos6502Addressing;

		set[0x69] = {&op::ADC, &addr::Immediate, U"ADC IMM"};
		set[0x65] = {&op::ADC, &addr::ZeroPage, U"ADC ZP"};
		set[0x75] = {&op::ADC, &addr::ZeroPageX, U"ADC ZP,X"};
		set[0x6D] = {&op::ADC, &addr::Absolute, U"ADC ABS"};
		set[0x7D] = {&op::ADC, &addr::AbsoluteX, U"ADC ABS,X"};
		set[0x79] = {&op::ADC, &addr::AbsoluteY, U"ADC ABS,Y"};
		set[0x61] = {&op::ADC, &addr::IndirectX, U"ADC IND,X"};
		set[0x71] = {&op::ADC, &addr::IndirectY, U"ADC IND,Y"};
		set[0x29] = {&op::AND, &addr::Immediate, U"AND IMM"};
		set[0x25] = {&op::AND, &addr::ZeroPage, U"AND ZP"};
		set[0x35] = {&op::AND, &addr::ZeroPageX, U"AND ZP,X"};
		set[0x2D] = {&op::AND, &addr::Absolute, U"AND ABS"};
		set[0x3D] = {&op::AND, &addr::AbsoluteX, U"AND ABS,X"};
		set[0x39] = {&op::AND, &addr::AbsoluteY, U"AND ABS,Y"};
		set[0x21] = {&op::AND, &addr::IndirectX, U"AND IND,X"};
		set[0x31] = {&op::AND, &addr::IndirectY, U"AND IND,Y"};
		set[0x0A] = {&op::ASL<true>, &addr::Accumulator, U"ASL ACC"};
		set[0x06] = {&op::ASL<false>, &addr::ZeroPage, U"ASL ZP"};
		set[0x16] = {&op::ASL<false>, &addr::ZeroPageX, U"ASL ZP,X"};
		set[0x0E] = {&op::ASL<false>, &addr::Absolute, U"ASL ABS"};
		set[0x1E] = {&op::ASL<false>, &addr::AbsoluteX, U"ASL ABS,X"};
		set[0x24] = {&op::BIT, &addr::ZeroPage, U"BIT ZP"};
		set[0x2C] = {&op::BIT, &addr::Absolute, U"BIT ABS"};
		set[0x00] = {&op::BRK, &addr::Implied, U"BRK IMP"};
		set[0x90] = {&op::BCC, &addr::Relative, U"BCC REL"};
		set[0xB0] = {&op::BCS, &addr::Relative, U"BCS REL"};
		set[0xF0] = {&op::BEQ, &addr::Relative, U"BEQ REL"};
		set[0x30] = {&op::BMI, &addr::Relative, U"BMI REL"};
		set[0xD0] = {&op::BNE, &addr::Relative, U"BNE REL"};
		set[0x10] = {&op::BPL, &addr::Relative, U"BPL REL"};
		set[0x50] = {&op::BVC, &addr::Relative, U"BVC REL"};
		set[0x70] = {&op::BVS, &addr::Relative, U"BVS REL"};
		set[0x18] = {&op::CLC, &addr::Implied, U"CLC IMP"};
		set[0xD8] = {&op::CLD, &addr::Implied, U"CLD IMP"};
		set[0x58] = {&op::CLI, &addr::Implied, U"CLI IMP"};
		set[0xB8] = {&op::CLV, &addr::Implied, U"CLV IMP"};
		set[0xC9] = {&op::CMP, &addr::Immediate, U"CMP IMM"};
		set[0xC5] = {&op::CMP, &addr::ZeroPage, U"CMP ZP"};
		set[0xD5] = {&op::CMP, &addr::ZeroPageX, U"CMP ZP,X"};
		set[0xCD] = {&op::CMP, &addr::Absolute, U"CMP ABS"};
		set[0xDD] = {&op::CMP, &addr::AbsoluteX, U"CMP ABS,X"};
		set[0xD9] = {&op::CMP, &addr::AbsoluteY, U"CMP ABS,Y"};
		set[0xC1] = {&op::CMP, &addr::IndirectX, U"CMP IND,X"};
		set[0xD1] = {&op::CMP, &addr::IndirectY, U"CMP IND,Y"};
		set[0xE0] = {&op::CPX, &addr::Immediate, U"CPX IMM"};
		set[0xE4] = {&op::CPX, &addr::ZeroPage, U"CPX ZP"};
		set[0xEC] = {&op::CPX, &addr::Absolute, U"CPX ABS"};
		set[0xC0] = {&op::CPY, &addr::Immediate, U"CPY IMM"};
		set[0xC4] = {&op::CPY, &addr::ZeroPage, U"CPY ZP"};
		set[0xCC] = {&op::CPY, &addr::Absolute, U"CPY ABS"};
		set[0xC6] = {&op::DEC, &addr::ZeroPage, U"DEC ZP"};
		set[0xD6] = {&op::DEC, &addr::ZeroPageX, U"DEC ZP,X"};
		set[0xCE] = {&op::DEC, &addr::Absolute, U"DEC ABS"};
		set[0xDE] = {&op::DEC, &addr::AbsoluteX, U"DEC ABS,X"};
		set[0xCA] = {&op::DEX, &addr::Implied, U"DEX IMP"};
		set[0x88] = {&op::DEY, &addr::Implied, U"DEY IMP"};
		set[0xE6] = {&op::INC, &addr::ZeroPage, U"INC ZP"};
		set[0xF6] = {&op::INC, &addr::ZeroPageX, U"INC ZP,X"};
		set[0xEE] = {&op::INC, &addr::Absolute, U"INC ABS"};
		set[0xFE] = {&op::INC, &addr::AbsoluteX, U"INC ABS,X"};
		set[0xE8] = {&op::INX, &addr::Implied, U"INX IMP"};
		set[0xC8] = {&op::INY, &addr::Implied, U"INY IMP"};
		set[0x4C] = {&op::JMP, &addr::Absolute, U"JMP ABS"};
		set[0x6C] = {&op::JMP, &addr::Indirect, U"JMP IND"};
		set[0x20] = {&op::JSR, &addr::Absolute, U"JSR ABS"};
		set[0xA9] = {&op::LDA, &addr::Immediate, U"LDA IMM"};
		set[0xA5] = {&op::LDA, &addr::ZeroPage, U"LDA ZP"};
		set[0xB5] = {&op::LDA, &addr::ZeroPageX, U"LDA ZP,X"};
		set[0xAD] = {&op::LDA, &addr::Absolute, U"LDA ABS"};
		set[0xBD] = {&op::LDA, &addr::AbsoluteX, U"LDA ABS,X"};
		set[0xB9] = {&op::LDA, &addr::AbsoluteY, U"LDA ABS,Y"};
		set[0xA1] = {&op::LDA, &addr::IndirectX, U"LDA IND,X"};
		set[0xB1] = {&op::LDA, &addr::IndirectY, U"LDA IND,Y"};
		set[0xA2] = {&op::LDX, &addr::Immediate, U"LDX IMM"};
		set[0xA6] = {&op::LDX, &addr::ZeroPage, U"LDX ZP"};
		set[0xB6] = {&op::LDX, &addr::ZeroPageY, U"LDX ZP,Y"};
		set[0xAE] = {&op::LDX, &addr::Absolute, U"LDX ABS"};
		set[0xBE] = {&op::LDX, &addr::AbsoluteY, U"LDX ABS,Y"};
		set[0xA0] = {&op::LDY, &addr::Immediate, U"LDY IMM"};
		set[0xA4] = {&op::LDY, &addr::ZeroPage, U"LDY ZP"};
		set[0xB4] = {&op::LDY, &addr::ZeroPageX, U"LDY ZP,X"};
		set[0xAC] = {&op::LDY, &addr::Absolute, U"LDY ABS"};
		set[0xBC] = {&op::LDY, &addr::AbsoluteX, U"LDY ABS,X"};
		set[0x4A] = {&op::LSR<true>, &addr::Accumulator, U"LSR ACC"};
		set[0x46] = {&op::LSR<false>, &addr::ZeroPage, U"LSR ZP"};
		set[0x56] = {&op::LSR<false>, &addr::ZeroPageX, U"LSR ZP,X"};
		set[0x4E] = {&op::LSR<false>, &addr::Absolute, U"LSR ABS"};
		set[0x5E] = {&op::LSR<false>, &addr::AbsoluteX, U"LSR ABS,X"};
		set[0xEA] = {&op::NOP, &addr::Implied, U"NOP IMP"};
		set[0x49] = {&op::EOR, &addr::Immediate, U"EOR IMM"};
		set[0x45] = {&op::EOR, &addr::ZeroPage, U"EOR ZP"};
		set[0x15] = {&op::EOR, &addr::ZeroPageX, U"EOR ZP,X"};
		set[0x4D] = {&op::EOR, &addr::Absolute, U"EOR ABS"};
		set[0x5D] = {&op::EOR, &addr::AbsoluteX, U"EOR ABS,X"};
		set[0x59] = {&op::EOR, &addr::AbsoluteY, U"EOR ABS,Y"};
		set[0x41] = {&op::EOR, &addr::IndirectX, U"EOR IND,X"};
		set[0x51] = {&op::EOR, &addr::IndirectY, U"EOR IND,Y"};
		set[0x09] = {&op::ORA, &addr::Immediate, U"ORA IMM"};
		set[0x05] = {&op::ORA, &addr::ZeroPage, U"ORA ZP"};
		set[0x15] = {&op::ORA, &addr::ZeroPageX, U"ORA ZP,X"};
		set[0x0D] = {&op::ORA, &addr::Absolute, U"ORA ABS"};
		set[0x1D] = {&op::ORA, &addr::AbsoluteX, U"ORA ABS,X"};
		set[0x19] = {&op::ORA, &addr::AbsoluteY, U"ORA ABS,Y"};
		set[0x01] = {&op::ORA, &addr::IndirectX, U"ORA IND,X"};
		set[0x11] = {&op::ORA, &addr::IndirectY, U"ORA IND,Y"};
		set[0x48] = {&op::PHA, &addr::Implied, U"PHA IMP"};
		set[0x08] = {&op::PHP, &addr::Implied, U"PHP IMP"};
		set[0x68] = {&op::PLA, &addr::Implied, U"PLA IMP"};
		set[0x28] = {&op::PLP, &addr::Implied, U"PLP IMP"};
		set[0x2A] = {&op::ROL<true>, &addr::Accumulator, U"ROL ACC"};
		set[0x26] = {&op::ROL<false>, &addr::ZeroPage, U"ROL ZP"};
		set[0x36] = {&op::ROL<false>, &addr::ZeroPageX, U"ROL ZP,X"};
		set[0x2E] = {&op::ROL<false>, &addr::Absolute, U"ROL ABS"};
		set[0x3E] = {&op::ROL<false>, &addr::AbsoluteX, U"ROL ABS,X"};
		set[0x6A] = {&op::ROR<true>, &addr::Accumulator, U"ROR ACC"};
		set[0x66] = {&op::ROR<false>, &addr::ZeroPage, U"ROR ZP"};
		set[0x76] = {&op::ROR<false>, &addr::ZeroPageX, U"ROR ZP,X"};
		set[0x6E] = {&op::ROR<false>, &addr::Absolute, U"ROR ABS"};
		set[0x7E] = {&op::ROR<false>, &addr::AbsoluteX, U"ROR ABS,X"};
		set[0x40] = {&op::RTI, &addr::Implied, U"RTI IMP"};
		set[0x60] = {&op::RTS, &addr::Implied, U"RTS IMP"};
		set[0xE9] = {&op::SBC, &addr::Immediate, U"SBC IMM"};
		set[0xE5] = {&op::SBC, &addr::ZeroPage, U"SBC ZP"};
		set[0xF5] = {&op::SBC, &addr::ZeroPageX, U"SBC ZP,X"};
		set[0xED] = {&op::SBC, &addr::Absolute, U"SBC ABS"};
		set[0xFD] = {&op::SBC, &addr::AbsoluteX, U"SBC ABS,X"};
		set[0xF9] = {&op::SBC, &addr::AbsoluteY, U"SBC ABS,Y"};
		set[0xE1] = {&op::SBC, &addr::IndirectX, U"SBC IND,X"};
		set[0xF1] = {&op::SBC, &addr::IndirectY, U"SBC IND,Y"};
		set[0x38] = {&op::SEC, &addr::Implied, U"SEC IMP"};
		set[0xF8] = {&op::SED, &addr::Implied, U"SED IMP"};
		set[0x78] = {&op::SEI, &addr::Implied, U"SEI IMP"};
		set[0x85] = {&op::STA, &addr::ZeroPage, U"STA ZP"};
		set[0x95] = {&op::STA, &addr::ZeroPageX, U"STA ZP,X"};
		set[0x8D] = {&op::STA, &addr::Absolute, U"STA ABS"};
		set[0x9D] = {&op::STA, &addr::AbsoluteX, U"STA ABS,X"};
		set[0x99] = {&op::STA, &addr::AbsoluteY, U"STA ABS,Y"};
		set[0x81] = {&op::STA, &addr::IndirectX, U"STA IND,X"};
		set[0x91] = {&op::STA, &addr::IndirectY, U"STA IND,Y"};
		set[0x86] = {&op::STX, &addr::ZeroPage, U"STX ZP"};
		set[0x96] = {&op::STX, &addr::ZeroPageY, U"STX ZP,Y"};
		set[0x8E] = {&op::STX, &addr::Absolute, U"STX ABS"};
		set[0x84] = {&op::STY, &addr::ZeroPage, U"STY ZP"};
		set[0x94] = {&op::STY, &addr::ZeroPageX, U"STY ZP,X"};
		set[0x8C] = {&op::STY, &addr::Absolute, U"STY ABS"};
		set[0xAA] = {&op::TAX, &addr::Implied, U"TAX IMP"};
		set[0xA8] = {&op::TAY, &addr::Implied, U"TAY IMP"};
		set[0xBA] = {&op::TSX, &addr::Implied, U"TSX IMP"};
		set[0x8A] = {&op::TXA, &addr::Implied, U"TXA IMP"};
		set[0x9A] = {&op::TXS, &addr::Implied, U"TXS IMP"};
		set[0x98] = {&op::TYA, &addr::Implied, U"TYA IMP"};
		return set;
	}
}

namespace Nes
{
	const Mos6502Instruction& GetMos6502Instruction(uint8 opcode)
	{
		static constexpr auto instructionSet = createSet();
		return instructionSet[opcode];
	}

	CpuCycle GetMos6502OperationCycleCount(uint8 opcode)
	{
		static constexpr std::array<CpuCycle, 256> cycleCounts = {
			7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
			2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
			2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
			2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
			2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
			2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
			2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
		};
		return cycleCounts[opcode];
	}
}
