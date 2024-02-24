#include "stdafx.h"
#include "GuiToolbar.h"

#include "GuiForward.h"
#include "HwFrame.h"
#include "WidgetButton.h"
#include "WidgetCheckbox.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"GuiToolbar." + key);
	}
}

struct GuiToolbar::Impl
{
	bool m_isPaused{};
	WidgetCheckbox m_stoppingCheck;
	WidgetButton m_stepCycleButton;
	WidgetButton m_stepFrameButton;

	void Update(const Size& availableRegion)
	{
		const int leftMargin = getToml<int>(U"leftMargin");
		const Rect lineRect = Rect(availableRegion.withY(LineHeight)).moveBy(leftMargin, 0);

		auto&& frame = Nes::HwFrame::Instance();

		if (m_stoppingCheck.Update({
			.availableRect = lineRect.movedBy(0, 3 * LineHeight),
			.toggle = m_isPaused,
			.text = U"Pause Emulation (F9)",
			.textColor = Palette::Darkgray,
		}) || KeyF9.down())
		{
			if (KeyF9.down()) m_isPaused = not m_isPaused;
			frame.SetPaused(m_isPaused);
		}

		if (m_stepCycleButton.Update({
			.availableRect = lineRect.movedBy(0, 4 * LineHeight),
			.emojiIcon = U"▶️"_emoji,
			.text = U"Step 1 Cycle (F10)",
			.textColor = Palette::Darkgray,
		}) || KeyF10.down())
		{
			frame.StepOneCycle();
		}

		if (m_stepFrameButton.Update({
			.availableRect = lineRect.movedBy(0, 5 * LineHeight),
			.emojiIcon = U"⏩"_emoji,
			.text = U"Step 1 Frame",
			.textColor = Palette::Darkgray,
		}))
		{
			frame.StepOneFrame();
		}
	}
};

namespace Gui
{
	GuiToolbar::GuiToolbar() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiToolbar::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
