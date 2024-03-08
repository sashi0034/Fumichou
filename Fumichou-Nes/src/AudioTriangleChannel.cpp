#include "stdafx.h"
#include "AudioTriangleChannel.h"

#include "AudioTable.h"

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
		m_timerPeriod = SetBits<8, 15>(m_timerPeriod, value);
		m_timerValue = m_timerPeriod;
		m_lengthValue = LengthTable[GetBits<3, 7>(value)];
		m_counterReload = true;
	}
}
