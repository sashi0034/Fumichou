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
	int m_headIndex{};
	int m_focusedIndex{};
	WidgetSlideBar m_slideBar{};

	void Update(const SizeF& availableRegion)
	{
		const int lineHeight = getToml<int>(U"lineHeight");
		const int tagLeft = getToml<int>(U"tagLeft");
		const int messageLeft = getToml<int>(U"messageLeft");

		// 表示領域のライン描画
		int indexTail = 0;
		for (double y = availableRegion.y; y > 0; y -= lineHeight)
		{
			const int lineIndex = m_headIndex + indexTail;
			auto&& data = Nes::LogReader::GetTraceData(lineIndex);
			auto&& font = FontAsset(FontKeys::ZxProto_20_Bitmap);
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

		// インデックス移動
		if (RectF(availableRegion).intersects(Cursor::PosF()))
		{
			const int amount = indexTail / 8;
			if (Mouse::Wheel() < 0) m_headIndex += amount;
			else if (Mouse::Wheel() > 0) m_headIndex -= amount;
		}

		m_slideBar.Update({
			.availableRect = WidgetSlideBar::AvailableAtRightCenter(availableRegion),
			.currentIndex = m_headIndex,
			.minIndex = 0,
			.maxIndex = Nes::LogReader::GetTraceSize(),
			.pageSize = indexTail
		});

		// フォーカス変更
		if (not IsMouseCaptured() && MouseL.down())
		{
			int index = m_headIndex;
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

private:
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
