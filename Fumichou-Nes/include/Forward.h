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

	class Hardware;

	constexpr uint32 AddrSize_0x10000 = 0x10000;
}
