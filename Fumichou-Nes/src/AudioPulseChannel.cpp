#include "stdafx.h"
#include "AudioPulseChannel.h"

using namespace Nes;

namespace
{
	constexpr std::array<uint8, 32> lengthTable{
		10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
		12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30,
	};
}

namespace Nes
{
	void AudioPulseChannel::WriteControl(uint8 value)
	{
		m_envelopePeriod = GetBits<0, 3>(value);
		m_constantVolume = m_envelopePeriod;
		m_envelopeEnabled = not GetBits<4>(value);
		m_envelopeLoop = GetBits<5>(value);
		m_lengthEnabled = not GetBits<5>(value);
		m_dutyMode = GetBits<6, 7>(value);
		m_envelopeStart = true;
	}

	void AudioPulseChannel::WriteSweep(uint8 value)
	{
		m_sweepShift = GetBits<0, 2>(value);
		m_sweepNegate = GetBits<3>(value);
		m_sweepPeriod = GetBits<4, 6>(value) + 1;
		m_sweepEnabled = GetBits<7>(value);
		m_sweepReload = true;
	}

	void AudioPulseChannel::WriteFrequency(uint8 value)
	{
		m_timerPeriod = SetBits<0, 7>(m_timerPeriod, value);
	}

	void AudioPulseChannel::WriteLength(uint8 value)
	{
		m_timerPeriod = SetBits<8, 15>(m_timerPeriod, value);
		m_lengthValue = lengthTable[GetBits<3, 7>(value)];
		m_envelopeStart = true;
		m_dutyValue = 0;
	}
}
