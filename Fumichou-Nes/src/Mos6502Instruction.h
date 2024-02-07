#pragma once
#include "Forward.h"

namespace Nes
{
	class Mmu;

	struct Mos6502Instruction
	{
		using operation_type = uint8(*)(const Hardware& hw);
		using addressing_type = addr16(*)(addr16& pc, const Mmu& mmu);

		operation_type operation{};
		addressing_type addressing{};
	};

	[[nodiscard]]
	const Mos6502Instruction& GetMos6502Instruction(uint8 opcode);

	[[nodiscard]]
	int GetMos6502OperationTakenCycles(uint8 opcode);
}
