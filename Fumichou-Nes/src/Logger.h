#pragma once

namespace Nes
{
	class Logger
	{
	public:
		static void Abort(const std::source_location& location = std::source_location::current());
	};
}
