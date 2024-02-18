#include "stdafx.h"
#include "Ppu.h"

using namespace Nes;

namespace Nes
{
	Ppu::Ppu()
	{
		m_video.texture = s3d::RenderTexture(Display_256x240);
	}
}
