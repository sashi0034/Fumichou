#include "stdafx.h"
#include "GuiStatus.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "HwFrame.h"
#include "LogReader.h"
#include "WidgetButton.h"
#include "WidgetDocument.h"
#include "Details/DebuggerScript.h"

using namespace Gui;

namespace
{
	struct FunctionViewer
	{
		WidgetButton clearAbortButton{};
		WidgetButton copyTraceButton{};
	};

	struct EmulationViewer
	{
	};

	struct CartridgeViewer
	{
		FilePathView currentPath{};
		String cachedName{};
	};

	struct CpuViewer
	{
	};

	struct PpuViewer
	{
	};

	struct ScriptViewer
	{
		DebuggerScript script{};
	};

	struct StatusDrawer : Document::Drawer
	{
		using Drawer::operator();

		void drawTextLine(int y, const String& text) const
		{
			const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
			font(text).draw(Arg::leftCenter = leftCenter.movedBy(0, y * LineHeight), Palette::Darkgray);
		};

		void operator ()(FunctionViewer& self) const
		{
			auto&& frame = Nes::HwFrame::Instance();

			if (self.clearAbortButton.Update({
				.availableRect = LineRect().movedBy(0, 0 * LineHeight),
				.emojiIcon = frame.GetAbort().has_value() ? U"🧹"_emoji : U"✨"_emoji,
				.text = U"Clear Abort",
				.textColor = Palette::Darkgray,
			}))
			{
				frame.ClearAbort();
			}

			if (self.copyTraceButton.Update({
				.availableRect = LineRect().movedBy(0, 1.5 * LineHeight),
				.emojiIcon = U"🚜"_emoji,
				.text = U"Copy Traces to Clipboard",
				.textColor = Palette::Darkgray,
			}))
			{
				String str{};
				for (int i = Nes::LogReader::GetTraceSize() - 1; i >= 0; --i)
				{
					auto&& next = Nes::LogReader::GetTraceData(i);
					str += U"[{}] {}\n"_fmt(next.tag, next.message);
				}
				Clipboard::SetText(str);
			}
		}

		void operator ()(EmulationViewer& self) const
		{
			const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
			auto&& frame = Nes::HwFrame::Instance();
			drawTextLine(0, U"Frame={}"_fmt(frame.GetFrameCount()));
			drawTextLine(1, U"Cycles={}"_fmt(frame.GetCycleCount()));
			const double performance = 1000 * (frame.GetRunningTime() / frame.GetFrameCount());
			drawTextLine(2, U"Performance={:.4f}ms / {:.4f}ms"_fmt(performance, 1000 / 60.0));
		}

		void operator ()(CartridgeViewer& self) const
		{
			const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
			auto&& frame = Nes::HwFrame::Instance();
			auto&& cart = frame.GetHw().GetCartridge();

			if (frame.CurrentRomFile() != self.currentPath)
			{
				self.currentPath = frame.CurrentRomFile();
				self.cachedName = FileSystem::FileName(self.currentPath);
			}

			drawTextLine(0, U"File=\"{}\""_fmt(self.cachedName));
			drawTextLine(1, U"Mapper={}"_fmt(static_cast<uint8>(cart.GetRomData().GetMapperNumber())));
		}

		void operator ()(CpuViewer) const
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

		void operator ()(PpuViewer) const
		{
			auto& ppu = Nes::HwFrame::Instance().GetHw().GetPpu();

			const auto& reg0 = ppu.Regs();
			const auto& reg1 = ppu.UnstableRegs();

			drawTextLine(0, U"Scanline={} Linecycles={}"_fmt(ppu.ScanLine(), ppu.LineCycles()));
			drawTextLine(1, U"CONTROL={:02X} STASUS={:02X}"_fmt(
				             static_cast<uint8>(reg0.control), static_cast<uint8>(reg1.status)));
		}

		void operator ()(ScriptViewer& self) const
		{
			self.script.Refresh();
			auto&& lastReloaded = self.script.LastReloadedTime();
			drawTextLine(0, U"File=\"{}\""_fmt(self.script.Filepath()));
			drawTextLine(1, U"{:02}:{:02}:{:02} {}"_fmt(
				             lastReloaded.hour, lastReloaded.minute, lastReloaded.second,
				             (self.script.IsSucceeded() ? U"Succeed 🎉"_sv : U"Error ❌ See Logs for Details."_sv)));
		}
	};

	using StatusDocumentData = DocumentData<
		StatusDrawer,
		FunctionViewer,
		CartridgeViewer,
		EmulationViewer,
		CpuViewer,
		PpuViewer,
		ScriptViewer>;

	void generateTexts(StatusDocumentData::array_type& texts)
	{
		texts.push_back(Document::HeaderText(U"Functions"));
		texts.push_back(std::monostate{});
		texts.push_back(FunctionViewer{});
		for (const auto i : step(static_cast<int>(2 * 1.5) - 1)) texts.push_back(std::monostate{});

		texts.push_back(Document::SplitLine{});

		texts.push_back(Document::HeaderText(U"Emulation Status"));
		texts.push_back(std::monostate{});
		texts.push_back(EmulationViewer());
		for (const auto i : step(2)) texts.push_back(std::monostate{});

		texts.push_back(Document::SplitLine{});

		texts.push_back(Document::HeaderText(U"Cartridge Information"));
		texts.push_back(std::monostate{});
		texts.push_back(CartridgeViewer{});
		for (const auto i : step(1)) texts.push_back(std::monostate{});

		texts.push_back(Document::SplitLine{});

		texts.push_back(Document::HeaderText(U"CPU Status"));
		texts.push_back(std::monostate{});
		texts.push_back(CpuViewer());
		for (const auto i : step(2)) texts.push_back(std::monostate{});

		texts.push_back(Document::SplitLine{});

		texts.push_back(Document::HeaderText(U"PPU Status"));
		texts.push_back(std::monostate{});
		texts.push_back(PpuViewer());
		for (const auto i : step(1)) texts.push_back(std::monostate{});

		texts.push_back(Document::SplitLine{});

		texts.push_back(Document::HeaderText(U"Script Information"));
		texts.push_back(std::monostate{});
		texts.push_back(ScriptViewer());
		for (const auto i : step(1)) texts.push_back(std::monostate{});
	}
}

struct GuiStatus::Impl
{
	WidgetDocument<StatusDocumentData> m_document{};

	void Update(const Size& availableRegion)
	{
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
