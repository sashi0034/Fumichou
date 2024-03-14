#include "stdafx.h"
#include "Ppu_In.h"

#include "Hardware.h"
#include "Logger.h"
#include "Mos6502_In.h"
#include "Ppu_Renderer.h"

using namespace Nes;

namespace
{
	constexpr PpuCycle cyclesPerLine_341 = 341;
	constexpr int maxLines_262 = 262;

	constexpr uint8 tile_8 = 8;
}

class Ppu::In::Impl
{
public:
	static void Step(Hardware& hw, PpuCycle cycle)
	{
		auto& ppu = hw.GetPpu();
		ppu.m_lineCycles += cycle;

		if (ppu.m_scanningSprZero && ppu.m_oam.sprites[0].x > ppu.m_lineCycles)
		{
			// スプライト0ヒットをチェック
			checkSprZeroHit(hw, ppu);
			ppu.m_scanningSprZero = false;
		}

		if (ppu.m_lineCycles >= cyclesPerLine_341)
		{
			reachedLineEnd(hw, ppu, ppu.m_scanLine);

			ppu.m_lineCycles -= cyclesPerLine_341;
			ppu.m_scanLine = (ppu.m_scanLine + 1) % maxLines_262;
		}
	}

	static void RenderVideo(Hardware& hw)
	{
		auto& ppu = hw.GetPpu();
		ppu.m_renderer->Render({
			.ppu = ppu,
			.mmu = hw.GetMmu(),
			.board = hw.GetCartridge().GetBoard()
		});
	}

private:
	static void applyScrollPos(Ppu& ppu, uint32 line)
	{
		// スクロール位置登録
		const auto tableX = ppu.m_regs.tempAddr.NameTableAddrX() ? DisplayWidth_256 : 0;
		const auto tableY = ppu.m_regs.tempAddr.NameTableAddrY() ? DisplayHeight_240 : 0;

		ppu.m_fixedScroll = ppu.m_scrollBuffer + ScrollPoint(tableX, tableY);
		ppu.m_renderer->SetScrollPos(line, ppu.m_fixedScroll);
	}

	static void applyTilePageOffsets(Hardware& hw, Ppu& ppu)
	{
		// 64タイルを4ページ、つまり256タイル分のタイルのオフセット登録
		for (int i = 0; i < ppu.m_tilePageOffsets.size(); ++i)
		{
			ppu.m_tilePageOffsets[i] = hw.GetCartridge().GetBoard().TilePageOffsets()[i];
		}
	}

	static void reachedLineEnd(Hardware& hw, Ppu& ppu, uint32 line)
	{
		if (line < 240)
		{
			if (line < 239) applyScrollPos(ppu, line + 1);

			if (ppu.m_scanningSprZero)
			{
				// DMAのタイミングで、前回のスプライト0ヒットが残ってしまうかもしれないので一応チェック
				checkSprZeroHit(hw, ppu);
			}

			// スプライト0ヒットを検出するラインかチェック
			const auto spr0 = ppu.m_oam.sprites[0];
			ppu.m_scanningSprZero = not ppu.m_unstable.status.SprZeroHit() && spr0.y <= line && line <= spr0.y + 7;
		}
		else if (line == 240)
		{
			// 垂直同期
			beginVerticalBlank(hw, ppu);
			ppu.m_scanningSprZero = false;
		}
		else if (line == 241)
		{
			// 垂直同期時にNMIが発生すると「ソロモンの鍵」が動かないようなので数クロック送らせる
			if (ppu.m_regs.control.NmiEnabled())
			{
				// NMI割り込み
				Mos6502::In::FireNmi(hw.GetMos6502());
			}
		}
		else if (line == 260)
		{
			// 本来は261ライン最初で行う?
			ppu.m_unstable.status.VBlank().Set(false);
			ppu.m_unstable.status.SprZeroHit().Set(false);
		}
		else if (line == 261)
		{
			applyScrollPos(ppu, 0);
			applyTilePageOffsets(hw, ppu);
		}
	}

