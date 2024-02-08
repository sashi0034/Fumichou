#pragma once
#include "Forward.h"
#include "Mos6502Forward.h"

namespace Nes
{
	class Mmu;

	struct Mos6502Instruction
	{
		using operation_type = uint8(*)(Hardware& hw);
		using addressing_type = addr16(*)(const Mos6502AddressingArgs& args);

		operation_type operation{};
		addressing_type addressing{};
	};

	[[nodiscard]]
	const Mos6502Instruction& GetMos6502Instruction(uint8 opcode);

	[[nodiscard]]
	int GetMos6502OperationTakenCycles(uint8 opcode);
}
