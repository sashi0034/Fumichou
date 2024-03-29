﻿#pragma once
#include "Forward.h"
#include "Mos6502Forward.h"

namespace Nes
{
	class Mmu;

	struct Mos6502Instruction
	{
		using operation_type = void(*)(const Mos6502OpArgs& args);
		using addressing_type = addr16(*)(const Mos6502AddressingArgs& args);

		operation_type operation{};
		addressing_type addressing{};
		StringView desc{};
	};

	[[nodiscard]]
	const Mos6502Instruction& GetMos6502Instruction(uint8 opcode);

	[[nodiscard]]
	CpuCycle GetMos6502OperationCycleCount(uint8 opcode);
}
