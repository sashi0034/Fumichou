#include "stdafx.h"
#include "UiScreenOverlay.h"

#include "FontKeys.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"UiScreenOverlay." + key);
	}

	constexpr int modeCount_2 = 2;
}

struct UiScreenOverlay::Impl
{
	Point m_previousCursorPos{};
	double m_stoppingTime{};

	void Update(const update_args& args)
	{
		if (not args.screenRect.intersects(Cursor::Pos()))
		{
			m_stoppingTime = 0;
			return;
		}

		// カーソルを止めているか判定
		if (Cursor::Pos() == m_previousCursorPos && not MouseL.pressed())
		{
			m_stoppingTime += Scene::DeltaTime();
		}
		else
		{
			m_stoppingTime = 0;
			m_previousCursorPos = Cursor::Pos();
		}

		// カーソルが止まっていたらフェードアウトする
		constexpr double fadeStart = 1.5;
		constexpr double fadeEnd = 2.0;
		if (m_stoppingTime > fadeEnd) return;
		const ScopedColorMul2D colorMul2D{
			ColorF(1, 1 - Math::Clamp(m_stoppingTime - fadeStart, 0, 1) / (fadeEnd - fadeStart))
		};

		const int iconSize = getToml<int>(U"iconSize");
		const int iconMargin = getToml<int>(U"iconMargin");

		auto&& iconFont = FontAsset(FontKeys::Icon_40_Msdf);

		// 左上アイコン
		if (args.showModeChange)
		{
			const auto tlPoint = args.screenRect.tl().movedBy(Vec2::One() * (iconMargin + iconSize / 2));
			const auto tlRect =
				RectF(Arg::center = tlPoint, SizeF{1.0, 0.8} * iconSize).rounded(8).draw(ColorF{0.1, 0.5});
			const bool tlHover = tlRect.rect.intersects(Cursor::Pos());
			if (tlHover)(void)tlRect.drawFrame(1, Palette::White);
			(void)iconFont(U"\U000F1002").drawAt(iconSize * 0.8, tlPoint, ColorF(0.9));

			// モード変更
			if (tlHover && MouseL.down() && args.clickedModeChange) args.clickedModeChange();
		}

		{
			// 右上アイコン
			const auto trPoint = args.screenRect.tr().movedBy(Vec2{-1, 1} * (iconMargin + iconSize / 2));
			const auto trRect =
				RectF(Arg::center = trPoint, SizeF{1.0, 0.8} * iconSize).rounded(8).draw(ColorF{0.1, 0.5});
			const bool trHover = trRect.rect.intersects(Cursor::Pos());
			if (trHover)(void)trRect.drawFrame(1, Palette::White);
			(void)iconFont(U"\U000F0A24").drawAt(iconSize * 0.8, trPoint, ColorF(0.9));

			// 縮小モード
			if (trHover && MouseL.down() && args.clickedMinimalize) args.clickedMinimalize();
		}
	}
};

namespace Gui
{
	UiScreenOverlay::UiScreenOverlay() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void UiScreenOverlay::Update(const update_args& args)
	{
		p_impl->Update(args);
	}
}
