#include "stdafx.h"
#include "Ppu.h"

#include "Ppu_Renderer.h"

using namespace Nes;

namespace Nes
{
	Ppu::Ppu() :
		m_renderer(IRenderer::Create())
	{
	}

	const s3d::Texture& Ppu::GetVideo() const
	{
		return m_renderer->GetVideoTexture();
	}

	const std::array<OamData, 64>& Ppu::GetRenderedSprites() const
	{
		return m_renderer->GetRenderedSprites();
	}
}
