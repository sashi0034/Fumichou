#pragma once
#include "Mos6502.h"

namespace Nes
{
	class Mos6502::In
	{
	public:
		static void Reset(Hardware& hw);
		static CycleCount Step(Hardware& hw);

		class Op;

	private:
		class Impl;
	};
}
