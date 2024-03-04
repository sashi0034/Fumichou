﻿#include "stdafx.h"
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
		for (int y = 0; y < availableRegion.y - LineHeight; y += LineHeight)
		{
			const int index = indexTail + m_headIndex;

			if (y == 0 && m_data->IsEmpty(index))
			{
				// ページ上部分の切れ端
				int y0{};
				for (int i0 = index - 1; i0 >= 0; i0--)
				{
					y0 -= LineHeight;
					if (m_data->IsEmpty(i0)) continue;
					m_data->Draw(i0, Document::Drawer{
						             .leftCenter = Vec2{leftMargin, y0 + LineHeight / 2},
						             .rightmost = availableRegion.x,
					             });
					break;
				}
			}

			if (index < m_data->Size())
			{
				// インデックス箇所を描画
				m_data->Draw(index, Document::Drawer{
					             .leftCenter = Vec2{leftMargin, y + LineHeight / 2},
					             .rightmost = availableRegion.x,
				             });
			}
			indexTail++;
		}

		// インデックス移動
		if (RectF(availableRegion).intersects(Cursor::PosF()))
		{
			const int step = indexTail / 16;
			const auto wheel = Mouse::Wheel();
			if (wheel < 0) m_headIndex -= step;
			else if (wheel > 0) m_headIndex += step;
		}

		// 垂直バー
		m_verticalSlider.UpdateVerticalInverted({
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
	RectF Document::Drawer::LineRect() const
	{
		return RectF{Arg::leftCenter = leftCenter, SizeF(rightmost - leftCenter.x, LineHeight)};
	}

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
