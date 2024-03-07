#pragma once

namespace Nes
{
	struct DebugParameter
	{
		std::function<void()> watchInterrupt{};
		std::array<std::function<void()>, 0x10000> watchPc{};

		bool spriteVisibility{true};
		bool bgVisibility{true};

		static DebugParameter& Instance();
	};
}
