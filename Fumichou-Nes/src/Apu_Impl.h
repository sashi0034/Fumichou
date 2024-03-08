#pragma once
#include "Apu.h"
#include "AudioPulseChannel.h"

// https://github.com/fogleman/nes/blob/master/nes/apu.go

namespace Nes
{
	class Apu::Impl
	{
	public:
		class Io;

	private:
		AudioPulseChannel m_pulseChannel1{};
		AudioPulseChannel m_pulseChannel2{};
	};
}
