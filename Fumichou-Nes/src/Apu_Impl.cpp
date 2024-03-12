#include "stdafx.h"
#include "Apu_Impl.h"

#include "Hardware.h"
#include "HardwareConstants.h"

using namespace Nes;

namespace
{
	constexpr uint32 frameCounterFrequency = CpuFrequency_1789773 / 240;
}

class Apu::Impl::Internal
{
public:
	static void StepCycle(Apu_Impl& apu, Mos6502& cpu, const Mmu& mmu)
	{
		apu.m_cycleCount++;

		stepTimer(apu, cpu, mmu);

		if ((apu.m_cycleCount % frameCounterFrequency) == 0)
		{
			// TODO
		}
	}

private:
	static void stepTimer(Apu_Impl& apu, Mos6502& cpu, const Mmu& mmu)
	{
		if (not(apu.m_cycleCount & 1))
		{
			apu.m_pulseChannel1.StepTimer();
			apu.m_pulseChannel2.StepTimer();
			apu.m_noiseChannel.StepTimer();
			apu.m_dmc.StepTimer(cpu, mmu);
		}
		apu.m_triangleChannel.StepTimer();
	}
};

namespace Nes
{
	void Apu::Impl::Step(Hardware& hw, CpuCycle cycle)
	{
		for (int i = 0; i < cycle; ++i)
		{
			Internal::StepCycle(*hw.GetApu().p_impl, hw.GetMos6502(), hw.GetMmu());
		}
	}

	void Apu::Impl::WriteStatus(uint8 value)
	{
		m_pulseChannel1.Enable(GetBits<0>(value));
		m_pulseChannel2.Enable(GetBits<1>(value));
		m_triangleChannel.Enable(GetBits<2>(value));
		m_noiseChannel.Enable(GetBits<3>(value));
		m_dmc.Enable(GetBits<4>(value));
	}
}
