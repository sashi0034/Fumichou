#include "stdafx.h"
#include "AudioPulseChannel.h"

using namespace Nes;

namespace
{
	constexpr std::array<uint8, 4> dutyRatioTable{2, 4, 8, 12};
}

namespace Nes
{
	void AudioPulseChannel::WriteVolumeDecay(uint8 value)
	{
		m_decayCounter = GetBits<0, 3>(value);
		m_volumeDecayRate = m_decayCounter;
		m_decayEnabled = not GetBits<4>(value);
		m_loopEnabled = GetBits<5>(value);
		m_dutyRatio = dutyRatioTable[GetBits<6, 7>(value)];
	}
}
