#include "stdafx.h"
#include "WidgetTabBar.h"

#include <iso646.h>

#include "FontKeys.h"
#include "GuiForward.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"WidgetTabBar." + key);
	}
}

struct WidgetTabBar::Impl
{
	void Update(const update_args& args)
	{
		const auto tabW = args.availableRect.w / args.tabNames.size();
		double offsetX{};
		const auto plainColor = getToml<ColorF>(U"plainColor");
		const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
		for (int i = 0; i < args.tabNames.size(); ++i)
		{
			const auto rect =
				RectF(args.availableRect.pos.movedBy(offsetX, 0), SizeF{tabW, args.availableRect.size.y});

			const bool hovered = rect.intersects(Cursor::Pos());

			// 描画
			if (i != args.currentIndex)
			{
				(void)rect.rounded(4).draw(plainColor * (hovered ? 0.9 : 1.0));
			}
			(void)font(args.tabNames[i]).drawAt(rect.center());

			// タブ切り替え
			if (not IsClickCaptured() && hovered && MouseL.down())
			{
				AcceptClickCaptured();
				args.currentIndex.get() = i;
			}

			offsetX += tabW;
		}
	}
};

namespace Gui
{
	WidgetTabBar::WidgetTabBar() :
		p_impl(std::make_shared<Impl>())
	{
	}

	void WidgetTabBar::Update(const update_args& args)
	{
		p_impl->Update(args);
	}
}
