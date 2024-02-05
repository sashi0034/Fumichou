#include "stdafx.h"
#include "Logger.h"

namespace Nes
{
	void Logger::Abort(const std::source_location& location)
	{
		String message = U"{}\n\tlocation: {} ({}) {}"_fmt(
			message,
			Unicode::Widen(location.file_name()),
			location.line(),
			Unicode::Widen(location.function_name()));
		System::MessageBoxOK(message);
	}
}
