#include "stdafx.h"
#include "TraceLog.h"

#include "Mos6502Instruction.h"

namespace Nes
{
	String TraceMemoryRead::Stringify() const
	{
		return U"${} -> {}"_fmt(addr, read);
	}

	String TraceMemoryWrite::Stringify() const
	{
		return U"${} -> {}"_fmt(addr, wrote);
	}

	String TraceCpuOperation::Stringify() const
	{
		return String(GetMos6502Instruction(opcode).desc);
	}
}
