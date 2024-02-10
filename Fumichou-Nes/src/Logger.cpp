#include "stdafx.h"
#include "Logger.h"

#include "LogReader.h"
#include "TraceLog.h"

namespace
{
	using namespace Nes;

	constexpr int TraceBufferSize = 65536;

	class TraceBuffer
	{
	public:
		TraceBuffer()
		{
			m_buffer.fill(TraceEmpty());
		}

		void Push(const TraceLogType& trace)
		{
			m_buffer[m_head] = trace;
			m_head = (m_head + 1) % TraceBufferSize;
		}

		const TraceLogType& Get(int index) const
		{
			return m_buffer[(m_head - 1 - index + TraceBufferSize) % TraceBufferSize];
		}

	private:
		std::array<TraceLogType, TraceBufferSize> m_buffer{};
		int m_head{};
	};

	TraceBuffer s_traceBuffer{};
}

namespace Nes
{
	void Logger::Trace(const TraceLogType& trace)
	{
		s_traceBuffer.Push(trace);
	}

	void Logger::Error(StringView message)
	{
		std::cout << "[Error] " << message << "\n";
	}

	void Logger::Abort(StringView message)
	{
		// System::MessageBoxOK(message, MessageBoxStyle::Error);
		throw EmulationAbort(message);
	}

	void Logger::Abort(const std::source_location& location)
	{
		const String message = U"Aborted at {} ({}) {}"_fmt(
			Unicode::Widen(location.file_name()),
			location.line(),
			Unicode::Widen(location.function_name()));
		Abort(message);
	}

	int LogReader::GetTraceSize()
	{
		return TraceBufferSize;
	}

	LogReader::TraceData LogReader::GetTraceData(int index)
	{
		auto&& traceData = s_traceBuffer.Get(index);
		return {
			.tag = std::visit([&](auto&& arg) { return arg.Tag(); }, traceData),
			.message = std::visit([&](auto&& arg) { return arg.Stringify(); }, traceData)
		};
	}
}
