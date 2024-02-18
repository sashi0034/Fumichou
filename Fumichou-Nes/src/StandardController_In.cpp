#include "stdafx.h"
#include "StandardController_In.h"

#include "Logger.h"

using namespace Nes;

class StandardController::In::Impl
{
public:
	static void Input(const StandardController& self, int player)
	{
		for (uint8 button = 0; button < 8; ++button)
		{
			self.m_states[player] |= (self.m_controllers[player][button].pressed() << player);
		}
	}

	static uint8 Output(const StandardController& self, int player)
	{
		uint8 result = 0x40;
		if (self.m_strobe)
		{
			result |= static_cast<uint8>(self.m_controllers[player][0].pressed());
		}
		else
		{
			result |= self.m_states[player] & 1;
			self.m_states[player] >>= 1;
		}
		return result;
	}
};

namespace Nes
{
	MappedRead StandardController::In::MapReadPrg(const StandardController& self, addr16 addr)
	{
		if (not AddrRange<addr16>(0x4016, 0x4017).IsBetween(addr)) Logger::Abort();

		return MappedRead{
			.desc = U"Standard Controller Output"_sv,
			.ctx = &self,
			.func = [](const void* ctx, addr16 addr)
			{
				auto& self = *static_cast<const StandardController*>(ctx);
				const int player = addr & 1;
				return Impl::Output(self, player);
			}
		};
	}

	MappedWrite StandardController::In::MapWritePrg_0x4016(StandardController& self)
	{
		return MappedWrite{
			.desc = U"Standard Controller Input"_sv,
			.ctx = &self,
			.func = [](void* ctx, addr16, uint8 value)
			{
				auto& self = *static_cast<StandardController*>(ctx);
				self.m_strobe = value & 1;
				if (not self.m_strobe)
				{
					Impl::Input(self, 0);
					Impl::Input(self, 1);
				}
			}
		};
	}
}
