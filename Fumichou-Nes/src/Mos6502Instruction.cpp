#include "stdafx.h"
#include "Mos6502Instruction.h"

namespace
{
	using namespace Nes;

	using InstructionSet = std::array<Mos6502Instruction, 0x100>;

	consteval InstructionSet create()
	{
		InstructionSet set{};
		/* TODO
		set[0x69] = {&o::ADC, &a::Immediate};
		set[0x65] = {&o::ADC, &a::ZeroPage};
		set[0x75] = {&o::ADC, &a::ZeroPageX};
		set[0x6D] = {&o::ADC, &a::Absolute};
		set[0x7D] = {&o::ADC, &a::AbsoluteX};
		set[0x79] = {&o::ADC, &a::AbsoluteY};
		set[0x61] = {&o::ADC, &a::IndirectX};
		set[0x71] = {&o::ADC, &a::IndirectY};
		set[0x29] = {&o::AND, &a::Immediate};
		set[0x25] = {&o::AND, &a::ZeroPage};
		set[0x35] = {&o::AND, &a::ZeroPageX};
		set[0x2D] = {&o::AND, &a::Absolute};
		set[0x3D] = {&o::AND, &a::AbsoluteX};
		set[0x39] = {&o::AND, &a::AbsoluteY};
		set[0x21] = {&o::AND, &a::IndirectX};
		set[0x31] = {&o::AND, &a::IndirectY};
		set[0x0A] = {&o::ASL, &a::Accumulator};
		set[0x06] = {&o::ASL, &a::ZeroPage};
		set[0x16] = {&o::ASL, &a::ZeroPageX};
		set[0x0E] = {&o::ASL, &a::Absolute};
		set[0x1E] = {&o::ASL, &a::AbsoluteX};
		set[0x24] = {&o::BIT, &a::ZeroPage};
		set[0x2C] = {&o::BIT, &a::Absolute};
		set[0x00] = {&o::BRK, &a::Implied};
		set[0x90] = {&o::BCC, &a::Relative};
		set[0xB0] = {&o::BCS, &a::Relative};
		set[0xF0] = {&o::BEQ, &a::Relative};
		set[0x30] = {&o::BMI, &a::Relative};
		set[0xD0] = {&o::BNE, &a::Relative};
		set[0x10] = {&o::BPL, &a::Relative};
		set[0x50] = {&o::BVC, &a::Relative};
		set[0x70] = {&o::BVS, &a::Relative};
		set[0x18] = {&o::CLC, &a::Implied};
		set[0xD8] = {&o::CLD, &a::Implied};
		set[0x58] = {&o::CLI, &a::Implied};
		set[0xB8] = {&o::CLV, &a::Implied};
		set[0xC9] = {&o::CMP, &a::Immediate};
		set[0xC5] = {&o::CMP, &a::ZeroPage};
		set[0xD5] = {&o::CMP, &a::ZeroPageX};
		set[0xCD] = {&o::CMP, &a::Absolute};
		set[0xDD] = {&o::CMP, &a::AbsoluteX};
		set[0xD9] = {&o::CMP, &a::AbsoluteY};
		set[0xC1] = {&o::CMP, &a::IndirectX};
		set[0xD1] = {&o::CMP, &a::IndirectY};
		set[0xE0] = {&o::CPX, &a::Immediate};
		set[0xE4] = {&o::CPX, &a::ZeroPage};
		set[0xEC] = {&o::CPX, &a::Absolute};
		set[0xC0] = {&o::CPY, &a::Immediate};
		set[0xC4] = {&o::CPY, &a::ZeroPage};
		set[0xCC] = {&o::CPY, &a::Absolute};
		set[0xC6] = {&o::DEC, &a::ZeroPage};
		set[0xD6] = {&o::DEC, &a::ZeroPageX};
		set[0xCE] = {&o::DEC, &a::Absolute};
		set[0xDE] = {&o::DEC, &a::AbsoluteX};
		set[0xCA] = {&o::DEX, &a::Implied};
		set[0x88] = {&o::DEY, &a::Implied};
		set[0xE6] = {&o::INC, &a::ZeroPage};
		set[0xF6] = {&o::INC, &a::ZeroPageX};
		set[0xEE] = {&o::INC, &a::Absolute};
		set[0xFE] = {&o::INC, &a::AbsoluteX};
		set[0xE8] = {&o::INX, &a::Implied};
		set[0xC8] = {&o::INY, &a::Implied};
		set[0x4C] = {&o::JMP, &a::Absolute};
		set[0x6C] = {&o::JMP, &a::Indirect};
		set[0x20] = {&o::JSR, &a::Absolute};
		set[0xA9] = {&o::LDA, &a::Immediate};
		set[0xA5] = {&o::LDA, &a::ZeroPage};
		set[0xB5] = {&o::LDA, &a::ZeroPageX};
		set[0xAD] = {&o::LDA, &a::Absolute};
		set[0xBD] = {&o::LDA, &a::AbsoluteX};
		set[0xB9] = {&o::LDA, &a::AbsoluteY};
		set[0xA1] = {&o::LDA, &a::IndirectX};
		set[0xB1] = {&o::LDA, &a::IndirectY};
		set[0xA2] = {&o::LDX, &a::Immediate};
		set[0xA6] = {&o::LDX, &a::ZeroPage};
		set[0xB6] = {&o::LDX, &a::ZeroPageY};
		set[0xAE] = {&o::LDX, &a::Absolute};
		set[0xBE] = {&o::LDX, &a::AbsoluteY};
		set[0xA0] = {&o::LDY, &a::Immediate};
		set[0xA4] = {&o::LDY, &a::ZeroPage};
		set[0xB4] = {&o::LDY, &a::ZeroPageX};
		set[0xAC] = {&o::LDY, &a::Absolute};
		set[0xBC] = {&o::LDY, &a::AbsoluteX};
		set[0x4A] = {&o::LSR, &a::Accumulator};
		set[0x46] = {&o::LSR, &a::ZeroPage};
		set[0x56] = {&o::LSR, &a::ZeroPageX};
		set[0x4E] = {&o::LSR, &a::Absolute};
		set[0x5E] = {&o::LSR, &a::AbsoluteX};
		set[0xEA] = {&o::NOP, &a::Implied};
		set[0x49] = {&o::EOR, &a::Immediate};
		set[0x45] = {&o::EOR, &a::ZeroPage};
		set[0x15] = {&o::EOR, &a::ZeroPageX};
		set[0x4D] = {&o::EOR, &a::Absolute};
		set[0x5D] = {&o::EOR, &a::AbsoluteX};
		set[0x59] = {&o::EOR, &a::AbsoluteY};
		set[0x41] = {&o::EOR, &a::IndirectX};
		set[0x51] = {&o::EOR, &a::IndirectY};
		set[0x09] = {&o::ORA, &a::Immediate};
		set[0x05] = {&o::ORA, &a::ZeroPage};
		set[0x15] = {&o::ORA, &a::ZeroPageX};
		set[0x0D] = {&o::ORA, &a::Absolute};
		set[0x1D] = {&o::ORA, &a::AbsoluteX};
		set[0x19] = {&o::ORA, &a::AbsoluteY};
		set[0x01] = {&o::ORA, &a::IndirectX};
		set[0x11] = {&o::ORA, &a::IndirectY};
		set[0x48] = {&o::PHA, &a::Implied};
		set[0x08] = {&o::PHP, &a::Implied};
		set[0x68] = {&o::PLA, &a::Implied};
		set[0x28] = {&o::PLP, &a::Implied};
		set[0x2A] = {&o::ROL, &a::Accumulator};
		set[0x26] = {&o::ROL, &a::ZeroPage};
		set[0x36] = {&o::ROL, &a::ZeroPageX};
		set[0x2E] = {&o::ROL, &a::Absolute};
		set[0x3E] = {&o::ROL, &a::AbsoluteX};
		set[0x6A] = {&o::ROR, &a::Accumulator};
		set[0x66] = {&o::ROR, &a::ZeroPage};
		set[0x76] = {&o::ROR, &a::ZeroPageX};
		set[0x6E] = {&o::ROR, &a::Absolute};
		set[0x7E] = {&o::ROR, &a::AbsoluteX};
		set[0x40] = {&o::RTI, &a::Implied};
		set[0x60] = {&o::RTS, &a::Implied};
		set[0xE9] = {&o::SBC, &a::Immediate};
		set[0xE5] = {&o::SBC, &a::ZeroPage};
		set[0xF5] = {&o::SBC, &a::ZeroPageX};
		set[0xED] = {&o::SBC, &a::Absolute};
		set[0xFD] = {&o::SBC, &a::AbsoluteX};
		set[0xF9] = {&o::SBC, &a::AbsoluteY};
		set[0xE1] = {&o::SBC, &a::IndirectX};
		set[0xF1] = {&o::SBC, &a::IndirectY};
		set[0x38] = {&o::SEC, &a::Implied};
		set[0xF8] = {&o::SED, &a::Implied};
		set[0x78] = {&o::SEI, &a::Implied};
		set[0x85] = {&o::STA, &a::ZeroPage};
		set[0x95] = {&o::STA, &a::ZeroPageX};
		set[0x8D] = {&o::STA, &a::Absolute};
		set[0x9D] = {&o::STA, &a::AbsoluteX};
		set[0x99] = {&o::STA, &a::AbsoluteY};
		set[0x81] = {&o::STA, &a::IndirectX};
		set[0x91] = {&o::STA, &a::IndirectY};
		set[0x86] = {&o::STX, &a::ZeroPage};
		set[0x96] = {&o::STX, &a::ZeroPageY};
		set[0x8E] = {&o::STX, &a::Absolute};
		set[0x84] = {&o::STY, &a::ZeroPage};
		set[0x94] = {&o::STY, &a::ZeroPageX};
		set[0x8C] = {&o::STY, &a::Absolute};
		set[0xAA] = {&o::TAX, &a::Implied};
		set[0xA8] = {&o::TAY, &a::Implied};
		set[0xBA] = {&o::TSX, &a::Implied};
		set[0x8A] = {&o::TXA, &a::Implied};
		set[0x9A] = {&o::TXS, &a::Implied};
		set[0x98] = {&o::TYA, &a::Implied};
		*/
		return set;
	}
}

namespace Nes
{
}
