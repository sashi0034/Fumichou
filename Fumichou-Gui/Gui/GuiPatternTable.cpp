#include "stdafx.h"
#include "GuiPatternTable.h"

#include "HwFrame.h"
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
	void Update(const Size& availableRegion)
	{
		const int margin = getToml<int>(U"margin");
		auto&& patternTable = Nes::HwFrame::Instance().GetHw().GetCartridge().GetBoard().PatternTableTexture();
		const int lineW = (availableRegion.x / 8) * 8;
		for (int y = 0; y < patternTable.width() / lineW + 1; ++y)
		{
			patternTable(lineW * y, 0, lineW, 8).draw(Arg::topCenter = Vec2{availableRegion.x / 2, margin + y * 8});
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
