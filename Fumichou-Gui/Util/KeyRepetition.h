#pragma once

namespace Util
{
	[[nodiscard]] bool IsKeyRepeating(Input input, double repeatDelay = 0.5, double repeatInterval = 0.1);

	void RegisterKeyRepetitionAddon();
}
