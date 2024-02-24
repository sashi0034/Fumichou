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
	WidgetButton m_loadButton{};
	WidgetButton m_resetButton{};
	WidgetCheckbox m_stoppingCheck{};
	WidgetButton m_stepCycleButton{};
	WidgetButton m_stepFrameButton{};

	WidgetButton m_dummyButton{};

	void Update(const Size& availableRegion)
	{
		const int leftMargin = getToml<int>(U"leftMargin");
		const Rect lineRect = Rect(availableRegion.withY(LineHeight)).moveBy(leftMargin, 2);

		auto&& frame = Nes::HwFrame::Instance();
		constexpr double lineH = 1.5 * LineHeight;
		const int halfW = leftMargin + availableRegion.x / 2;

		if (m_loadButton.Update({
			.availableRect = lineRect.movedBy(0, 0 * lineH),
			.emojiIcon = U"📂"_emoji,
			.text = U"Load ROM File",
			.textColor = Palette::Darkgray,
		}))
		{
			const auto romPath = Dialog::OpenFile({}, FileSystem::CurrentDirectory() + U"rom").value_or(U"");
			(void)frame.StartRomFile(romPath);
		}

		if (m_loadButton.Update({
			.availableRect = lineRect.movedBy(0, 1 * lineH),
			.emojiIcon = U"♻️"_emoji,
			.text = U"Reset (Ctrl + R)",
			.textColor = Palette::Darkgray,
		}) || (KeyControl.pressed() && KeyR.down()))
		{
			(void)frame.ResetRomFile();
		}

		if (m_stoppingCheck.Update({
			.availableRect = lineRect.movedBy(0, 2 * lineH),
			.toggle = m_isPaused,
			.text = U"Pause Emulation (F9)",
			.textColor = Palette::Darkgray,
		}) || KeyF9.down())
		{
			if (KeyF9.down()) m_isPaused = not m_isPaused;
			frame.SetPaused(m_isPaused);
		}

		if (m_stepCycleButton.Update({
			.availableRect = lineRect.movedBy(0, 3 * lineH),
			.emojiIcon = U"▶️"_emoji,
			.text = U"Step 1 Cycle (F10)",
			.textColor = Palette::Darkgray,
		}) || KeyF10.down())
		{
			frame.StepOneCycle();
		}

		if (m_stepFrameButton.Update({
			.availableRect = lineRect.movedBy(0, 4 * lineH),
			.emojiIcon = U"⏩"_emoji,
			.text = U"Step 1 Frame (F11)",
			.textColor = Palette::Darkgray,
		}) || KeyF11.down())
		{
			frame.StepOneFrame();
		}

		if (m_dummyButton.Update({
			.availableRect = lineRect.movedBy(halfW, 3 * lineH),
			.emojiIcon = U"😺"_emoji,
			.text = U"Dummy Button 1",
			.textColor = Palette::Darkgray,
		}))
		{
			// TODO: なんか考える
			// 多分スプライト可視化ボタンなど
		}

		if (m_dummyButton.Update({
			.availableRect = lineRect.movedBy(halfW, 4 * lineH),
			.emojiIcon = U"😺"_emoji,
			.text = U"Dummy Button 2",
			.textColor = Palette::Darkgray,
		}))
		{
			// TODO: なんか考える
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
