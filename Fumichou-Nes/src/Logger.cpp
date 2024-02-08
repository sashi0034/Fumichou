#include "stdafx.h"
#include "Logger.h"

namespace Nes
{
	void Logger::Error(StringView message)
	{
		std::cout << "[Error] " << message << "\n";
	}

	void Logger::Abort(const std::source_location& location)
	{
		const String message = U"Aborted at {} ({}) {}"_fmt(
			Unicode::Widen(location.file_name()),
			location.line(),
			Unicode::Widen(location.function_name()));
		System::MessageBoxOK(message, MessageBoxStyle::Error);
	}
}
