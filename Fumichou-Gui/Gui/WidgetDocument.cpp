#include "stdafx.h"
#include "WidgetDocument.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "WidgetSlideBar.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"WidgetDocument." + key);
	}
}

struct WidgetDocument0::Impl
{
	int m_headIndex{};
	WidgetSlideBar m_verticalSlider{};
	std::unique_ptr<IDocumentData> m_data{};

	void Update(const Size& availableRegion)
	{
		const double leftMargin = getToml<int>(U"leftMargin");

		int indexTail = 0;
		for (double y = 0; y <= availableRegion.y; y += LineHeight)
		{
			const int index = indexTail + m_headIndex;
			if (index < m_data->Size())
			{
				m_data->Draw(index, Document::Drawer{
					             .leftCenter = Vec2{leftMargin, y + LineHeight / 2},
					             .rightmost = availableRegion.x,
				             });
			}
			indexTail++;
		}

		m_verticalSlider.UpdateVertical({
			.availableRect = WidgetSlideBar::AvailableAtLeftCenter(availableRegion),
			.currentIndex = m_headIndex,
			.minIndex = 0,
			.maxIndex = static_cast<int>(m_data->Size()),
			.pageSize = indexTail
		});
	}
};

namespace Gui
{
	void Document::Drawer::operator()(const HeaderText& text) const
	{
		FontAsset(FontKeys::ZxProto_20_Bitmap)(text.text)
			.draw(24, Arg::leftCenter = leftCenter.movedBy(0, LineHeight / 2));
	}

	void Document::Drawer::operator()(const PlainText& text) const
	{
		FontAsset(FontKeys::ZxProto_20_Bitmap)(text.text).draw(Arg::leftCenter = leftCenter);
	}

	void Document::Drawer::operator()(SplitLine) const
	{
		(void)Line(leftCenter, leftCenter.withX(rightmost)).stretched(-4).draw(1, Palette::Gray);
	}

	void WidgetDocument0::Update(const Size& availableRegion)
	{
		p_impl->Update(availableRegion);
	}

	WidgetDocument0::WidgetDocument0(std::unique_ptr<IDocumentData>&& data):
		p_impl(std::make_shared<Impl>())
	{
		p_impl->m_data = std::move(data);
	}

	IDocumentData& WidgetDocument0::Data()
	{
		return *p_impl->m_data;
	}

	const IDocumentData& WidgetDocument0::Data() const
	{
		return *p_impl->m_data;
	}
}
