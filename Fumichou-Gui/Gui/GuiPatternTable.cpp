#include "stdafx.h"
#include "GuiPatternTable.h"

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

	void Update(const Size& availableRegion)
	{
		auto&& patternTable = Nes::HwFrame::Instance().GetHw().GetCartridge().GetBoard().PatternTableTexture();

		// 行の適切なサイズを計算
		const int lineColumn = getToml<int>(U"lineColumn");
		const int sliderMargin = getToml<int>(U"sliderMargin");
		const int lineW = lineColumn * 8;
		const double scale = (availableRegion.x - sliderMargin) / lineW;

		// 行ごとに描画
		const ScopedRenderStates2D renderStates2D{SamplerState::ClampNearest};
		const int lineSize = patternTable.width() / lineW;
		for (int y = 0; y < lineSize; ++y)
		{
			patternTable(lineW * y, 0, lineW, 8)
				.scaled(scale)
				.draw(Arg::topLeft = Vec2{0, -m_offsetY + y * 8 * scale});
		}

		// 垂直バー
		m_slider.UpdateVertical({
			.availableRect = WidgetSlideBar::AvailableAtRightCenter(availableRegion),
			.currentIndex = m_offsetY,
			.minIndex = 0,
			.maxIndex = static_cast<int>(lineSize * 8 * scale),
			.pageSize = availableRegion.y - 1
		});
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
