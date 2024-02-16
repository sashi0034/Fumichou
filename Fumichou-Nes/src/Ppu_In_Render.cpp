#include "stdafx.h"
#include "Ppu_In_Render.h"

#include "PaletteColors.h"
#include "ShaderKeys.h"

using namespace Nes;

namespace
{
	constexpr uint8 tile_8 = 8;

	struct CbPaletteColors
	{
		Float4 colors[64];
	};

	struct CbPaletteIndex
	{
		uint32 palette[4];
	};
}

class Ppu::In::Renderer::Impl
{
public:
	static void Render(const render_args& args)
	{
		static ConstantBuffer<CbPaletteColors> cbPaletteColors = []()
		{
			ConstantBuffer<CbPaletteColors> cb{};
			for (int i = 0; i < PaletteColors.size(); ++i) cb->colors[i] = PaletteColors[i].toFloat4();
			return cb;
		}();
		Graphics2D::SetPSConstantBuffer(1, cbPaletteColors);
		const ScopedCustomShader2D shader{PixelShaderAsset(PixelShaderKeys::bg_render)};

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
				const uint8 attrIndex = 0x3C0 | (addr & 0xC00) | ((addr >> 4) & 0x38) | ((addr >> 2) & 0x7);
				const uint8 attribute = ppu.m_nametableData[attrIndex];
				const uint8 shift = ((addr >> 4) & 4) | (addr & 2);
				const uint8 paletteIndex = ((attribute >> shift) & 0x3) << 2;
				ConstantBuffer<CbPaletteIndex> cb{};
				cb->palette[0] = readPalette(ppu, paletteIndex + 0);
				cb->palette[1] = readPalette(ppu, paletteIndex + 1);
				cb->palette[2] = readPalette(ppu, paletteIndex + 2);
				cb->palette[3] = readPalette(ppu, paletteIndex + 3);
				Graphics2D::SetPSConstantBuffer(2, cb);
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
