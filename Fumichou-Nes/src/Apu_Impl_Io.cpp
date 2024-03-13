#include "stdafx.h"
#include "Apu_Impl_Io.h"

#include "Apu_Impl.h"
#include "Hardware.h"

namespace Nes
{
	// https://d1.amobbs.com/bbs_upload782111/files_28/ourdev_551332.pdf

	// MappedRead Apu::In::Io::MapReadPrg(const Hardware& hw, addr16 addr)
	// {
	// 	return MappedRead::Invalid(MappingType::Cpu);
	// }

	MappedWrite Apu::Impl::Io::MapWritePrg(Hardware& hw, addr16 addr)
	{
		auto apu = hw.GetApu().p_impl.get();
		switch (addr)
		{
		case 0x4000:
			return MappedWrite{
				.desc = U"APU Pulse 1 Control"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_pulseChannel1.WriteControl(value);
				}
			};
		case 0x4001:
			return MappedWrite{
				.desc = U"APU Pulse 1 Sweep"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_pulseChannel1.WriteSweep(value);
				}
			};
		case 0x4002:
			return MappedWrite{
				.desc = U"APU Pulse 1 Frequency"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_pulseChannel1.WriteFrequency(value);
				}
			};
		case 0x4003:
			return MappedWrite{
				.desc = U"APU Pulse 1 Length"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_pulseChannel1.WriteLength(value);
				}
			};
		case 0x4004:
			return MappedWrite{
				.desc = U"APU Pulse 2 Control"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_pulseChannel2.WriteControl(value);
				}
			};
		case 0x4005:
			return MappedWrite{
				.desc = U"APU Pulse 2 Sweep"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_pulseChannel2.WriteSweep(value);
				}
			};
		case 0x4006:
			return MappedWrite{
				.desc = U"APU Pulse 2 Frequency"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_pulseChannel2.WriteFrequency(value);
				}
			};
		case 0x4007:
			return MappedWrite{
				.desc = U"APU Pulse 2 Length"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_pulseChannel2.WriteLength(value);
				}
			};
		case 0x4008:
			return MappedWrite{
				.desc = U"APU Traiangle Control"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_triangleChannel.WriteControl(value);
				}
			};
		case 0x4009:
			return MappedWrite::None();
		case 0x400A:
			return MappedWrite{
				.desc = U"APU Traiangle Frequency"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_triangleChannel.WriteFrequency(value);
				}
			};
		case 0x400B:
			return MappedWrite{
				.desc = U"APU Traiangle Length"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_triangleChannel.WriteLength(value);
				}
			};
		case 0x400C:
			return MappedWrite{
				.desc = U"APU Noise Control"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_noiseChannel.WriteLength(value);
				}
			};
		case 0x400D:
			return MappedWrite::None();
		case 0x400E:
			return MappedWrite{
				.desc = U"APU Noise Frequency"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_noiseChannel.WriteFrequency(value);
				}
			};
		case 0x400F:
			return MappedWrite{
				.desc = U"APU Noise Length"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_noiseChannel.WriteLength(value);
				}
			};
		case 0x4010:
			return MappedWrite{
				.desc = U"APU DMC Control"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_dmc.WriteControl(value);
				}
			};
		case 0x4011:
			return MappedWrite{
				.desc = U"APU DMC Value"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_dmc.WriteValue(value);
				}
			};
		case 0x4012:
			return MappedWrite{
				.desc = U"APU DMC Address"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_dmc.WriteAddress(value);
				}
			};
		case 0x4013:
			return MappedWrite{
				.desc = U"APU DMC Length"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.m_dmc.WriteLength(value);
				}
			};
		// case 0x4014: // OAM DMA
		case 0x4015:
			return MappedWrite{
				.desc = U"APU Status"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.WriteStatus(value);
				}
			};
		// case 0x4016: // Standard Controller Input
		case 0x4017:
			return MappedWrite{
				.desc = U"APU Status"_sv,
				.ctx = &apu,
				.func = [](void* ctx, addr16, uint8 value)
				{
					auto& apu = *static_cast<Apu_Impl*>(ctx);
					apu.WriteFrameCounter(value);
				}
			};
		default: break;
		}
		return MappedWrite::Invalid(MappingType::Cpu);
	}
}
