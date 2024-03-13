#pragma once
#include "Apu.h"
#include "Audio/AudioNoiseChannel.h"
#include "Audio/AudioDmc.h"
#include "Audio/AudioPulseChannel.h"
#include "Audio/AudioTriangleChannel.h"

// https://github.com/fogleman/nes/blob/master/nes/apu.go

namespace Nes
{
	class Apu::Impl
	{
	public:
		class Io;

		static void Step(Hardware& hw, CpuCycle cycle);

		void WriteStatus(uint8 value);
		void WriteFrameCounter(uint8 value);

	private:
		CpuCycle m_cycleCount{};
		AudioPulseChannel m_pulseChannel1{};
		AudioPulseChannel m_pulseChannel2{};
		AudioTriangleChannel m_triangleChannel{};
		AudioNoiseChannel m_noiseChannel{};
		AudioDmc m_dmc{};
		uint8 m_framePeriod{};
		uint8 m_frameValue{};
		uint8 m_frameIrq{};

		class Internal;
	};

	using Apu_Impl = Apu::Impl;
}
