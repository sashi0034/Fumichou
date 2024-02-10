#include "stdafx.h"
#include "TraceLog.h"

#include "Mos6502Instruction.h"

namespace Nes
{
	String TraceMemoryRead::Stringify() const
	{
		return U"{:04X} -> {:02X}"_fmt(addr, read);
	}

	StringView TraceMemoryRead::Tag()
	{
		return U"RD";
	}

	String TraceMemoryWrite::Stringify() const
	{
		return U"{:04X} <- {:02X}"_fmt(addr, wrote);
	}

	StringView TraceMemoryWrite::Tag()
	{
		return U"ST";
	}

	String TraceCpuOperation::Stringify() const
	{
		return U"{:04X} | {}"_fmt(pc, GetMos6502Instruction(opcode).desc);
	}

	StringView TraceCpuOperation::Tag()
	{
		return U"OP";
	}
}
