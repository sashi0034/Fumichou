#pragma once

#include "Forward.h"

namespace Nes
{
	class AudioTriangleChannel
	{
	public:
		void WriteControl(uint8 value);
		void WriteFrequency(uint8 value);
		void WriteLength(uint8 value);

		void Enable(uint8 enable);

	private:
		bool m_enabled{};
		bool m_lengthEnabled{};
		uint8 m_lengthValue{};
		uint16 m_timerPeriod{};
		uint16 m_timerValue{};
		uint8 m_dutyValue{};
		uint8 m_counterPeriod{};
		uint8 m_counterValue{};
		bool m_counterReload{};
	};
}
