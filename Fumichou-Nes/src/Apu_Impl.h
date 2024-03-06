#pragma once
#include "Apu.h"
#include "AudioPulseChannel.h"

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
