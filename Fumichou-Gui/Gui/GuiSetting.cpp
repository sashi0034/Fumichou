#include "stdafx.h"
#include "GuiSetting.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "WidgetButton.h"
#include "WidgetCheckbox.h"
#include "WidgetDocument.h"

using namespace Gui;

namespace
{
	struct SchemeSetting
	{
		bool isFullscreen{};
		WidgetCheckbox fullscreenCheckbox{};
		WidgetButton windowSizeButton{};
		bool isAutoSave{};
		WidgetCheckbox autosaveCheckbox{};
		WidgetButton soundVolumeButton{};
	};

	struct ControllerSetting
	{
	};

	struct SettingDrawer : Document::Drawer
	{
		using Drawer::operator();

		void operator ()(SchemeSetting& self) const
		{
			if (self.fullscreenCheckbox.Update({
				.availableRect = LineRect().movedBy(0, 0 * 1.5 * LineHeight),
				.toggle = self.isFullscreen,
				.text = U"Fullscreen",
				.textColor = Palette::Darkgray,
			}))
			{
			}

			if (self.windowSizeButton.Update({
				.availableRect = LineRect().movedBy(0, 1 * 1.5 * LineHeight),
				.emojiIcon = U"🖥️"_emoji,
				.text = U"Window Size",
				.textColor = Palette::Darkgray,
			}))
			{
			}

			if (self.autosaveCheckbox.Update({
				.availableRect = LineRect().movedBy(0, 2 * 1.5 * LineHeight),
				.toggle = self.isAutoSave,
				.text = U"Auto Save on Closing",
				.textColor = Palette::Darkgray,
			}))
			{
			}

			if (self.soundVolumeButton.Update({
				.availableRect = LineRect().movedBy(0, 3 * 1.5 * LineHeight),
				.emojiIcon = U"📢"_emoji,
				.text = U"Sound Volume",
				.textColor = Palette::Darkgray,
			}))
			{
			}
		}

		void operator ()(ControllerSetting& self) const
		{
		}
	};

	using SettingDocumentData = DocumentData<
		SettingDrawer,
		SchemeSetting,
		ControllerSetting>;

	void generateTexts(SettingDocumentData::array_type& texts)
	{
		texts.push_back(Document::HeaderText(U"Schemes"));
		texts.push_back(std::monostate{});
		texts.push_back(SchemeSetting());
		for (const auto i : step(static_cast<int>(4 * 1.5) - 1)) texts.push_back(std::monostate{});

		texts.push_back(Document::SplitLine());

		texts.push_back(Document::HeaderText(U"Controller"));
		texts.push_back(std::monostate{});
	}
}

struct GuiSetting::Impl
{
	WidgetDocument<SettingDocumentData> m_document{};

	void Update(const Size& availableRegion)
	{
		if (not m_document.Data().Size()) generateTexts(m_document.Data().Raw());
		m_document.Update(availableRegion);
	}
};

namespace Gui
{
	GuiSetting::GuiSetting() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiSetting::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
