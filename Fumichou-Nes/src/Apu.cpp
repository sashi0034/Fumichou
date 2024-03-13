#include "stdafx.h"
#include "Apu.h"

#include "Audio/Apu_Impl.h"

namespace Nes
{
	Apu::Apu() :
		p_impl(std::make_shared<Impl>())
	{
	}
}
