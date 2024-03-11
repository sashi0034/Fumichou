﻿#include "stdafx.h"
#include "AudioDmc.h"

#include "Mmu.h"

using namespace Nes;

namespace
{
	std::array<s3d::uint16, 16> dmcTable = {214, 190, 170, 160, 143, 127, 113, 107, 95, 80, 71, 64, 53, 42, 36, 27,};
}

class AudioDmc::Impl
{
public:
	static void restart(AudioDmc& dmc)
	{
		dmc.m_currentAddress = dmc.m_sampleAddress;
		dmc.m_currentLength = dmc.m_sampleLength;
	}

	static void stepReader(AudioDmc& dmc, const Mmu& mmu)
	{
		if (dmc.m_currentLength > 0 && dmc.m_bitCount == 0)
		{
			// FIXME: CPUに遅延発生?
			dmc.m_shiftRegister = mmu.ReadPrg8(dmc.m_currentAddress);
			dmc.m_bitCount = 8;
			dmc.m_currentAddress++;
			if (dmc.m_currentAddress == 0)
			{
				dmc.m_currentAddress = 0x8000;
			}
			dmc.m_currentLength--;
			if (dmc.m_currentLength == 0 && dmc.m_loop)
			{
				restart(dmc);
			}
		}
	}
};

namespace Nes
{
	void AudioDmc::WriteControl(uint8 value)
	{
		m_tickPeriod = dmcTable[GetBits<0, 3>(value)];
		m_loop = GetBits<6>(value);
		m_irq = GetBits<7>(value);
	}

	void AudioDmc::WriteValue(uint8 value)
	{
		m_value = GetBits<0, 6>(value);
	}

	void AudioDmc::WriteAddress(uint8 value)
	{
		// Sample Address = %11AAAAAA.AA000000
		m_sampleAddress = 0xC000 | (static_cast<uint16>(value) << 6);
	}

	void AudioDmc::WriteLength(uint8 value)
	{
		// Sample Length = %0000LLLL.LLLL0001
		m_sampleLength = (static_cast<uint16>(value) << 4) | 1;
	}

	void AudioDmc::Enable(uint8 enabled)
	{
		m_enabled = enabled;
		if (enabled)
		{
			if (m_currentLength == 0)
			{
				m_currentAddress = m_sampleAddress;
				m_currentLength = m_sampleLength;
			}
		}
		else
		{
			m_currentLength = 0;
		}
	}

	void AudioDmc::StepTimer()
	{
		if (not m_enabled) return;

		// TODO
	}
}
