#pragma once
#include "Mmu.h"

namespace Nes
{
	class Mmu::In
	{
	public:
		static void MapWholeAddr(Hardware& hw);

	private:
		struct Impl;
	};
}
