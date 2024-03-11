#pragma once
#include "Apu.h"
#include "AudioNoiseChannel.h"
#include "AudioDmc.h"
#include "AudioPulseChannel.h"
#include "AudioTriangleChannel.h"

// https://github.com/fogleman/nes/blob/master/nes/apu.go

namespace Nes
{
	class Apu::Impl
	{
	public:
		class Io;

		static void Step(Hardware& hw, CpuCycle cycle);

		void WriteStatus(uint8 value);

	private:
		CpuCycle m_cycleCount{};
		AudioPulseChannel m_pulseChannel1{};
		AudioPulseChannel m_pulseChannel2{};
		AudioTriangleChannel m_triangleChannel{};
		AudioNoiseChannel m_noiseChannel{};
		AudioDmc m_dmc{};

		class Internal;
	};

	using Apu_Impl = Apu::Impl;
}
