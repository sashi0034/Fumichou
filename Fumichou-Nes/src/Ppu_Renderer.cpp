﻿#include "stdafx.h"
#include "Ppu_Renderer.h"

#include "BoardBase.h"
#include "DebugParameter.h"
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

		union
		{
			uint32 words[4 * 2];
			uint8 bytes[32];
		} paletteIndexes;
	};

	struct CbBgData
	{
		uint32 patternTableSize[4];
		uint32 tilePageOffsets[4];

		// スキャンラインごとにスクロール位置の決定
		union
		{
			uint32 words[4 * 30];
			uint16 shorts[DisplayHeight_240];
		} scrollX;

		union
		{
			uint32 words[4 * 30];
			uint16 shorts[DisplayHeight_240];
		} scrollY;

		uint32 nametable[4 * 256];
	};
}

class Ppu::IRenderer::HleRenderer : public IRenderer
{
public:
	s3d::RenderTexture m_bgRenderBuffer{Display_256x240};
	s3d::RenderTexture m_videoTexture{Display_256x240};
	s3d::ConstantBuffer<CbPaletteColors> m_cbPaletteColors{};
	s3d::ConstantBuffer<CbBgData> m_cbBgData{};
	std::array<OamData, 64> m_renderedSprites{};

	HleRenderer()
	{
		for (int i = 0; i < PaletteColors.size(); ++i)
		{
			m_cbPaletteColors->paletteColors[i] = PaletteColors[i].toFloat4();
		}
	}

	~HleRenderer() override = default;

	const s3d::Texture& GetVideoTexture() const override
	{
		return m_videoTexture;
	}

	void SetScrollPos(uint32 scanline, const ScrollPoint& scroll) override
	{
		m_cbBgData->scrollX.shorts[scanline] = scroll.x;
		m_cbBgData->scrollY.shorts[scanline] = scroll.y;
	}

	const std::array<OamData, 64>& GetRenderedSprites() const override
	{
		return m_renderedSprites;
	}

	void Render(const render_args& args) override
	{
		auto& ppu = args.ppu.get();

		const s3d::Transformer2D transformer2D{s3d::Mat3x2::Identity(), s3d::Transformer2D::Target::SetLocal};
		const s3d::ScopedViewport2D viewport2D{s3d::Rect(Display_256x240)};
		const s3d::ScopedRenderStates2D renderStates2D{s3d::SamplerState::ClampNearest};

		// パレット登録
		applyPalettes(ppu);
		s3d::Graphics2D::SetPSConstantBuffer(1, m_cbPaletteColors);

		// BGバッファへ描画
		renderBg(args, ppu);

		// メインターゲットへ描画設定
		if (DebugParameter::Instance().bgVisibility)m_videoTexture.clear(PaletteColors[ppu.m_palettes[0]]);
		else m_videoTexture.clear(s3d::Color(16));
		const s3d::ScopedRenderTarget2D renderTarget2D{m_videoTexture};

		// BGバッファの反映
		(void)m_bgRenderBuffer.draw();

		// スプライト描画
		renderSprites(args, ppu);

		// デバッグ用にデータ格納
		std::memcpy(&m_renderedSprites, &ppu.m_oam, sizeof(ppu.m_oam));
	}

private:
	void applyPalettes(const Ppu& ppu)
	{
		std::memcpy(
			&m_cbPaletteColors->paletteIndexes, ppu.m_palettes.data(), sizeof(m_cbPaletteColors->paletteIndexes));

		// 透明色反映
		m_cbPaletteColors->paletteIndexes.bytes[0x04] = ppu.m_palettes[0x00];
		m_cbPaletteColors->paletteIndexes.bytes[0x08] = ppu.m_palettes[0x00];
		m_cbPaletteColors->paletteIndexes.bytes[0x0C] = ppu.m_palettes[0x00];

		// ミラー反映
		m_cbPaletteColors->paletteIndexes.bytes[0x10] = ppu.m_palettes[0x00];
		m_cbPaletteColors->paletteIndexes.bytes[0x14] = ppu.m_palettes[0x04];
		m_cbPaletteColors->paletteIndexes.bytes[0x18] = ppu.m_palettes[0x08];
		m_cbPaletteColors->paletteIndexes.bytes[0x1C] = ppu.m_palettes[0x0C];
	}

