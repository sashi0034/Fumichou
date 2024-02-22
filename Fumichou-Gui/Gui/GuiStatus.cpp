#include "stdafx.h"
#include "GuiStatus.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "HwFrame.h"
#include "WidgetButton.h"
#include "WidgetCheckbox.h"
#include "WidgetDocument.h"

using namespace Gui;

namespace
{
	struct EmulationView
	{
		bool isPaused;
		WidgetCheckbox stoppingCheck;
		WidgetButton stepCycleButton;
		WidgetButton stepFrameButton;
	};

	struct CpuView
	{
	};

	struct PpuView
	{
	};

	struct StatusDraw : Document::Drawer
	{
		using Drawer::operator();

		void drawTextLine(int y, const String& text) const
		{
			const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
			font(text).draw(Arg::leftCenter = leftCenter.movedBy(0, y * LineHeight), Palette::Darkgray);
		};

		void operator ()(EmulationView& self) const
		{
			const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
			auto&& frame = Nes::HwFrame::Instance();
			drawTextLine(0, U"Frame={}"_fmt(frame.GetFrameCount()));
			drawTextLine(1, U"Cycles={}"_fmt(frame.GetCycleCount()));
			if (self.stoppingCheck.Update({
				.availableRect = LineRect().movedBy(0, 3 * LineHeight),
				.toggle = self.isPaused,
				.text = U"Pause Emulation (F9)",
				.textColor = Palette::Darkgray,
			}) || KeyF9.down())
			{
				if (KeyF9.down()) self.isPaused = not self.isPaused;
				frame.SetPaused(self.isPaused);
			}

			if (self.stepCycleButton.Update({
				.availableRect = LineRect().movedBy(0, 4 * LineHeight),
				.emojiIcon = U"▶️"_emoji,
				.text = U"Step 1 Cycle (F10)",
				.textColor = Palette::Darkgray,
			}) || KeyF10.down())
			{
				frame.StepOneCycle();
			}

			if (self.stepFrameButton.Update({
				.availableRect = LineRect().movedBy(0, 5 * LineHeight),
				.emojiIcon = U"⏩"_emoji,
				.text = U"Step 1 Frame",
				.textColor = Palette::Darkgray,
			}))
			{
				frame.StepOneFrame();
			}
		}

		void operator ()(CpuView) const
		{
			auto& cpu = Nes::HwFrame::Instance().GetHw().GetMos6502();
			auto& regs = cpu.GetRegs();
			auto& flags = cpu.GetFlags();
			drawTextLine(0, U"PC={:04X} SP={:02X}"_fmt(regs.pc, regs.sp));
			drawTextLine(1, U"A={:02X} X={:02X} Y={:02X}"_fmt(regs.a, regs.x, regs.y));
			drawTextLine(2, U"C={} Z={} I={} D={} V={} N={}"_fmt(
				             static_cast<uint8>(flags.c),
				             static_cast<uint8>(flags.z),
				             static_cast<uint8>(flags.i),
				             static_cast<uint8>(flags.d),
				             static_cast<uint8>(flags.d),
				             static_cast<uint8>(flags.n)));
		}

		void operator ()(PpuView) const
		{
			auto& ppu = Nes::HwFrame::Instance().GetHw().GetPpu();
			// TODO
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
		texts.push_back(std::monostate{});
		texts.push_back(std::monostate{});
		texts.push_back(std::monostate{});

		texts.push_back(Document::SplitLine{});

		texts.push_back(Document::HeaderText(U"CPU Status"));
		texts.push_back(std::monostate{});
		texts.push_back(CpuView());
		texts.push_back(std::monostate{});
		texts.push_back(std::monostate{});
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
