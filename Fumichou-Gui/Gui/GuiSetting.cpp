#include "stdafx.h"
#include "GuiSetting.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "WidgetDocument.h"

using namespace Gui;

namespace
{
	struct SchemeSetting
	{
	};

	struct ControllerSetting
	{
	};

	struct SettingDrawer : Document::Drawer
	{
		using Drawer::operator();

		void drawTextLine(int y, const String& text) const
		{
			const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
			font(text).draw(Arg::leftCenter = leftCenter.movedBy(0, y * LineHeight), Palette::Darkgray);
		};

		void operator ()(SchemeSetting& self) const
		{
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
