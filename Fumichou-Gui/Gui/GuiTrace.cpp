#include "stdafx.h"
#include "GuiTrace.h"

#include "FontKeys.h"
#include "LogReader.h"
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

	void Update(const SizeF& availableRegion)
	{
		const int lineHeight = getToml<int>(U"lineHeight");
		const int tagWidth = getToml<int>(U"tagWidth");
		int index = 0;
		for (double y = availableRegion.y; y > -lineHeight; y -= lineHeight)
		{
			auto&& data = Nes::LogReader::GetTraceData(m_head + index);
			FontAsset(FontKeys::ZxProto_20_Bitmap)(data.tag)
				.draw(Arg::leftCenter = Vec2{8, y + lineHeight / 2});
			FontAsset(FontKeys::ZxProto_20_Bitmap)(data.message)
				.draw(Arg::leftCenter = Vec2{tagWidth, y + lineHeight / 2});
			index++;
		}
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
