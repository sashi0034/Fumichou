#include "stdafx.h"
#include "GuiPatternTable.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "HwFrame.h"
#include "WidgetSlideBar.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"GuiPatternTable." + key);
	}
}

struct GuiPatternTable::Impl
{
	int m_offsetY{};
	WidgetSlideBar m_slider{};
	bool m_showGrid{};

	void Update(const Size& availableRegion)
	{
		auto&& patternTable = Nes::HwFrame::Instance().GetHw().GetCartridge().GetBoard().PatternTableTexture();

		// 行の適切なサイズを計算
		const int lineColumn = getToml<int>(U"lineColumn");
		const int lineLeft = getToml<int>(U"lineLeft");
		const int lineHeight = getToml<int>(U"lineHeight");
		const int sliderMargin = getToml<int>(U"sliderMargin");
		const int lineW = lineColumn * 8;
		const double scale = static_cast<double>(availableRegion.x - sliderMargin - lineLeft) / lineW;

		// 行ごとに描画
		const ScopedRenderStates2D renderStates2D{SamplerState::ClampNearest};
		const int lineSize = patternTable.width() / lineW;
		auto&& font = FontAsset(FontKeys::ZxProto_20_Bitmap);
		for (int y = 0; y < lineSize; ++y)
		{
			const auto leftCenter = Vec2{0, -m_offsetY + (y + 0.5) * lineHeight * scale};
			font(U"{:03X}"_fmt(y * lineColumn)).draw(Arg::leftCenter = leftCenter);
			const auto lineRect = patternTable(lineW * y, 0, lineW, 8)
			                      .scaled(scale)
			                      .draw(Arg::leftCenter = leftCenter.withX(lineLeft));
			// (void)rect.bottom().movedBy(0, 1).draw(ColorBlue);
		}

		if (m_showGrid)
		{
			// グリッド表示
			const auto gridThickness = getToml<int>(U"gridThickness");
			for (int y = 1; y < lineSize; ++y)
			{
				const auto by = -m_offsetY + y * lineHeight * scale;
				(void)Line(lineLeft, by, availableRegion.x - sliderMargin, by)
					.draw(gridThickness, Palette::Darkslateblue);
			}
			for (int x = 1; x < lineColumn; ++x)
			{
				const auto rx = lineLeft + x * 8 * scale;
				(void)Line(rx, 0, rx, availableRegion.y)
					.draw(gridThickness, x % 4 == 0 ? Palette::Blue : Palette::Darkslateblue);
			}
		}

		// インデックス移動
		bool mouseIntersects{};
		if (RectF(availableRegion).intersects(Cursor::PosF()))
		{
			mouseIntersects = true;
			const int step = static_cast<int>(lineHeight * scale);
			const auto wheel = Mouse::Wheel();
			int amount{};
			if (wheel < 0) amount = -step;
			else if (wheel > 0) amount = step;
			m_offsetY += amount;
		}

		// 垂直バー
		m_slider.UpdateVerticalInverted({
			.availableRect = WidgetSlideBar::AvailableAtRightCenter(availableRegion),
			.currentIndex = m_offsetY,
			.minIndex = 0,
			.maxIndex = static_cast<int>(lineSize * lineHeight * scale),
			.pageSize = availableRegion.y - 1
		});

		if (mouseIntersects && not IsClickCaptured() && MouseL.down())
		{
			// グリッド表示切り替え
			AcceptClickCaptured();
			m_showGrid = not m_showGrid;
		}
	}
};

namespace Gui
{
	GuiPatternTable::GuiPatternTable() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiPatternTable::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
