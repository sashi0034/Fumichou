#pragma once

namespace Nes
{
	struct DebugParameter
	{
		bool spriteVisibility{true};
		bool bgVisibility{true};

		static DebugParameter& Instance();
	};
}
