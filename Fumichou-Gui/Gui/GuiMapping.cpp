#include "stdafx.h"
#include "GuiMapping.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "HwFrame.h"
#include "WidgetSlideBar.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"GuiMapping." + key);
	}

	struct MappingDesc
	{
		String addrRange;
		String desc;
		ColorF descColor;
	};

	struct SplitLine
	{
	};

	struct HeaderText
	{
		String text;
	};

	struct PlainText
	{
		String text;
	};

	using MappingTextType = std::variant<std::monostate, HeaderText, PlainText, MappingDesc, SplitLine>;

	template <typename T>
	void makeSingleDescs(
		Array<MappingTextType>& texts,
		const T& mapped,
		const ColorF& color)
	{
		uint16 currentStart{};
		StringView currentDesc{};
		for (uint32 addr = 0; addr < 0x10000; ++addr)
		{
			if (mapped[addr].desc != currentDesc || addr == 0xFFFF)
			{
				if (addr != 0)
				{
					texts.push_back(MappingDesc{
						.addrRange = U"[{:04X}, {:04X}]"_fmt(currentStart, addr - 1),
						.desc = currentDesc.data(),
						.descColor = color
					});
				}
				currentStart = addr;
				currentDesc = mapped[addr].desc;
			}
		}
	}

	Array<MappingTextType> generateTexts()
	{
		Array<MappingTextType> texts{};

		auto&& mmu = Nes::HwFrame::Instance().GetHw().GetMmu();
		texts.push_back(HeaderText(U"CPU read-mapping"));
		texts.push_back(std::monostate{});
		makeSingleDescs(texts, mmu.GetCpuRead(), ColorBlue);
		texts.push_back(SplitLine());
		texts.push_back(HeaderText(U"CPU write-mapping"));
		texts.push_back(std::monostate{});
		makeSingleDescs(texts, mmu.GetCpuWrite(), ColorOrange);
		texts.push_back(SplitLine());
		texts.push_back(HeaderText(U"PPU read-mapping"));
		texts.push_back(std::monostate{});
		makeSingleDescs(texts, mmu.GetPpuRead(), ColorBlue);
		texts.push_back(SplitLine());
		texts.push_back(HeaderText(U"PPU write-mapping"));
		texts.push_back(std::monostate{});
		makeSingleDescs(texts, mmu.GetPpuWrite(), ColorOrange);

		return texts;
	}

	struct TextDraw
	{
		int rightmost;
		Vec2 leftCenter;

		void operator()(std::monostate) const { return; }

		void operator()(const HeaderText& text) const
		{
			FontAsset(FontKeys::ZxProto_20_Bitmap)(text.text)
				.draw(24, Arg::leftCenter = leftCenter.movedBy(0, LineHeight / 2));
		}

		void operator()(const PlainText& text)
		{
			FontAsset(FontKeys::ZxProto_20_Bitmap)(text.text).draw(Arg::leftCenter = leftCenter);
		}

		void operator()(const MappingDesc& desc)
		{
			FontAsset(FontKeys::ZxProto_20_Bitmap)(desc.addrRange)
				.draw(Arg::leftCenter = leftCenter, Palette::Darkgray);
			FontAsset(FontKeys::ZxProto_20_Bitmap)(desc.desc)
				.draw(Arg::leftCenter = leftCenter.withX(getToml<int>(U"descLeft")), desc.descColor);
		}

		void operator()(SplitLine) const
		{
			(void)Line(leftCenter, leftCenter.withX(rightmost)).stretched(-4).draw(1, Palette::Gray);
		}
	};
}

struct GuiMapping::Impl
{
	int m_headIndex{};
	Array<MappingTextType> m_mappingTexts{};
	WidgetSlideBar m_verticalSlider{};

	void Update(const Size& availableRegion)
	{
		const ScopedViewport2D viewport2D{availableRegion};
		if (not m_mappingTexts) m_mappingTexts = generateTexts();

		const double leftMargin = Util::GetTomlStyle<int>(U"Common.leftMargin");

		int indexTail = 0;
		for (double y = 0; y <= availableRegion.y; y += LineHeight)
		{
			const int index = indexTail + m_headIndex;
			if (index < m_mappingTexts.size())
			{
				std::visit(TextDraw{
					           .rightmost = availableRegion.x,
					           .leftCenter = Vec2{leftMargin, y + LineHeight / 2}
				           },
				           m_mappingTexts[index]);
			}
			indexTail++;
		}

		m_verticalSlider.UpdateVertical({
			.availableRect = WidgetSlideBar::AvailableAtLeftCenter(availableRegion),
			.currentIndex = m_headIndex,
			.minIndex = 0,
			.maxIndex = static_cast<int>(m_mappingTexts.size()),
			.pageSize = indexTail
		});
	}

private:
};

namespace Gui
{
	GuiMapping::GuiMapping() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void GuiMapping::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}
}
