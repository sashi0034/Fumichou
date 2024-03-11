#pragma once
#include "Forward.h"

namespace Nes
{
	constexpr uint32 CpuFrequency_1789773 = 1789773;

	// https://nesdev.org/wiki/Cycle_reference_chart
	constexpr CpuCycle CpuCyclesPerFrame_29781 = 29781; // = 1789773 / 60
}
