#include "stdafx.h"
#include "WidgetSlideBar.h"

#include <iso646.h>

#include "GuiForward.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"WidgetSlideBar." + key);
	}
}

struct WidgetSlideBar::Impl
{
	struct
	{
		bool flag;
		Vec2 start;
		double offset;
	} m_dragging{};

	void Update(const update_args& args)
	{
		const int minHeight = getToml<int>(U"minHeight");
		const double tabSize = minHeight; // TODO: 少ない要素では大きく
		const auto minAvailableId = args.minIndex;
		const auto maxAvailableId = args.maxIndex - args.pageSize;
		const auto topY = args.availableRect.topY() + tabSize / 2.0;
		const auto bottomY = args.availableRect.bottomY() - tabSize / 2.0;
		const auto currentY = Math::Lerp(
			bottomY, topY, static_cast<double>(args.currentIndex - minAvailableId) / (maxAvailableId - minAvailableId));

		// 描画
		const auto tabColor = getToml<ColorF>(U"tabColor");
		const auto tabRect = RectF(Arg::center = Vec2{args.availableRect.centerX(), currentY},
		                           args.availableRect.size.withY(minHeight));
		(void)tabRect
		      .draw(tabColor)
		      .drawFrame(m_dragging.flag ? 1.0 : 0.0, Palette::Wheat);

		// ドラッグ処理
		if (not m_dragging.flag && MouseL.down() && args.availableRect.intersects(Cursor::PosF()))
		{
			// ドラッグ開始
			m_dragging.flag = true;
			m_dragging.start = Cursor::PosF();
			m_dragging.offset = tabRect.intersects(m_dragging.start) ? currentY - m_dragging.start.y : 0;
		}

		if (m_dragging.flag)
		{
			// ドラッグ中
			if (MouseL.pressed())
			{
				AcceptMouseCaptured();
				const auto newY = Cursor::PosF().y + m_dragging.offset;
				const auto rate = (newY - bottomY) / (topY - bottomY);
				args.currentIndex.get() = minAvailableId + (maxAvailableId - minAvailableId) * rate;
			}
			else
			{
				m_dragging.flag = false;
			}
		}

		// 丸め処理
		args.currentIndex.get() = std::min(std::max(args.currentIndex.get(), minAvailableId), maxAvailableId);
	}
};

namespace Gui
{
	WidgetSlideBar::WidgetSlideBar() :
		p_impl(std::make_shared<Impl>())
	{
	}

	int WidgetSlideBar::DefaultWidth()
	{
		return getToml<int>(U"defaultWidth");
	}

	RectF WidgetSlideBar::AvailableAtRightCenter(const SizeF& parentRegion)
	{
		return RectF{
			Arg::rightCenter = parentRegion.withY(parentRegion.y / 2),
			SizeF{DefaultWidth(), parentRegion.y}
		};
	}

	void WidgetSlideBar::UpdateVertical(const update_args& args)
	{
		p_impl->Update(args);
	}

	void WidgetSlideBar::UpdateHorizontal(update_args&& args)
	{
		args.availableRect.rotate90At(args.availableRect.center());
		const Transformer2D t{Mat3x2::Rotate(90_deg, args.availableRect.center()), TransformCursor::Yes};
		p_impl->Update(args);
	}
}
