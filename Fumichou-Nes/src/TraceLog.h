#pragma once

#include "Forward.h"

namespace Nes
{
	struct TraceMemoryRead
	{
		addr16 addr;
		uint8 read;

		String Stringify() const;
	};

	struct TraceMemoryWrite
	{
		addr16 addr;
		uint8 wrote;

		String Stringify() const;
	};

	struct TraceCpuOperation
	{
		addr16 pc;
		uint8 opcode;

		String Stringify() const;
	};

	using TraceLog = std::variant<TraceMemoryRead, TraceMemoryWrite, TraceCpuOperation>;
}
