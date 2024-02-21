#include "stdafx.h"
#include "Ppu_Renderer.h"

#include "BoardBase.h"
#include "PaletteColors.h"
#include "Ppu_In.h"
#include "ShaderKeys.h"

using namespace Nes;

namespace
{
	constexpr uint8 tile_8 = 8;

	struct CbPaletteColors
	{
		s3d::Float4 paletteColors[64];
		uint32 paletteIndexes[4 * 2];
	};

	struct CbBgData
	{
		struct
		{
			uint32 patternTableSize[2];
			uint32 pageOffset;
			uint32 padding_0x60[1];
		} ppu;

		uint32 nametable[4 * 256];
	};
}

class Ppu::IRenderer::Hle : public IRenderer
{
public:
	s3d::RenderTexture m_videoTexture{Display_256x240};
	s3d::ConstantBuffer<CbPaletteColors> cbPaletteColors{};
	s3d::ConstantBuffer<CbBgData> cbBgData{};

	Hle()
	{
		for (int i = 0; i < PaletteColors.size(); ++i)
		{
			cbPaletteColors->paletteColors[i] = PaletteColors[i].toFloat4();
		}
	}

	~Hle() override = default;

	const s3d::Texture& GetVideoTexture() const override
	{
		return m_videoTexture;
	}

	void Render(const render_args& args) override
	{
		auto& ppu = args.ppu.get();

		const s3d::ScopedRenderStates2D renderStates2D{s3d::SamplerState::ClampNearest};

		const s3d::ScopedRenderTarget2D renderTarget2D{m_videoTexture};

		// パレット登録
		std::memcpy(&cbPaletteColors->paletteIndexes, ppu.m_palettes.data(), sizeof(cbPaletteColors->paletteIndexes));
		s3d::Graphics2D::SetPSConstantBuffer(1, cbPaletteColors);

		// BG描画
		renderBg(args, ppu);

		// スプライト描画
		renderSprites(args, ppu);
	}

private:
	void renderBg(const render_args& args, const Ppu& ppu)
	{
		const s3d::ScopedCustomShader2D shader{s3d::PixelShaderAsset(ShaderKeys::bg_render)};

		// static TimeProfiler profiler{U"PPU Rendering"};
		// profiler.begin(U"BG");

		cbBgData->ppu.pageOffset =  PpuControl8(ppu.m_regs.control).SecondBgPattern() << 8;

		auto& patternTable = args.board.get().PatternTableTexture();
		cbBgData->ppu.patternTableSize[0] = patternTable.width();
		cbBgData->ppu.patternTableSize[1] = patternTable.height();
		std::memcpy(&cbBgData->nametable, ppu.m_nametableData.data(), sizeof(cbBgData->nametable));

		s3d::Graphics2D::SetPSConstantBuffer(2, cbBgData);
		(void)patternTable.resized(Display_256x240).draw();

		// profiler.end(U"BG");
		// profiler.console();
	}

	void renderSprites(const render_args& args, const Ppu& ppu)
	{
		const s3d::ScopedCustomShader2D shader{s3d::PixelShaderAsset(ShaderKeys::sprite_render)};
		auto& patternTable = args.board.get().PatternTableTexture();

		const uint16 sprPage = PpuControl8(ppu.m_regs.control).SecondSprPatter() << 8;

		// アドレス降順から描画開始
		for (int i = 63; i >= 0; --i)
		{
			auto&& spr = ppu.m_oam.sprites[i];
			const s3d::Point pos = s3d::Point(spr.x, spr.y + 1);
			const uint8 paletteIdBase = 0x10 | (GetBits<0, 1>(spr.attribute) << 2);
			const bool behindBg = GetBits<5>(spr.attribute); // TODO
			const bool flipH = GetBits<6>(spr.attribute);
			const bool flipV = GetBits<7>(spr.attribute);

			// RにパレットIDを渡して描画
			patternTable(s3d::Rect(s3d::Point(sprPage | spr.tile, 0) * tile_8, tile_8, tile_8))
				.mirrored(flipH)
				.flipped(flipV)
				.draw(pos, s3d::ColorF(paletteIdBase, 0, 0));
		}
	}
};

namespace Nes
{
	std::unique_ptr<Ppu::IRenderer> Ppu::IRenderer::Create()
	{
		return std::make_unique<Hle>();
	}
}
