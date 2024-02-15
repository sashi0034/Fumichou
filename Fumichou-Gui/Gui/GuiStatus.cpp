#include "stdafx.h"
#include "GuiStatus.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "HwFrame.h"
#include "WidgetCheckbox.h"
#include "WidgetDocument.h"

using namespace Gui;

namespace
{
	struct EmulationView
	{
		bool isPaused;
		WidgetCheckbox stoppingCheck;
	};

	struct CpuView
	{
	};

	struct StatusDraw : Document::Drawer
	{
		using Drawer::operator();

		void operator ()(EmulationView& self) const
		{
			const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
			auto draw = [&](int y, const String& text)
			{
				font(text).draw(Arg::leftCenter = leftCenter.movedBy(0, y * LineHeight), Palette::Darkgray);
			};
			auto&& frame = Nes::HwFrame::Instance();
			draw(0, U"Frame={}"_fmt(frame.GetFrameCount()));
			draw(1, U"Cycles={}"_fmt(frame.GetCycleCount()));
			if (self.stoppingCheck.Update({
				.availableRect = LineRect().movedBy(0, 2 * LineHeight),
				.toggle = self.isPaused,
				.text = U"Pause Emulation",
				.textColor = Palette::Darkgray,
			}))
			{
				frame.SetPaused(self.isPaused);
			}
		}

		void operator ()(CpuView) const
		{
			auto&& cpu = Nes::HwFrame::Instance().GetHw().GetMos6502();
			const String text = U"PC=${:04X}"_fmt(cpu.GetRegs().pc);
			FontAsset(FontKeys::ZxProto_20_Bitmap)(text)
				.draw(Arg::leftCenter = leftCenter, Palette::Darkgray);
		}
	};

	using StatusDocumentData = DocumentData<
		StatusDraw,
		EmulationView,
		CpuView>;

	void generateTexts(StatusDocumentData::array_type& texts)
	{
		texts.push_back(Document::HeaderText(U"Emulation Status"));
		texts.push_back(std::monostate{});
		texts.push_back(EmulationView());
		texts.push_back(std::monostate{});
		texts.push_back(std::monostate{});

		texts.push_back(Document::HeaderText(U"CPU Status"));
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
