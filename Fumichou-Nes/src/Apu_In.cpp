#include "stdafx.h"
#include "Apu_In.h"

#include "Audio/Apu_Impl.h"
#include "Audio/Apu_Impl_Io.h"
#include "Hardware.h"

namespace Nes
{
	MappedWrite Apu::In::MapWritePrg(Hardware& hw, addr16 addr)
	{
		return Impl::Io::MapWritePrg(hw, addr);
	}

	void Apu::In::Reset(Apu& apu)
	{
		apu.p_impl->Reset();
	}

	void Apu::In::Step(Hardware& hw, CpuCycle cycle)
	{
		hw.GetApu().p_impl->Step(hw.GetMos6502(), hw.GetMmu(), cycle);
	}
}
