#include "stdafx.h"
#include "GuiMapping.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "HwFrame.h"
#include "WidgetDocument.h"
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

	struct MappingDraw : Document::Drawer
	{
		using Drawer::operator();

		void operator()(const MappingDesc& desc) const
		{
			FontAsset(FontKeys::ZxProto_20_Bitmap)(desc.addrRange)
				.draw(Arg::leftCenter = leftCenter, Palette::Darkgray);
			FontAsset(FontKeys::ZxProto_20_Bitmap)(desc.desc)
				.draw(Arg::leftCenter = leftCenter.withX(getToml<int>(U"descLeft")), desc.descColor);
		}
	};

	using MappingDocumentData = DocumentData<MappingDraw, MappingDesc>;

	template <typename T>
	void makeSingleDescs(
		MappingDocumentData::array_type& texts,
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

	void generateTexts(MappingDocumentData::array_type& texts)
	{
		auto&& mmu = Nes::HwFrame::Instance().GetHw().GetMmu();
		texts.push_back(Document::HeaderText(U"CPU Read-mapping"));
		texts.push_back(std::monostate{});
		makeSingleDescs(texts, mmu.GetCpuRead(), ColorBlue);
		texts.push_back(Document::SplitLine());
		texts.push_back(Document::HeaderText(U"CPU Write-mapping"));
		texts.push_back(std::monostate{});
		makeSingleDescs(texts, mmu.GetCpuWrite(), ColorOrange);
		texts.push_back(Document::SplitLine());
		texts.push_back(Document::HeaderText(U"PPU Read-mapping"));
		texts.push_back(std::monostate{});
		makeSingleDescs(texts, mmu.GetPpuRead(), ColorBlue);
		texts.push_back(Document::SplitLine());
		texts.push_back(Document::HeaderText(U"PPU Write-mapping"));
		texts.push_back(std::monostate{});
		makeSingleDescs(texts, mmu.GetPpuWrite(), ColorOrange);
	}
}

struct GuiMapping::Impl
{
	int m_headIndex{};
	WidgetDocument<MappingDocumentData> m_document{};

	void Update(const Size& availableRegion)
	{
		const ScopedViewport2D viewport2D{availableRegion};
		if (not m_document.Data().Size()) generateTexts(m_document.Data().Raw());
		m_document.Update(availableRegion);
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
