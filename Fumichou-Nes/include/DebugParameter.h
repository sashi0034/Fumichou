#pragma once

namespace Nes
{
	struct DebugParameter
	{
		std::function<void()> watchInterrupt{};

		bool spriteVisibility{true};
		bool bgVisibility{true};

		static DebugParameter& Instance();
	};
}
