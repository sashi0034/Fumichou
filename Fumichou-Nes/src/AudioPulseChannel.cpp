#include "stdafx.h"
#include "AudioPulseChannel.h"
#include "AudioTable.h"

using namespace Nes;

namespace
{
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
		m_lengthValue = LengthTable[GetBits<3, 7>(value)];
		m_envelopeStart = true;
		m_dutyValue = 0;
	}

	void AudioPulseChannel::Enable(bool enabled)
	{
		m_enabled = enabled;
		if (not enabled)
		{
			m_lengthValue = 0;
		}
	}
}
