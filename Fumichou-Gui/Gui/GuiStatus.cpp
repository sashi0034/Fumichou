#include "stdafx.h"
#include "GuiStatus.h"

#include "FontKeys.h"
#include "HwFrame.h"
#include "WidgetDocument.h"

using namespace Gui;

namespace
{
	struct CpuView
	{
	};

	struct StatusDraw : Document::Drawer
	{
		using Drawer::operator();

		void operator ()(CpuView) const
		{
			auto&& cpu = Nes::HwFrame::Instance().GetHw().GetMos6502();
			const String text = U"PC={:04X}"_fmt(cpu.GetRegs().pc);
			FontAsset(FontKeys::ZxProto_20_Bitmap)(text)
				.draw(Arg::leftCenter = leftCenter, Palette::Darkgray);
		}
	};

	using StatusDocumentData = DocumentData<StatusDraw, CpuView>;

	void generateTexts(StatusDocumentData::array_type& texts)
	{
		texts.push_back(Document::HeaderText(U"CPU status"));
		texts.push_back(std::monostate{});
		texts.push_back(CpuView());
	}
}

struct GuiStatus::Impl
{
	WidgetDocument<StatusDocumentData> m_document{};

	void Update(const Size& availableRegion)
	{
		const ScopedViewport2D viewport2D{availableRegion};
		if (not m_document.Data().Size()) generateTexts(m_document.Data().Raw());
		m_document.Update(availableRegion);
	}
};

namespace Gui
{
	GuiStatus::GuiStatus()
		:
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiStatus::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
