#pragma once
#include "Mmu.h"
#include "StandardController.h"

namespace Nes
{
	class StandardController::In
	{
	public:
		static MappedRead MapReadPrg(const StandardController& self, addr16 addr);
		static MappedWrite MapWritePrg_0x4016(StandardController& self);

	private:
		class Impl;
	};
}
