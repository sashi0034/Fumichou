#include "stdafx.h"
#include "GuiTrace.h"

#include "FontKeys.h"
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
	int m_head{};
	WidgetSlideBar m_slideBar{};

	void Update(const SizeF& availableRegion)
	{
		const int lineHeight = getToml<int>(U"lineHeight");
		const int tagLeft = getToml<int>(U"tagLeft");
		const int messageLeft = getToml<int>(U"messageLeft");

		int indexTail = 0;
		for (double y = availableRegion.y; y > 0; y -= lineHeight)
		{
			auto&& data = Nes::LogReader::GetTraceData(m_head + indexTail);
			auto&& font = FontAsset(FontKeys::ZxProto_20_Bitmap);
			font(Format(m_head + indexTail))
				.draw(Arg::leftCenter = Vec2{8, y - lineHeight / 2}, Palette::Gray);
			font(data.tag)
				.draw(Arg::leftCenter = Vec2{tagLeft, y - lineHeight / 2});
			font(data.message)
				.draw(Arg::leftCenter = Vec2{messageLeft, y - lineHeight / 2});
			indexTail++;
		}

		m_slideBar.Update({
			.availableRect = WidgetSlideBar::AvailableAtRightCenter(availableRegion),
			.currentIndex = m_head,
			.minIndex = 0,
			.maxIndex = Nes::LogReader::GetTraceSize(),
			.pageSize = indexTail
		});
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