	static void checkSprZeroHit(Hardware& hw, Ppu& ppu)
	{
		// TODO: 8x16の対応?

		const auto scanLine = ppu.m_scanLine;

		auto&& spr0 = ppu.m_oam.sprites[0];
		const auto secondSpPattern = PpuControl8(ppu.m_regs.control).SecondSprPatter() * 4;
		const auto spr0Tile = ppu.m_tilePageOffsets[secondSpPattern | (spr0.tile >> 6)] | (spr0.tile & 0x3F);

		// R, G にパターンデータを格納している
		auto&& patternTable = hw.GetCartridge().GetBoard().PatternTableImage();

		const auto scrollPos = s3d::Point(ppu.m_fixedScroll.x, ppu.m_fixedScroll.y);
		const auto secondBgPattern = PpuControl8(ppu.m_regs.control).SecondBgPattern() * 4;
		const auto& tilePageOffsets = ppu.m_tilePageOffsets;

		// FIXME: 用検証

		for (uint8 x = 0; x < 8; ++x)
		{
			// スプライトチェック
			const auto& sprPixel = patternTable[s3d::Point(spr0Tile * tile_8 + x, scanLine - (spr0.y + 1))];
			const bool sprYes = sprPixel.r || sprPixel.g;
			if (sprYes == false) continue;

			// BGチェック
			// bg_render.hlsl を見ながら書いています
			const auto screenPos0 = s3d::Point(spr0.x + x, scanLine) + scrollPos;
			const auto screenPos = screenPos0 % (2 * Display_256x240);
			const auto tileCoarse = screenPos / tile_8;
			const auto tileFine = screenPos - tileCoarse * tile_8;

			const uint32 crossPageX = ((tileCoarse.x) >> 5) * (0x400 - 32); // Correct if X is greater than 32
			const uint32 crossPageY = ((tileCoarse.y + 2) >> 5) * (0x400 + 64); // Correct if Y is greater than 30
			const auto addr = tileCoarse.x + tileCoarse.y * 32 + crossPageX + crossPageY;
			const uint32 tileId0 = ppu.m_nametableData[(addr & 0x3FF) + ppu.m_nametableOffset[addr >> 10]];
			const uint32 tileId = tilePageOffsets[secondBgPattern | (tileId0 >> 6)] | (tileId0 & 0x3F);
			const auto tileUV = s3d::Point(tileId * tile_8, 0) + tileFine;
			const auto bgPixel = patternTable[tileUV];
			const bool bgYes = bgPixel.r || bgPixel.g;
			if (bgYes == false) continue;

			// ヒット
			ppu.m_unstable.status.SprZeroHit().Set(true);
			break;
		}
	}

	static void beginVerticalBlank(Hardware& hw, Ppu& ppu)
	{
		// 垂直同期フラグ
		ppu.m_unstable.status.VBlank().Set(true);

		// ディスプレイ描画
		RenderVideo(hw);
	}
};

namespace Nes
{
	void Ppu::In::Step(Hardware& hw, PpuCycle cycle)
	{
		Impl::Step(hw, cycle);
	}

	void Ppu::In::RenderVideo(Hardware& hw)
	{
		Impl::RenderVideo(hw);
	}

	// https://www.nesdev.org/wiki/Mirroring
	void Ppu::In::UpdateMirroring(Ppu& ppu, NameTableMirror mirror)
	{
		ppu.m_mirroring = mirror;
		switch (mirror)
		{
		case NameTableMirror::Horizontal:
			ppu.m_nametableOffset = {0x000, 0x000, 0x400, 0x400};
			break;
		case NameTableMirror::Vertical:
			ppu.m_nametableOffset = {0x000, 0x400, 0x000, 0x400};
			break;
		case NameTableMirror::SingleLower:
			ppu.m_nametableOffset = {0x000, 0x000, 0x000, 0x000};
			break;
		case NameTableMirror::SingleHigher:
			ppu.m_nametableOffset = {0x400, 0x400, 0x400, 0x400};
			break;
		case NameTableMirror::FourScreen:
			ppu.m_nametableOffset = {0x000, 0x400, 0x800, 0xC00};
			break;
		default: ;
			Logger::Abort();
		}
	}

	uint8 Ppu::In::readPalette(const Ppu& ppu, uint8 paletteIndex)
	{
		// 0x0010, 0x0014, 0x0018, 0x001C はミラー
		if ((paletteIndex & 0x10) && (paletteIndex & 0x3) == 0)
		{
			paletteIndex &= 0xF;
		}
		return ppu.m_palettes[paletteIndex];
	}

	void Ppu::In::writePalette(Ppu& ppu, uint8 paletteIndex, uint8 value)
	{
		// 0x0010, 0x0014, 0x0018, 0x001C はミラー
		if ((paletteIndex & 0x10) && (paletteIndex & 0x3) == 0)
		{
			paletteIndex &= 0xF;
		}
		ppu.m_palettes[paletteIndex] = value;
	}
}
