#include "stdafx.h"
#include "AudioChannelNoise.h"

#include "AudioTable.h"

using namespace Nes;

namespace
{
	std::array<uint16, 16> noiseTable{4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068};
}

namespace Nes
{
	void AudioChannelNoise::WriteControl(uint8 value)
	{
		m_constantVolume = GetBits<0, 3>(value);
		m_envelopePeriod = m_constantVolume;
		m_envelopeEnabled = not GetBits<4>(value);
		m_envelopeLoop = GetBits<4>(value);
		m_lengthEnabled = not m_envelopeLoop;
		m_envelopeStart = true;
	}

	void AudioChannelNoise::WriteFrequency(uint8 value)
	{
		m_timerPeriod = noiseTable[GetBits<0, 3>(value)];
		m_mode = GetBits<7>(value);
	}

	void AudioChannelNoise::WriteLength(uint8 value)
	{
		m_lengthValue = LengthTable[GetBits<3, 7>(value)];
		m_envelopeStart = true;
	}

	void AudioChannelNoise::Enable(uint8 enable)
	{
		m_enabled = enable;
		if (not m_enabled)
		{
			m_lengthValue = 0;
		}
	}
}
