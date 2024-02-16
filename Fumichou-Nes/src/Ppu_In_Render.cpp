#include "stdafx.h"
#include "Ppu_In_Render.h"

using namespace Nes;

namespace
{
	constexpr uint8 tile_8 = 8;
}

class Ppu::In::Renderer::Impl
{
public:
	static void Render(const render_args& args)
	{
		const ScopedRenderStates2D renderStates2D{SamplerState::ClampNearest};

		auto& ppu = args.ppu.get();
		auto& mmu = args.mmu.get();
		const ScopedRenderTarget2D renderTarget2D{ppu.m_video.texture};

		auto& patternTable = args.board.get().PatternTableTexture();

		// BG描画
		constexpr int w_32 = DisplayWidth_256 / tile_8;
		constexpr int h_30 = DisplayHeight_240 / tile_8;
		for (int16 y = 0; y < h_30; ++y)
		{
			for (int16 x = 0; x < w_32; ++x)
			{
				const uint16 addr = x + y * 32;
				const uint8 id = ppu.m_nametableData[addr]; // mmu.ReadChr8(addr);
				(void)patternTable(id * tile_8, 0, tile_8, tile_8).draw(Point{x, y} * tile_8);
			}
		}
	}
};

namespace Nes
{
	void Ppu::In::Renderer::Render(const render_args& args)
	{
		Impl::Render(args);
	}
}
