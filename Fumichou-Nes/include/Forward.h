#pragma once
#include <iso646.h>
#include "Util.h"

namespace Nes
{
	using namespace Util;

	using sint8 = int8;
	using sint16 = int16;
	using sint32 = int32;
	using sint64 = int64;

	using addr16 = uint16;

	using CpuCycle = int;
	using PpuCycle = int;

	class Hardware;

	constexpr uint32 AddrSize_0x10000 = 0x10000;

	constexpr uint32 DisplayWidth_256 = 256;
	constexpr uint32 DisplayHeight_240 = 240;
	constexpr Size Display_256x240{DisplayWidth_256, DisplayHeight_240};

	class EmulationAbort final : public Error
	{
	public:
		using Error::Error;
		[[nodiscard]] StringView type() const noexcept override { return U"EmulationAbort"; }
	};
}
