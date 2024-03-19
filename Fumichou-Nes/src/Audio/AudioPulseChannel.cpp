#include "stdafx.h"
#include "AudioPulseChannel.h"
#include "AudioContants.h"

using namespace Nes;

namespace
{
	constexpr std::array<std::array<uint8, 8>, 4> dutyTable
	{
		{
			{0, 1, 0, 0, 0, 0, 0, 0},
			{0, 1, 1, 0, 0, 0, 0, 0},
			{0, 1, 1, 1, 1, 0, 0, 0},
			{1, 0, 0, 1, 1, 1, 1, 1},
		}
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
		m_lengthEnabled = not m_envelopeLoop;
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

	void AudioPulseChannel::StepTimer()
	{
		if (m_timerValue == 0)
		{
			m_timerValue = m_timerPeriod;
			m_dutyValue = (m_dutyValue + 1) & 7;
		}
		else
		{
			m_timerValue--;
		}
	}

	void AudioPulseChannel::StepEnvelope()
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

	void AudioPulseChannel::StepSweep()
	{
		if (m_sweepReload)
		{
			if (m_sweepEnabled && m_sweepValue == 0)
			{
				sweep();
			}
			m_sweepValue = m_sweepPeriod;
			m_sweepReload = false;
		}
		else if (m_sweepValue > 0)
		{
			m_sweepValue--;
		}
		else
		{
			if (m_sweepEnabled)
			{
				sweep();
			}
			m_sweepValue = m_sweepPeriod;
		}
	}

	void AudioPulseChannel::StepLength()
	{
		if (m_lengthEnabled && m_lengthValue > 0)
		{
			m_lengthValue--;
		}
	}

	uint8 AudioPulseChannel::Output() const
	{
		if (not m_enabled) return 0;
		if (m_lengthValue == 0) return 0;
		if (dutyTable[m_dutyMode][m_dutyValue] == 0) return 0;
		if (m_timerPeriod < 8 || m_timerPeriod > 0x7FF) return 0;

		if (m_envelopeEnabled) return m_envelopeVolume;
		else return m_constantVolume;
	}

	void AudioPulseChannel::sweep()
	{
		const auto delta = m_timerPeriod >> m_sweepShift;
		if (m_sweepNegate)
		{
			m_timerPeriod -= delta;
			if (m_channel == 1)
			{
				m_timerPeriod--;
			}
		}
		else
		{
			m_timerPeriod += delta;
		}
	}
}
