﻿#pragma once
#include "Forward.h"

namespace Nes
{
	class AudioPulseChannel
	{
	public:
		AudioPulseChannel() = default;
		AudioPulseChannel(uint8 channel);

		void WriteControl(uint8 value);
		void WriteSweep(uint8 value);
		void WriteFrequency(uint8 value);
		void WriteLength(uint8 value);

		void Enable(bool enabled);
		void StepTimer();
		void StepEnvelope();
		void StepSweep();
		void StepLength();

		uint8 Output() const;

	private:
		void sweep();

		bool m_enabled{};
		uint8 m_channel{};
		bool m_lengthEnabled{};
		uint8 m_lengthValue{};
		uint16 m_timerPeriod{};
		uint16 m_timerValue{};
		uint8 m_dutyMode{};
		uint8 m_dutyValue{};
		bool m_sweepReload{};
		bool m_sweepEnabled{};
		bool m_sweepNegate{};
		uint8 m_sweepShift{};
		uint8 m_sweepPeriod{};
		uint8 m_sweepValue{};
		bool m_envelopeEnabled{};
		bool m_envelopeLoop{};
		bool m_envelopeStart{};
		uint8 m_envelopePeriod{};
		uint8 m_envelopeValue{};
		uint8 m_envelopeVolume{};
		uint8 m_constantVolume{};
	};
}
