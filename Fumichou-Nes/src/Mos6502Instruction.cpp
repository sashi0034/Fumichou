#include "stdafx.h"
#include "Mos6502Instruction.h"

#include "Mos6502.h"
#include "Mos6502Addressing.h"

namespace
{
	using namespace Nes;

	using InstructionSet = std::array<Mos6502Instruction, 0x100>;

	consteval InstructionSet createSet()
	{
		// https://www.makingnesgames.com/Instruction_Set.html
		InstructionSet set{};
		/*
		namespace addr = Mos6502Addressing;
		set[0x69] = {&o::ADC, &addr::Immediate};
		set[0x65] = {&o::ADC, &addr::ZeroPage};
		set[0x75] = {&o::ADC, &addr::ZeroPageX};
		set[0x6D] = {&o::ADC, &addr::Absolute};
		set[0x7D] = {&o::ADC, &addr::AbsoluteX};
		set[0x79] = {&o::ADC, &addr::AbsoluteY};
		set[0x61] = {&o::ADC, &addr::IndirectX};
		set[0x71] = {&o::ADC, &addr::IndirectY};
		set[0x29] = {&o::AND, &addr::Immediate};
		set[0x25] = {&o::AND, &addr::ZeroPage};
		set[0x35] = {&o::AND, &addr::ZeroPageX};
		set[0x2D] = {&o::AND, &addr::Absolute};
		set[0x3D] = {&o::AND, &addr::AbsoluteX};
		set[0x39] = {&o::AND, &addr::AbsoluteY};
		set[0x21] = {&o::AND, &addr::IndirectX};
		set[0x31] = {&o::AND, &addr::IndirectY};
		set[0x0A] = {&o::ASL, &addr::Accumulator};
		set[0x06] = {&o::ASL, &addr::ZeroPage};
		set[0x16] = {&o::ASL, &addr::ZeroPageX};
		set[0x0E] = {&o::ASL, &addr::Absolute};
		set[0x1E] = {&o::ASL, &addr::AbsoluteX};
		set[0x24] = {&o::BIT, &addr::ZeroPage};
		set[0x2C] = {&o::BIT, &addr::Absolute};
		set[0x00] = {&o::BRK, &addr::Implied};
		set[0x90] = {&o::BCC, &addr::Relative};
		set[0xB0] = {&o::BCS, &addr::Relative};
		set[0xF0] = {&o::BEQ, &addr::Relative};
		set[0x30] = {&o::BMI, &addr::Relative};
		set[0xD0] = {&o::BNE, &addr::Relative};
		set[0x10] = {&o::BPL, &addr::Relative};
		set[0x50] = {&o::BVC, &addr::Relative};
		set[0x70] = {&o::BVS, &addr::Relative};
		set[0x18] = {&o::CLC, &addr::Implied};
		set[0xD8] = {&o::CLD, &addr::Implied};
		set[0x58] = {&o::CLI, &addr::Implied};
		set[0xB8] = {&o::CLV, &addr::Implied};
		set[0xC9] = {&o::CMP, &addr::Immediate};
		set[0xC5] = {&o::CMP, &addr::ZeroPage};
		set[0xD5] = {&o::CMP, &addr::ZeroPageX};
		set[0xCD] = {&o::CMP, &addr::Absolute};
		set[0xDD] = {&o::CMP, &addr::AbsoluteX};
		set[0xD9] = {&o::CMP, &addr::AbsoluteY};
		set[0xC1] = {&o::CMP, &addr::IndirectX};
		set[0xD1] = {&o::CMP, &addr::IndirectY};
		set[0xE0] = {&o::CPX, &addr::Immediate};
		set[0xE4] = {&o::CPX, &addr::ZeroPage};
		set[0xEC] = {&o::CPX, &addr::Absolute};
		set[0xC0] = {&o::CPY, &addr::Immediate};
		set[0xC4] = {&o::CPY, &addr::ZeroPage};
		set[0xCC] = {&o::CPY, &addr::Absolute};
		set[0xC6] = {&o::DEC, &addr::ZeroPage};
		set[0xD6] = {&o::DEC, &addr::ZeroPageX};
		set[0xCE] = {&o::DEC, &addr::Absolute};
		set[0xDE] = {&o::DEC, &addr::AbsoluteX};
		set[0xCA] = {&o::DEX, &addr::Implied};
		set[0x88] = {&o::DEY, &addr::Implied};
		set[0xE6] = {&o::INC, &addr::ZeroPage};
		set[0xF6] = {&o::INC, &addr::ZeroPageX};
		set[0xEE] = {&o::INC, &addr::Absolute};
		set[0xFE] = {&o::INC, &addr::AbsoluteX};
		set[0xE8] = {&o::INX, &addr::Implied};
		set[0xC8] = {&o::INY, &addr::Implied};
		set[0x4C] = {&o::JMP, &addr::Absolute};
		set[0x6C] = {&o::JMP, &addr::Indirect};
		set[0x20] = {&o::JSR, &addr::Absolute};
		set[0xA9] = {&o::LDA, &addr::Immediate};
		set[0xA5] = {&o::LDA, &addr::ZeroPage};
		set[0xB5] = {&o::LDA, &addr::ZeroPageX};
		set[0xAD] = {&o::LDA, &addr::Absolute};
		set[0xBD] = {&o::LDA, &addr::AbsoluteX};
		set[0xB9] = {&o::LDA, &addr::AbsoluteY};
		set[0xA1] = {&o::LDA, &addr::IndirectX};
		set[0xB1] = {&o::LDA, &addr::IndirectY};
		set[0xA2] = {&o::LDX, &addr::Immediate};
		set[0xA6] = {&o::LDX, &addr::ZeroPage};
		set[0xB6] = {&o::LDX, &addr::ZeroPageY};
		set[0xAE] = {&o::LDX, &addr::Absolute};
		set[0xBE] = {&o::LDX, &addr::AbsoluteY};
		set[0xA0] = {&o::LDY, &addr::Immediate};
		set[0xA4] = {&o::LDY, &addr::ZeroPage};
		set[0xB4] = {&o::LDY, &addr::ZeroPageX};
		set[0xAC] = {&o::LDY, &addr::Absolute};
		set[0xBC] = {&o::LDY, &addr::AbsoluteX};
		set[0x4A] = {&o::LSR, &addr::Accumulator};
		set[0x46] = {&o::LSR, &addr::ZeroPage};
		set[0x56] = {&o::LSR, &addr::ZeroPageX};
		set[0x4E] = {&o::LSR, &addr::Absolute};
		set[0x5E] = {&o::LSR, &addr::AbsoluteX};
		set[0xEA] = {&o::NOP, &addr::Implied};
		set[0x49] = {&o::EOR, &addr::Immediate};
		set[0x45] = {&o::EOR, &addr::ZeroPage};
		set[0x15] = {&o::EOR, &addr::ZeroPageX};
		set[0x4D] = {&o::EOR, &addr::Absolute};
		set[0x5D] = {&o::EOR, &addr::AbsoluteX};
		set[0x59] = {&o::EOR, &addr::AbsoluteY};
		set[0x41] = {&o::EOR, &addr::IndirectX};
		set[0x51] = {&o::EOR, &addr::IndirectY};
		set[0x09] = {&o::ORA, &addr::Immediate};
		set[0x05] = {&o::ORA, &addr::ZeroPage};
		set[0x15] = {&o::ORA, &addr::ZeroPageX};
		set[0x0D] = {&o::ORA, &addr::Absolute};
		set[0x1D] = {&o::ORA, &addr::AbsoluteX};
		set[0x19] = {&o::ORA, &addr::AbsoluteY};
		set[0x01] = {&o::ORA, &addr::IndirectX};
		set[0x11] = {&o::ORA, &addr::IndirectY};
		set[0x48] = {&o::PHA, &addr::Implied};
		set[0x08] = {&o::PHP, &addr::Implied};
		set[0x68] = {&o::PLA, &addr::Implied};
		set[0x28] = {&o::PLP, &addr::Implied};
		set[0x2A] = {&o::ROL, &addr::Accumulator};
		set[0x26] = {&o::ROL, &addr::ZeroPage};
		set[0x36] = {&o::ROL, &addr::ZeroPageX};
		set[0x2E] = {&o::ROL, &addr::Absolute};
		set[0x3E] = {&o::ROL, &addr::AbsoluteX};
		set[0x6A] = {&o::ROR, &addr::Accumulator};
		set[0x66] = {&o::ROR, &addr::ZeroPage};
		set[0x76] = {&o::ROR, &addr::ZeroPageX};
		set[0x6E] = {&o::ROR, &addr::Absolute};
		set[0x7E] = {&o::ROR, &addr::AbsoluteX};
		set[0x40] = {&o::RTI, &addr::Implied};
		set[0x60] = {&o::RTS, &addr::Implied};
		set[0xE9] = {&o::SBC, &addr::Immediate};
		set[0xE5] = {&o::SBC, &addr::ZeroPage};
		set[0xF5] = {&o::SBC, &addr::ZeroPageX};
		set[0xED] = {&o::SBC, &addr::Absolute};
		set[0xFD] = {&o::SBC, &addr::AbsoluteX};
		set[0xF9] = {&o::SBC, &addr::AbsoluteY};
		set[0xE1] = {&o::SBC, &addr::IndirectX};
		set[0xF1] = {&o::SBC, &addr::IndirectY};
		set[0x38] = {&o::SEC, &addr::Implied};
		set[0xF8] = {&o::SED, &addr::Implied};
		set[0x78] = {&o::SEI, &addr::Implied};
		set[0x85] = {&o::STA, &addr::ZeroPage};
		set[0x95] = {&o::STA, &addr::ZeroPageX};
		set[0x8D] = {&o::STA, &addr::Absolute};
		set[0x9D] = {&o::STA, &addr::AbsoluteX};
		set[0x99] = {&o::STA, &addr::AbsoluteY};
		set[0x81] = {&o::STA, &addr::IndirectX};
		set[0x91] = {&o::STA, &addr::IndirectY};
		set[0x86] = {&o::STX, &addr::ZeroPage};
		set[0x96] = {&o::STX, &addr::ZeroPageY};
		set[0x8E] = {&o::STX, &addr::Absolute};
		set[0x84] = {&o::STY, &addr::ZeroPage};
		set[0x94] = {&o::STY, &addr::ZeroPageX};
		set[0x8C] = {&o::STY, &addr::Absolute};
		set[0xAA] = {&o::TAX, &addr::Implied};
		set[0xA8] = {&o::TAY, &addr::Implied};
		set[0xBA] = {&o::TSX, &addr::Implied};
		set[0x8A] = {&o::TXA, &addr::Implied};
		set[0x9A] = {&o::TXS, &addr::Implied};
		set[0x98] = {&o::TYA, &addr::Implied};
		*/
		return set;
	}
}

namespace Nes
{
}
