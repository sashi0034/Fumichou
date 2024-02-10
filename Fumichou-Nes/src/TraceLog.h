#pragma once

#include "Forward.h"

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

	using TraceLogType = std::variant<
		TraceEmpty,
		TraceMemoryRead,
		TraceMemoryWrite,
		TraceCpuOperation
	>;

	constexpr size_t TraceLogSize = sizeof(TraceLogType);

	// class TraceLogTag
	// {
	// public:
	// 	StringView operator()(const TraceMemoryRead&) const { return U"RD"; }
	// 	StringView operator()(const TraceMemoryWrite&) const { return U"ST"; }
	// 	StringView operator()(const TraceCpuOperation&) const { return U"OP"; }
	// };
}
