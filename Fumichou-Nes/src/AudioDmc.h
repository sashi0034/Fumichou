#pragma once

#include "Forward.h"

namespace Nes
{
	class AudioDmc
	{
	public:
		void WriteControl(uint8 value);
		void WriteValue(uint8 value);
		void WriteAddress(uint8 value);
		void WriteLength(uint8 value);

		void Enable(uint8 enabled);

	private:
		bool m_enabled{};
		uint8 m_value{};
		uint16 m_sampleAddress{};
		uint16 m_sampleLength{};
		uint16 m_currentAddress{};
		uint16 m_currentLength{};
		uint8 m_shiftRegister{};
		uint8 m_bitCount{};
		uint8 m_tickPeriod{};
		uint8 m_tickValue{};
		bool m_loop{};
		bool m_irq{};
	};
}
