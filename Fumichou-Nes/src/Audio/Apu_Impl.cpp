#include "stdafx.h"
#include "Apu_Impl.h"

#include "Hardware.h"
#include "HardwareConstants.h"
#include "Mos6502_In.h"

using namespace Nes;

namespace
{
	constexpr uint32 frameCounterFrequency_7457 = CpuFrequency_1789773 / 240;
}

class Apu::Impl::Internal
{
public:
	static void StepCycle(Apu_Impl& apu, Mos6502& cpu, const Mmu& mmu)
	{
		apu.m_cycleCount++;

		stepTimer(apu, cpu, mmu);

		if ((apu.m_cycleCount % frameCounterFrequency_7457) == 0)
		{
			StepFrameCounter(apu, cpu);
		}

		// TODO サンプリング
	}

	static void StepEnvelop(Apu_Impl& apu)
	{
		apu.m_pulseChannel1.StepEnvelope();
		apu.m_pulseChannel2.StepEnvelope();
		apu.m_triangleChannel.StepCounter();
		apu.m_noiseChannel.StepEnvelop();
	}

	static void StepSweep(Apu_Impl& apu)
	{
		apu.m_pulseChannel1.StepSweep();
		apu.m_pulseChannel2.StepSweep();
	}

	static void StepLength(Apu_Impl& apu)
	{
		apu.m_pulseChannel1.StepLength();
		apu.m_pulseChannel2.StepLength();
		apu.m_triangleChannel.StepLength();
		apu.m_noiseChannel.StepLength();
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

	static void StepFrameCounter(Apu_Impl& apu, Mos6502& cpu)
	{
		if (apu.m_framePeriod == 4)
		{
			apu.m_frameValue = (apu.m_frameValue + 1) & 3;
			switch (apu.m_frameValue)
			{
			case 0: [[fallthrough]];
			case 2:
				StepEnvelop(apu);
				break;
			case 1:
				StepEnvelop(apu);
				StepSweep(apu);
				StepLength(apu);
				break;
			case 3:
				StepEnvelop(apu);
				StepSweep(apu);
				StepLength(apu);
				if (apu.m_frameIrq) Mos6502::In::InvokeIrq(cpu);
				break;
			default: assert(false);
			}
		}
		else // 5
		{
			apu.m_frameValue = (apu.m_frameValue + 1) % 5;
			switch (apu.m_frameValue)
			{
			case 0: [[fallthrough]];
			case 2:
				StepEnvelop(apu);
				break;
			case 1: [[fallthrough]];
			case 3:
				StepEnvelop(apu);
				StepSweep(apu);
				StepLength(apu);
				break;
			default: break;
			}
		}
	}
};

namespace Nes
{
	void Apu::Impl::Reset()
	{
		m_framePeriod = 4;
	}

	void Apu::Impl::Step(Mos6502& cpu, const Mmu& mmu, CpuCycle cycle)
	{
		for (int i = 0; i < cycle; ++i)
		{
			Internal::StepCycle(*this, cpu, mmu);
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

	void Apu::Impl::WriteFrameCounter(uint8 value)
	{
		m_framePeriod = 4 + GetBits<7>(value);
		m_frameIrq = GetBits<6>(value);
		if (m_framePeriod == 5)
		{
			Internal::StepEnvelop(*this);
			Internal::StepSweep(*this);
			Internal::StepLength(*this);
		}
	}
}
