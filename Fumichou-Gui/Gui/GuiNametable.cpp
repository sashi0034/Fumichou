#include "stdafx.h"
#include "GuiNametable.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "HwFrame.h"
#include "WidgetSlideBar.h"
#include "WidgetTabBar.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"GuiNametable." + key);
	}
}

struct GuiNametable::Impl
{
	WidgetTabBar m_tableTab{};
	int m_tableIndex{};
	std::array<int, 4> m_headIndexes{};
	WidgetSlideBar m_slider{};
	int m_focusedIndex{-1};

	void Update(const Size& availableRegion)
	{
		static const std::array tabNames = {U"#0"_s, U"#1"_s, U"#2"_s, U"#3"_s};
		m_tableTab.Update({
			.availableRect = Rect(availableRegion.withY(LineHeight)),
			.currentIndex = m_tableIndex,
			.tabNames = tabNames,
		});

		const Transformer2D t{Mat3x2::Translate(0, LineHeight), TransformCursor::Yes};
		updateBody({availableRegion.x, availableRegion.y - LineHeight});
	}

private:
	void updateBody(const Size& availableRegion)
	{
		auto&& font = FontAsset(FontKeys::ZxProto_20_Bitmap);

		// 表示領域のライン描画
		constexpr int columnSize = 8;
		int indexTail = 0;
		auto&& ppu = Nes::HwFrame::Instance().GetHw().GetPpu();
		auto&& nt = Nes::HwFrame::Instance().GetHw().GetPpu().GetNametableData();
		const uint16 rowIndex0 = m_headIndexes[m_tableIndex] * columnSize + ppu.GetNametableOffset()[m_tableIndex];
		uint16 rowIndex = rowIndex0;
		const int dataLeft = getToml<int>(U"dataLeft");
		const int addressLeft = getToml<int>(U"addressLeft");
		for (int y = 0; y < availableRegion.y; y += LineHeight)
		{
			if (rowIndex == m_focusedIndex)
			{
				// フォーカス対象
				RectF(Arg::bottomLeft = Vec2{0, y}, availableRegion.withY(LineHeight)).draw(ColorF(1.0, 0.1));
			}

			// 実際のアドレス
			font(U"{:04X}"_fmt(0x2000 + rowIndex + indexTail * columnSize))
				.draw(Arg::topLeft = Vec2{8, y}, Palette::Gray);

			// インデックス
			font(U"{:03X}"_fmt(rowIndex))
				.draw(Arg::topLeft = Vec2{addressLeft, y}, ColorOrange);

			// データ
			const auto dataColor = (rowIndex & 0x3FF) < 0x3C0 ? ColorGreen : ColorBlue; // パレット部分は色を変える
			font(U"{:02X} {:02X} {:02X} {:02X}  {:02X} {:02X} {:02X} {:02X}"_fmt(
					nt[rowIndex + 0], nt[rowIndex + 1], nt[rowIndex + 2], nt[rowIndex + 3],
					nt[rowIndex + 4], nt[rowIndex + 5], nt[rowIndex + 6], nt[rowIndex + 7]))
				.draw(Arg::topLeft = Vec2{dataLeft, y}, dataColor);

			indexTail++;
			rowIndex += columnSize;
		}

		// インデックス移動
		if (Rect(availableRegion).intersects(Cursor::Pos()))
		{
			const int step = indexTail / 16;
			const auto wheel = Mouse::Wheel();
			if (wheel < 0) m_headIndexes[m_tableIndex] -= step;
			else if (wheel > 0) m_headIndexes[m_tableIndex] += step;
		}

		// 垂直バー
		m_slider.UpdateVerticalInverted({
			.availableRect = WidgetSlideBar::AvailableAtRightCenter(availableRegion),
			.currentIndex = m_headIndexes[m_tableIndex],
			.minIndex = 0,
			.maxIndex = static_cast<int>(nt.size() / m_headIndexes.size()) / columnSize,
			.pageSize = indexTail
		});

		// フォーカス変更
		if (not IsClickCaptured() && MouseL.down())
		{
			int index = rowIndex0;
			for (int y = 0; y < availableRegion.y; y += LineHeight)
			{
				if (RectF(Arg::bottomLeft = Vec2{0, y}, availableRegion.withY(LineHeight)).intersects(Cursor::Pos()))
				{
					AcceptClickCaptured();
					m_focusedIndex = index;
					break;
				}
				index += columnSize;
			}
		}
	}

	//そのネームテーブルに実際にアクセス可能であるアドレス
	// int getTableBaseAddr(const Nes::Ppu& ppu) const
	// {
	// 	for (int i = 0; i < 4; ++i)
	// 	{
	// 		if (0x400 * m_tableIndex == ppu.GetNametableOffset()[i]) return 0x2000 + 0x400 * i;
	// 	}
	//
	// 	return -1;
	// }
};

namespace Gui
{
	GuiNametable::GuiNametable() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiNametable::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
