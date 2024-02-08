#include "stdafx.h"
#include "Mos6502Instruction.h"

#include "Mos6502.h"
#include "Mos6502Addressing.h"
#include "Mos6502_In_Op.hpp"

namespace
{
	using namespace Nes;

	using InstructionSet = std::array<Mos6502Instruction, 0x100>;

	consteval InstructionSet createSet()
	{
		// https://www.makingnesgames.com/Instruction_Set.html
		InstructionSet set{};
		using op = Mos6502::In::Op;
		namespace addr = Mos6502Addressing;
		set[0x69] = {&op::ADC, &addr::Immediate};
		set[0x65] = {&op::ADC, &addr::ZeroPage};
		set[0x75] = {&op::ADC, &addr::ZeroPageX};
		set[0x6D] = {&op::ADC, &addr::Absolute};
		set[0x7D] = {&op::ADC, &addr::AbsoluteX};
		set[0x79] = {&op::ADC, &addr::AbsoluteY};
		set[0x61] = {&op::ADC, &addr::IndirectX};
		set[0x71] = {&op::ADC, &addr::IndirectY};
		set[0x29] = {&op::AND, &addr::Immediate};
		set[0x25] = {&op::AND, &addr::ZeroPage};
		set[0x35] = {&op::AND, &addr::ZeroPageX};
		set[0x2D] = {&op::AND, &addr::Absolute};
		set[0x3D] = {&op::AND, &addr::AbsoluteX};
		set[0x39] = {&op::AND, &addr::AbsoluteY};
		set[0x21] = {&op::AND, &addr::IndirectX};
		set[0x31] = {&op::AND, &addr::IndirectY};
		set[0x0A] = {&op::ASL, &addr::Accumulator};
		set[0x06] = {&op::ASL, &addr::ZeroPage};
		set[0x16] = {&op::ASL, &addr::ZeroPageX};
		set[0x0E] = {&op::ASL, &addr::Absolute};
		set[0x1E] = {&op::ASL, &addr::AbsoluteX};
		set[0x24] = {&op::BIT, &addr::ZeroPage};
		set[0x2C] = {&op::BIT, &addr::Absolute};
		set[0x00] = {&op::BRK, &addr::Implied};
		set[0x90] = {&op::BCC, &addr::Relative};
		set[0xB0] = {&op::BCS, &addr::Relative};
		set[0xF0] = {&op::BEQ, &addr::Relative};
		set[0x30] = {&op::BMI, &addr::Relative};
		set[0xD0] = {&op::BNE, &addr::Relative};
		set[0x10] = {&op::BPL, &addr::Relative};
		set[0x50] = {&op::BVC, &addr::Relative};
		set[0x70] = {&op::BVS, &addr::Relative};
		set[0x18] = {&op::CLC, &addr::Implied};
		set[0xD8] = {&op::CLD, &addr::Implied};
		set[0x58] = {&op::CLI, &addr::Implied};
		set[0xB8] = {&op::CLV, &addr::Implied};
		set[0xC9] = {&op::CMP, &addr::Immediate};
		set[0xC5] = {&op::CMP, &addr::ZeroPage};
		set[0xD5] = {&op::CMP, &addr::ZeroPageX};
		set[0xCD] = {&op::CMP, &addr::Absolute};
		set[0xDD] = {&op::CMP, &addr::AbsoluteX};
		set[0xD9] = {&op::CMP, &addr::AbsoluteY};
		set[0xC1] = {&op::CMP, &addr::IndirectX};
		set[0xD1] = {&op::CMP, &addr::IndirectY};
		set[0xE0] = {&op::CPX, &addr::Immediate};
		set[0xE4] = {&op::CPX, &addr::ZeroPage};
		set[0xEC] = {&op::CPX, &addr::Absolute};
		set[0xC0] = {&op::CPY, &addr::Immediate};
		set[0xC4] = {&op::CPY, &addr::ZeroPage};
		set[0xCC] = {&op::CPY, &addr::Absolute};
		set[0xC6] = {&op::DEC, &addr::ZeroPage};
		set[0xD6] = {&op::DEC, &addr::ZeroPageX};
		set[0xCE] = {&op::DEC, &addr::Absolute};
		set[0xDE] = {&op::DEC, &addr::AbsoluteX};
		set[0xCA] = {&op::DEX, &addr::Implied};
		set[0x88] = {&op::DEY, &addr::Implied};
		set[0xE6] = {&op::INC, &addr::ZeroPage};
		set[0xF6] = {&op::INC, &addr::ZeroPageX};
		set[0xEE] = {&op::INC, &addr::Absolute};
		set[0xFE] = {&op::INC, &addr::AbsoluteX};
		set[0xE8] = {&op::INX, &addr::Implied};
		set[0xC8] = {&op::INY, &addr::Implied};
		set[0x4C] = {&op::JMP, &addr::Absolute};
		set[0x6C] = {&op::JMP, &addr::Indirect};
		set[0x20] = {&op::JSR, &addr::Absolute};
		set[0xA9] = {&op::LDA, &addr::Immediate};
		set[0xA5] = {&op::LDA, &addr::ZeroPage};
		set[0xB5] = {&op::LDA, &addr::ZeroPageX};
		set[0xAD] = {&op::LDA, &addr::Absolute};
		set[0xBD] = {&op::LDA, &addr::AbsoluteX};
		set[0xB9] = {&op::LDA, &addr::AbsoluteY};
		set[0xA1] = {&op::LDA, &addr::IndirectX};
		set[0xB1] = {&op::LDA, &addr::IndirectY};
		set[0xA2] = {&op::LDX, &addr::Immediate};
		set[0xA6] = {&op::LDX, &addr::ZeroPage};
		set[0xB6] = {&op::LDX, &addr::ZeroPageY};
		set[0xAE] = {&op::LDX, &addr::Absolute};
		set[0xBE] = {&op::LDX, &addr::AbsoluteY};
		set[0xA0] = {&op::LDY, &addr::Immediate};
		set[0xA4] = {&op::LDY, &addr::ZeroPage};
		set[0xB4] = {&op::LDY, &addr::ZeroPageX};
		set[0xAC] = {&op::LDY, &addr::Absolute};
		set[0xBC] = {&op::LDY, &addr::AbsoluteX};
		set[0x4A] = {&op::LSR, &addr::Accumulator};
		set[0x46] = {&op::LSR, &addr::ZeroPage};
		set[0x56] = {&op::LSR, &addr::ZeroPageX};
		set[0x4E] = {&op::LSR, &addr::Absolute};
		set[0x5E] = {&op::LSR, &addr::AbsoluteX};
		set[0xEA] = {&op::NOP, &addr::Implied};
		set[0x49] = {&op::EOR, &addr::Immediate};
		set[0x45] = {&op::EOR, &addr::ZeroPage};
		set[0x15] = {&op::EOR, &addr::ZeroPageX};
		set[0x4D] = {&op::EOR, &addr::Absolute};
		set[0x5D] = {&op::EOR, &addr::AbsoluteX};
		set[0x59] = {&op::EOR, &addr::AbsoluteY};
		set[0x41] = {&op::EOR, &addr::IndirectX};
		set[0x51] = {&op::EOR, &addr::IndirectY};
		set[0x09] = {&op::ORA, &addr::Immediate};
		set[0x05] = {&op::ORA, &addr::ZeroPage};
		set[0x15] = {&op::ORA, &addr::ZeroPageX};
		set[0x0D] = {&op::ORA, &addr::Absolute};
		set[0x1D] = {&op::ORA, &addr::AbsoluteX};
		set[0x19] = {&op::ORA, &addr::AbsoluteY};
		set[0x01] = {&op::ORA, &addr::IndirectX};
		set[0x11] = {&op::ORA, &addr::IndirectY};
		set[0x48] = {&op::PHA, &addr::Implied};
		set[0x08] = {&op::PHP, &addr::Implied};
		set[0x68] = {&op::PLA, &addr::Implied};
		set[0x28] = {&op::PLP, &addr::Implied};
		set[0x2A] = {&op::ROL, &addr::Accumulator};
		set[0x26] = {&op::ROL, &addr::ZeroPage};
		set[0x36] = {&op::ROL, &addr::ZeroPageX};
		set[0x2E] = {&op::ROL, &addr::Absolute};
		set[0x3E] = {&op::ROL, &addr::AbsoluteX};
		set[0x6A] = {&op::ROR, &addr::Accumulator};
		set[0x66] = {&op::ROR, &addr::ZeroPage};
		set[0x76] = {&op::ROR, &addr::ZeroPageX};
		set[0x6E] = {&op::ROR, &addr::Absolute};
		set[0x7E] = {&op::ROR, &addr::AbsoluteX};
		set[0x40] = {&op::RTI, &addr::Implied};
		set[0x60] = {&op::RTS, &addr::Implied};
		set[0xE9] = {&op::SBC, &addr::Immediate};
		set[0xE5] = {&op::SBC, &addr::ZeroPage};
		set[0xF5] = {&op::SBC, &addr::ZeroPageX};
		set[0xED] = {&op::SBC, &addr::Absolute};
		set[0xFD] = {&op::SBC, &addr::AbsoluteX};
		set[0xF9] = {&op::SBC, &addr::AbsoluteY};
		set[0xE1] = {&op::SBC, &addr::IndirectX};
		set[0xF1] = {&op::SBC, &addr::IndirectY};
		set[0x38] = {&op::SEC, &addr::Implied};
		set[0xF8] = {&op::SED, &addr::Implied};
		set[0x78] = {&op::SEI, &addr::Implied};
		set[0x85] = {&op::STA, &addr::ZeroPage};
		set[0x95] = {&op::STA, &addr::ZeroPageX};
		set[0x8D] = {&op::STA, &addr::Absolute};
		set[0x9D] = {&op::STA, &addr::AbsoluteX};
		set[0x99] = {&op::STA, &addr::AbsoluteY};
		set[0x81] = {&op::STA, &addr::IndirectX};
		set[0x91] = {&op::STA, &addr::IndirectY};
		set[0x86] = {&op::STX, &addr::ZeroPage};
		set[0x96] = {&op::STX, &addr::ZeroPageY};
		set[0x8E] = {&op::STX, &addr::Absolute};
		set[0x84] = {&op::STY, &addr::ZeroPage};
		set[0x94] = {&op::STY, &addr::ZeroPageX};
		set[0x8C] = {&op::STY, &addr::Absolute};
		set[0xAA] = {&op::TAX, &addr::Implied};
		set[0xA8] = {&op::TAY, &addr::Implied};
		set[0xBA] = {&op::TSX, &addr::Implied};
		set[0x8A] = {&op::TXA, &addr::Implied};
		set[0x9A] = {&op::TXS, &addr::Implied};
		set[0x98] = {&op::TYA, &addr::Implied};
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

	CycleCount GetMos6502OperationCycleCount(uint8 opcode)
	{
		static constexpr std::array<uint8, 256> cycleCounts = {
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
