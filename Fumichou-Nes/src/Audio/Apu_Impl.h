#pragma once
#include "Apu.h"
#include "ApuStream.h"
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
		using Apu_Impl = Apu::Impl;
		class Io;

		Impl();
		void Reset();
		void Step(Mos6502& cpu, const Mmu& mmu, CpuCycle cycle);

		const std::shared_ptr<ApuStream>& GetStream() const { return m_stream; }

		void WriteStatus(uint8 value);
		void WriteFrameCounter(uint8 value);

	private:
		std::shared_ptr<ApuStream> m_stream{};
		CpuCycle m_cycleCount{};
		AudioPulseChannel m_pulseChannel1{1};
		AudioPulseChannel m_pulseChannel2{2};
		AudioTriangleChannel m_triangleChannel{};
		AudioNoiseChannel m_noiseChannel{};
		AudioDmc m_dmc{};
		uint8 m_framePeriod{};
		uint8 m_frameValue{};
		uint8 m_frameIrq{};

		class Internal;
	};
}
