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
	static void StepCycle(Apu_Impl& apu)
	{
		apu.m_cycleCount++;

		stepTimer(apu);

		if ((apu.m_cycleCount % frameCounterFrequency) == 0)
		{
			// TODO
		}
	}

private:
	static void stepTimer(Apu_Impl& apu)
	{
		// TODO
	}
};

namespace Nes
{
	void Apu::Impl::Step(Hardware& hw, CpuCycle cycle)
	{
		for (int i = 0; i < cycle; ++i)
		{
			Internal::StepCycle(*hw.GetApu().p_impl);
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
