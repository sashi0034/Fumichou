#pragma once

#include "TraceLog.h"

namespace Nes
{
	class Logger
	{
	public:
		static void Trace(const TraceLogType& trace);
		static void Error(StringView message);
		static void Abort(StringView message);
		static void Abort(const std::source_location& location = std::source_location::current());
	};
}
