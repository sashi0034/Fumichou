#include "stdafx.h"
#include "Apu_Impl.h"

namespace Nes
{
	void Apu::Impl::WriteStatus(uint8 value)
	{
		m_pulseChannel1.Enable(GetBits<0>(value));
		m_pulseChannel2.Enable(GetBits<1>(value));
		m_triangleChannel.Enable(GetBits<2>(value));
		m_channelNoise.Enable(GetBits<3>(value));
		m_dmc.Enable(GetBits<4>(value));
	}
}
