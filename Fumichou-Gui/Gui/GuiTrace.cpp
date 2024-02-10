#include "stdafx.h"
#include "GuiTrace.h"

#include <iso646.h>

#include "FontKeys.h"
#include "GuiForward.h"
#include "LogReader.h"
#include "WidgetSlideBar.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"GuiTrace." + key);
	}
}

struct GuiTrace::Impl
{
	int m_pageHead{};
	int m_headIndex{};
	int m_focusedIndex{};
	WidgetSlideBar m_horizontalSlider{};
	WidgetSlideBar m_verticalSlider{};

	void Update(const SizeF& availableRegion)
	{
		const int lineHeight = getToml<int>(U"lineHeight");
		const Transformer2D t{Mat3x2::Translate(0, lineHeight), TransformCursor::Yes};
		updateBody({availableRegion.x, availableRegion.y - lineHeight});
	}

private:
	void updateBody(const SizeF& availableRegion)
	{
		constexpr int pageSize = 1024;
		const int lineHeight = getToml<int>(U"lineHeight");
		const int tagLeft = getToml<int>(U"tagLeft");
		const int messageLeft = getToml<int>(U"messageLeft");

		auto&& font = FontAsset(FontKeys::ZxProto_20_Bitmap);

		// 表示領域のライン描画
		int indexTail = 0;
		{
			for (double y = availableRegion.y; y > 0; y -= lineHeight)
			{
				const int lineIndex = m_pageHead + m_headIndex + indexTail;
				auto&& data = Nes::LogReader::GetTraceData(lineIndex);
				if (lineIndex == m_focusedIndex)
				{
					// フォーカス対象
					RectF(Arg::bottomLeft = Vec2{0, y}, availableRegion.withY(lineHeight)).draw(ColorF(1.0, 0.1));
				}
				font(Format(lineIndex))
					.draw(Arg::leftCenter = Vec2{8, y - lineHeight / 2}, Palette::Gray);
				const auto color = getTagColor(data.tag);
				font(data.tag)
					.draw(Arg::leftCenter = Vec2{tagLeft, y - lineHeight / 2}, color);
				font(data.message)
					.draw(Arg::leftCenter = Vec2{messageLeft, y - lineHeight / 2}, color);
				indexTail++;
			}
		}

		// インデックス移動
		if (RectF(availableRegion).intersects(Cursor::PosF()))
		{
			const int step = indexTail / 8;
			const auto wheel = Mouse::Wheel();
			int amount{};
			if (wheel < 0) amount = step;
			else if (wheel > 0) amount = -step;
			if (KeyShift.pressed())
			{
				m_pageHead += amount;
			}
			else
			{
				m_headIndex += amount;
			}
		}

		m_horizontalSlider.UpdateHorizontal({
			.availableRect = RectF{
				Arg::topCenter = Vec2{availableRegion.x / 2, -lineHeight},
				SizeF{availableRegion.x, lineHeight}
			},
			.currentIndex = m_pageHead,
			.minIndex = 0,
			.maxIndex = Nes::LogReader::GetTraceSize(),
			.pageSize = pageSize
		});
		(void)font(Format(m_pageHead)).drawAt(m_horizontalSlider.GetTab().center(), ColorF(0.9));

		m_verticalSlider.UpdateVertical({
			.availableRect = WidgetSlideBar::AvailableAtRightCenter(availableRegion),
			.currentIndex = m_headIndex,
			.minIndex = 0,
			.maxIndex = pageSize,
			.pageSize = indexTail
		});

		// フォーカス変更
		if (not IsMouseCaptured() && MouseL.down())
		{
			int index = m_pageHead + m_headIndex;
			for (double y = availableRegion.y; y > 0; y -= lineHeight)
			{
				if (RectF(Arg::bottomLeft = Vec2{0, y}, availableRegion.withY(lineHeight)).intersects(Cursor::PosF()))
				{
					AcceptMouseCaptured();
					m_focusedIndex = index;
					break;
				}
				index++;
			}
		}
	}

	static const ColorF& getTagColor(StringView message)
	{
		static HashTable<String, ColorF> table = []()
		{
			HashTable<String, ColorF> table{};
			table[U"OP"] = ColorGreen.lerp(Palette::Lightgray, 0.3);
			table[U"RD"] = ColorBlue.lerp(Palette::Lightgray, 0.3);
			table[U"ST"] = ColorOrange.lerp(Palette::Lightgray, 0.3);
			return table;
		}();
		auto&& color = table.find(message.data());
		if (color != table.end()) return color->second;
		return Palette::Lightgray;
	}
};

namespace Gui
{
	GuiTrace::GuiTrace() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiTrace::Update(const SizeF& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
