#include "stdafx.h"
#include "AudioTriangleChannel.h"

#include "AudioForward.h"

namespace
{
	constexpr std::array<uint8_t, 32> triangleTable
	{
		15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	};
}

namespace Nes
{
	void AudioTriangleChannel::WriteControl(uint8 value)
	{
		m_counterPeriod = GetBits<0, 6>(value);
		m_lengthEnabled = not GetBits<7>(value);
	}

	void AudioTriangleChannel::WriteFrequency(uint8 value)
	{
		m_timerPeriod = SetBits<0, 7>(m_timerPeriod, value);
	}

	void AudioTriangleChannel::WriteLength(uint8 value)
	{
		m_timerPeriod = SetBits<8, 15>(m_timerPeriod, value & 7);
		m_timerValue = m_timerPeriod;
		m_lengthValue = LengthTable[GetBits<3, 7>(value)];
		m_counterReload = true;
	}

	void AudioTriangleChannel::Enable(uint8 enable)
	{
		m_enabled = enable;
		if (not enable)
		{
			m_lengthValue = 0;
		}
	}

	void AudioTriangleChannel::StepTimer()
	{
		if (m_timerValue == 0)
		{
			m_timerValue = m_timerPeriod;
			if (m_lengthValue > 0 && m_counterValue > 0)
			{
				m_dutyValue = (m_dutyValue + 1) & 0x1F;
			}
		}
		else
		{
			m_timerValue--;
		}
	}

	void AudioTriangleChannel::StepCounter()
	{
		if (m_counterReload)
		{
			m_counterValue = m_counterPeriod;
		}
		else if (m_counterValue > 0)
		{
			m_counterValue--;
		}

		if (m_lengthEnabled)
		{
			m_counterReload = false;
		}
	}

	void AudioTriangleChannel::StepLength()
	{
		if (m_lengthEnabled && m_lengthValue > 0)
		{
			m_lengthValue--;
		}
	}

	uint8 AudioTriangleChannel::Output() const
	{
		if (not m_enabled) return 0;
		if (m_timerPeriod < 3) return 0;
		if (m_lengthValue == 0) return 0;
		if (m_counterValue == 0) return 0;
		return triangleTable[m_dutyValue];
	}
}
