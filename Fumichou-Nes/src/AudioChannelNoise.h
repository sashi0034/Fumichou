#pragma once

#include "Forward.h"

namespace Nes
{
	class AudioChannelNoise
	{
	public:
		void WriteControl(uint8 value);
		void WriteFrequency(uint8 value);
		void WriteLength(uint8 value);

	private:
		bool m_enabled{};
		bool m_mode{};
		uint16 m_shiftRegister{};
		bool m_lengthEnabled{};
		uint8 m_lengthValue{};
		uint16 m_timerPeriod{};
		uint16 m_timerValue{};
		bool m_envelopeEnabled{};
		bool m_envelopeLoop{};
		bool m_envelopeStart{};
		uint8 m_envelopePeriod{};
		uint8 m_envelopeValue{};
		uint8 m_envelopeVolume{};
		uint8 m_constantVolume{};
	};
}
