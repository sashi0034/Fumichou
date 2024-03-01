#include "stdafx.h"
#include "RenderSpriteWireframe.h"

#include "FontKeys.h"
#include "Forward.h"
#include "HwFrame.h"
#include "Util/TomlStyleSheet.h"

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"OtherFunctions." + key);
	}
}

namespace Details
{
	void RenderSpriteWireframe(const RectF& displayRect)
	{
		const ScopedRenderStates2D renderStates2D{SamplerState::Default2D};
		const Transformer2D transformer2D{
			Mat3x2::Translate(displayRect.tl())
			.scaled(displayRect.size / Nes::Display_256x240, displayRect.tl())
		};

		auto&& ppu = Nes::HwFrame::Instance().GetHw().GetPpu();
		auto&& oam = ppu.GetRenderedSprites();
		constexpr double wireThickness = 0.5;

		// ワイヤーフレーム描画
		const int spriteHeight = Nes::PpuControl8(ppu.Regs().control).LongSprite() ? 16 : 8;
		for (auto&& spr : oam)
		{
			(void)Rect(spr.x, spr.y + 1, 8, spriteHeight)
			      .stretched(-wireThickness * 1)
			      .drawFrame(wireThickness * 2, Palette::Black);
		}

		const auto green = Palette::Lawngreen.lerp(Palette::Lime, Periodic::Sine0_1(0.5s));
		for (int i = 0; i < oam.size(); ++i)
		{
			auto&& spr = oam[i];
			(void)Rect(spr.x, spr.y + 1, 8, spriteHeight)
			      .stretched(-wireThickness * 1)
			      .drawFrame(wireThickness * 1, green);
		}

		// 番号描画
		const auto textColor = Palette::Lawngreen.lerp(Palette::Orange, Periodic::Sine0_1(0.5s));
		const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
		for (int i = 0; i < oam.size(); ++i)
		{
			auto&& spr = oam[i];
			(void)font(U"{:02X}"_fmt(i)).drawAt(6.0, Rect(spr.x, spr.y + 1, 8, spriteHeight).center(), textColor);
		}
	}
}
