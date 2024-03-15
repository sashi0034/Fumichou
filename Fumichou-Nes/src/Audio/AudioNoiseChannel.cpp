#include "stdafx.h"
#include "AudioNoiseChannel.h"

#include "AudioTable.h"

using namespace Nes;

namespace
{
	std::array<uint16, 16> noiseTable{4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068};
}

namespace Nes
{
	void AudioNoiseChannel::WriteControl(uint8 value)
	{
		m_constantVolume = GetBits<0, 3>(value);
		m_envelopePeriod = m_constantVolume;
		m_envelopeEnabled = not GetBits<4>(value);
		m_envelopeLoop = GetBits<4>(value);
		m_lengthEnabled = not m_envelopeLoop;
		m_envelopeStart = true;
	}

	void AudioNoiseChannel::WriteFrequency(uint8 value)
	{
		m_timerPeriod = noiseTable[GetBits<0, 3>(value)];
		m_mode = GetBits<7>(value);
	}

	void AudioNoiseChannel::WriteLength(uint8 value)
	{
		m_lengthValue = LengthTable[GetBits<3, 7>(value)];
		m_envelopeStart = true;
	}

	void AudioNoiseChannel::Enable(uint8 enable)
	{
		m_enabled = enable;
		if (not m_enabled)
		{
			m_lengthValue = 0;
		}
	}

	void AudioNoiseChannel::StepTimer()
	{
		if (m_timerValue == 0)
		{
			m_timerValue = m_timerPeriod;
			const uint8 shift = m_mode ? 6 : 1;
			const auto b1 = m_shiftRegister & 1;
			const auto b2 = (m_shiftRegister >> shift) & 1;
			m_shiftRegister >>= 1;
			m_shiftRegister |= (b1 ^ b2) << 14;
		}
		else
		{
			m_timerValue--;
		}
	}

	void AudioNoiseChannel::StepEnvelop()
	{
		if (m_envelopeStart)
		{
			m_envelopeVolume = 15;
			m_envelopeValue = m_envelopePeriod;
			m_envelopeStart = false;
		}
		else if (m_envelopeValue > 0)
		{
			m_envelopeValue--;
		}
		else
		{
			if (m_envelopeVolume > 0)
			{
				m_envelopeVolume--;
			}
			else if (m_envelopeLoop)
			{
				m_envelopeVolume = 15;
			}
			m_envelopeValue = m_envelopePeriod;
		}
	}
}
