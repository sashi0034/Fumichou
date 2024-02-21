#include "stdafx.h"
#include "Ppu_In.h"

#include <Siv3D/Console.hpp>

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

		if (ppu.m_sprZeroScan && ppu.m_oam.sprites[0].x > ppu.m_lineCycles)
		{
			// スプライト0ヒットをチェック
			checkSprZeroHit(hw, ppu);
			ppu.m_sprZeroScan = false;
		}

		if (ppu.m_lineCycles >= cyclesPerLine_341)
		{
			reachedLineEnd(hw, ppu, ppu.m_scanLine);

			ppu.m_lineCycles -= cycle;
			ppu.m_scanLine = (ppu.m_scanLine + 1) % maxLines_262;
		}
	}

private:
	static void reachedLineEnd(Hardware& hw, Ppu& ppu, uint32 line)
	{
		if (line < 240)
		{
			if (ppu.m_sprZeroScan)
			{
				// DMAのときが重なったら、前回のフラグが残ってしまうかもしれないので一応チェック
				checkSprZeroHit(hw, ppu);
			}
			else
			{
				ppu.m_unstable.status.SprZeroHit().Set(false);
			}

			// スプライト0ヒットを検出するラインかチェック
			const auto spr0 = ppu.m_oam.sprites[0];
			ppu.m_sprZeroScan = spr0.y <= line && line <= spr0.y + 7;
		}
		else if (line == 240)
		{
			// 垂直同期
			beginVerticalBlank(hw, ppu);
			ppu.m_sprZeroScan = false;
		}
	}

	static void checkSprZeroHit(Hardware& hw, Ppu& ppu)
	{
		// TODO: 8x16の対応?

		auto&& spr0 = ppu.m_oam.sprites[0];
		const auto scanLine = ppu.m_scanLine;

		// R, G にパターンデータを格納している
		auto&& patternTable = hw.GetCartridge().GetBoard().PatternTableImage();

		const uint16 bgPageOffset = ppu.m_regs.control.SecondBgPattern() << 8;

		// TODO
		const auto scrollPos = s3d::Point(
			ppu.m_regs.fineX | (ppu.m_unstable.vramAddr.CoarseX() << 3),
			ppu.m_unstable.vramAddr.FineY() | (ppu.m_unstable.vramAddr.CoarseY() << 3));

		for (uint8 x = 0; x < 8; ++x)
		{
			// スプライトチェック
			const auto& sprPixel = patternTable[s3d::Point(spr0.tile * tile_8 + x, scanLine - (spr0.y + 1))];
			const bool sprYes = sprPixel.r || sprPixel.g;
			if (sprYes == false) continue;

			// BGチェック
			// TODO: スクロール対応
			const auto screenPos = s3d::Point(spr0.x + x, scanLine);
			const auto tileCoarse = screenPos / tile_8;
			const auto tileFine = screenPos - tileCoarse * tile_8;

			const auto addr = tileCoarse.x + tileCoarse.y * 32;
			const uint32 tileId = bgPageOffset + ppu.m_nametableData[(addr & 0x3FF) + ppu.m_nametableOffset[addr >> 8]];
			const auto tileUV = s3d::Point(tileId * tile_8, 0) + tileFine;
			const auto bgPixel = patternTable[tileUV];
			const bool bgYes = bgPixel.r || bgPixel.g;
			if (bgYes == false) continue;

			// ヒット
			ppu.m_unstable.status.SprZeroHit().Set(true);
			// s3d::Console.writeln(U"Hit at: " + s3d::Format(screenPos));
			break;
		}
	}

	static void beginVerticalBlank(Hardware& hw, Ppu& ppu)
	{
		// 垂直同期フラグ
		ppu.m_unstable.status.VBlank().Set(true);
		if (ppu.m_regs.control.NmiEnabled())
		{
			// NMI割り込み
			Mos6502::In::RequestNmi(hw.GetMos6502());
		}

		// 描画用にPPUパレットのミラー領域を埋める
		ApplyPaletteMirror(ppu);

		// ディスプレイ描画
		ppu.m_renderer->Render({
			.ppu = hw.GetPpu(),
			.mmu = hw.GetMmu(),
			.board = hw.GetCartridge().GetBoard()
		});
	}
};

namespace Nes
{
	void Ppu::In::Step(Hardware& hw, PpuCycle cycle)
	{
		Impl::Step(hw, cycle);
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

	void Ppu::In::ApplyPaletteMirror(Ppu& ppu)
	{
		constexpr uint16 mirror = 0x0010;
		ppu.m_palettes[0x0010] = ppu.m_palettes[0x0010 - mirror];
		ppu.m_palettes[0x0014] = ppu.m_palettes[0x0014 - mirror];
		ppu.m_palettes[0x0018] = ppu.m_palettes[0x0018 - mirror];
		ppu.m_palettes[0x001C] = ppu.m_palettes[0x001C - mirror];
	}

	uint8 Ppu::In::readPalette(const Ppu& ppu, uint8 paletteIndex)
	{
		// TODO: 最適化
		if (paletteIndex == 0x0010 || paletteIndex == 0x0014 || paletteIndex == 0x0018 || paletteIndex == 0x001C)
		{
			paletteIndex -= 0x0010;
		}
		return ppu.m_palettes[paletteIndex];
	}

	void Ppu::In::writePalette(Ppu& ppu, uint8 paletteIndex, uint8 value)
	{
		// TODO: 最適化
		if (paletteIndex == 0x0010 || paletteIndex == 0x0014 || paletteIndex == 0x0018 || paletteIndex == 0x001C)
		{
			paletteIndex -= 0x0010;
		}
		ppu.m_palettes[paletteIndex] = value;
	}
}
