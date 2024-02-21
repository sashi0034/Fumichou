#include "stdafx.h"
#include "Apu_In_Io.h"

namespace Nes
{
	// MappedRead Apu::In::Io::MapReadPrg(const Hardware& hw, addr16 addr)
	// {
	// 	return MappedRead::Invalid(MappingType::Cpu);
	// }

	MappedWrite Apu::In::Io::MapWritePrg(Hardware& hw, addr16 addr)
	{
		switch (addr)
		{
		case 0x4000:
		case 0x4001:
		case 0x4002:
		case 0x4003:
		case 0x4004:
		case 0x4005:
		case 0x4006:
		case 0x4007:
		case 0x4008:
		case 0x4009:
		case 0x400A:
		case 0x400B:
		case 0x400C:
		case 0x400D:
		case 0x400E:
		case 0x400F:
		case 0x4010:
		case 0x4011:
		case 0x4012:
		case 0x4013:
		// case 0x4014: // OAM DMA
		case 0x4015:
			return MappedWrite{
				.desc = U"APU (TODO)"_sv,
				.ctx = &hw,
				.func = [](void* ctx, addr16, uint8 value)
				{
				}
			};
		default: break;
		}
		return MappedWrite::Invalid(MappingType::Cpu);
	}
}
