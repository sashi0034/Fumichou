#pragma once

namespace Nes
{
	class Logger
	{
	public:
		static void Error(StringView message);
		static void Abort(const std::source_location& location = std::source_location::current());
	};
}
