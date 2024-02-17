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
		Float4 paletteColors[64];
		uint32 paletteIndexes[4 * 2];
	};

	struct CbBgData
	{
		uint32 patternTableSize[4];
		uint32 nametable[4 * 256];
	};
}

class Ppu::In::Renderer::Impl
{
public:
	static void Render(const render_args& args)
	{
		auto& ppu = args.ppu.get();
		auto& mmu = args.mmu.get();

		const ScopedCustomShader2D shader{PixelShaderAsset(ShaderKeys::bg_render)};

		const ScopedRenderStates2D renderStates2D{SamplerState::ClampNearest};

		const ScopedRenderTarget2D renderTarget2D{ppu.m_video.texture};

		// パレット登録
		static ConstantBuffer<CbPaletteColors> cbPaletteColors = []()
		{
			ConstantBuffer<CbPaletteColors> cb{};
			for (int i = 0; i < PaletteColors.size(); ++i) cb->paletteColors[i] = PaletteColors[i].toFloat4();
			return cb;
		}();
		std::memcpy(&cbPaletteColors->paletteIndexes, ppu.m_palettes.data(), sizeof(cbPaletteColors->paletteIndexes));
		Graphics2D::SetPSConstantBuffer(1, cbPaletteColors);

		// PPUパレットのミラー領域を埋める
		applyPaletteMirror(ppu);

		// BG描画
		static ConstantBuffer<CbBgData> cbBgData{};

		// static TimeProfiler profiler{U"PPU Rendering"};
		// profiler.begin(U"BG");

		auto& patternTable = args.board.get().PatternTableTexture();
		cbBgData->patternTableSize[0] = patternTable.width();
		cbBgData->patternTableSize[1] = patternTable.height();
		std::memcpy(&cbBgData->nametable, ppu.m_nametableData.data(), sizeof(cbBgData->nametable));

		Graphics2D::SetPSConstantBuffer(2, cbBgData);
		(void)patternTable.resized(Display_256x240).draw();

		// profiler.end(U"BG");
		// profiler.console();
	}
};

namespace Nes
{
	void Ppu::In::Renderer::Render(const render_args& args)
	{
		Impl::Render(args);
	}
}