	void renderBg(const render_args& args, const Ppu& ppu)
	{
		const s3d::ScopedRenderTarget2D renderTarget2D{m_bgRenderBuffer.clear(s3d::Color(0, 0))};
		if (not DebugParameter::Instance().bgVisibility) return;
		if (not PpuMask8(ppu.m_regs.mask).ShowBackground()) return;

		// アルファ上書き
		using s3d::Blend;
		using s3d::BlendOp;
		const s3d::ScopedRenderStates2D rs{
			s3d::BlendState{
				true, Blend::One, Blend::Zero, BlendOp::Add, Blend::One, Blend::Zero, BlendOp::Max
			}
		};

		const s3d::ScopedCustomShader2D shader{s3d::PixelShaderAsset(ShaderKeys::bg_render)};

		const auto secondBgPattern = PpuControl8(ppu.m_regs.control).SecondBgPattern() * 4;
		for (int i = 0; i < 4; ++i)
		{
			m_cbBgData->tilePageOffsets[i] = ppu.m_tilePageOffsets[secondBgPattern | i];
		}

		auto& patternTable = args.board.get().PatternTableTexture();
		m_cbBgData->patternTableSize[0] = patternTable.width();
		m_cbBgData->patternTableSize[1] = patternTable.height();

		// ネームテーブル転送
		constexpr int tableCount_4 = 4;
		constexpr size_t tableQuarter_0x400 = sizeof(m_cbBgData->nametable) / tableCount_4;
		for (uint8 i = 0; i < tableCount_4; ++i)
		{
			std::memcpy(
				m_cbBgData->nametable + i * (tableQuarter_0x400 / sizeof(m_cbBgData->nametable)[0]),
				ppu.m_nametableData.data() + ppu.m_nametableOffset[i],
				sizeof(m_cbBgData->nametable) / tableCount_4);
		}

		s3d::Graphics2D::SetPSConstantBuffer(2, m_cbBgData);
		(void)patternTable.resized(Display_256x240).draw();
	}

	void renderSprites(const render_args& args, const Ppu& ppu)
	{
		if (not DebugParameter::Instance().spriteVisibility) return;
		if (not PpuMask8(ppu.m_regs.mask).ShowSprite()) return;

		const s3d::ScopedCustomShader2D shader{s3d::PixelShaderAsset(ShaderKeys::sprite_render)};

		s3d::Graphics2D::SetPSTexture(1, m_bgRenderBuffer);

		auto& patternTable = args.board.get().PatternTableTexture();
		const uint16 sprPage = PpuControl8(ppu.m_regs.control).SecondSprPatter() * 4;

		// アドレス降順から描画開始
		const bool longSprite = PpuControl8(ppu.m_regs.control).LongSprite();
		for (int i = 63; i >= 0; --i)
		{
			auto&& spr = ppu.m_oam.sprites[i];
			const s3d::Point pos = s3d::Point(spr.x, spr.y + 1);
			const uint8 paletteIdBase = 0x10 | (GetBits<0, 1>(spr.attribute) << 2);
			const bool behindBg = GetBits<5>(spr.attribute); // TODO
			const bool flipH = GetBits<6>(spr.attribute);
			const bool flipV = GetBits<7>(spr.attribute);
			const auto tileId = ppu.m_tilePageOffsets[sprPage | (spr.tile >> 6)] | (spr.tile & 0x3F);
			const auto colorCode = s3d::ColorF(paletteIdBase, behindBg, 0);

			// RにパレットIDを渡して描画
			if (longSprite)
			{
				// 8x16 描画
				auto id0 = (tileId) & ~0b1;
				auto id1 = (tileId | 0b1);
				if (flipV) std::swap(id0, id1);
				(void)patternTable(s3d::Rect(s3d::Point(id0, 0) * tile_8, tile_8, tile_8))
				      .mirrored(flipH)
				      .flipped(flipV)
				      .draw(pos, colorCode);
				(void)patternTable(s3d::Rect(s3d::Point(id1, 0) * tile_8, tile_8, tile_8))
				      .mirrored(flipH)
				      .flipped(flipV)
				      .draw(pos.movedBy(0, tile_8), colorCode);
			}
			else
			{
				// 8x8 描画
				(void)patternTable(s3d::Rect(s3d::Point(tileId, 0) * tile_8, tile_8, tile_8))
				      .mirrored(flipH)
				      .flipped(flipV)
				      .draw(pos, colorCode);
			}
		}
	}
};

namespace Nes
{
	std::unique_ptr<Ppu::IRenderer> Ppu::IRenderer::Create()
	{
		return std::make_unique<HleRenderer>();
	}
}
