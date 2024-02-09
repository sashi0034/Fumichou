#pragma once

namespace Nes
{
	namespace LogReader
	{
		struct TraceData
		{
			StringView tag;
			String message;
		};

		int GetTraceSize();
		TraceData GetTraceData(int index);
	};
}
