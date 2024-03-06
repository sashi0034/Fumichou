#pragma once
#include "Forward.h"

namespace Nes
{
	class AudioPulseChannel
	{
	public:
		void WriteVolumeDecay(uint8 value);

	private:
		uint8 m_decayCounter{};
		uint8 m_volumeDecayRate{};
		bool m_decayEnabled{};
		bool m_loopEnabled{};
		uint8 m_dutyRatio{};
	};
}
