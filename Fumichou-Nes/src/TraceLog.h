#pragma once

#include "Forward.h"
#include "Mos6502.h"

namespace Nes
{
	struct TraceEmpty
	{
		static String Stringify() { return {}; }
		static StringView Tag() { return U""; }
	};

	struct TraceMemoryRead
	{
		addr16 addr;
		uint8 read;

		String Stringify() const;
		static StringView Tag();
	};

	struct TraceMemoryWrite
	{
		addr16 addr;
		uint8 wrote;

		String Stringify() const;
		static StringView Tag();
	};

	struct TraceCpuOperation
	{
		addr16 pc;
		uint8 opcode;

		String Stringify() const;
		static StringView Tag();
	};

	struct TraceCpuInterrupt
	{
		addr16 pc;
		InterruptKind interrupt;

		String Stringify() const;
		static StringView Tag();
	};

	using TraceLogType = std::variant<
		TraceEmpty,
		TraceMemoryRead,
		TraceMemoryWrite,
		TraceCpuOperation,
		TraceCpuInterrupt
	>;

	constexpr size_t TraceLogSize = sizeof(TraceLogType);
}
