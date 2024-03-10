#pragma once
#include "Apu.h"
#include "AudioChannelNoise.h"
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

		void WriteStatus(uint8 value);

	private:
		AudioPulseChannel m_pulseChannel1{};
		AudioPulseChannel m_pulseChannel2{};
		AudioTriangleChannel m_triangleChannel{};
		AudioChannelNoise m_channelNoise{};
		AudioDmc m_dmc{};
	};
}
