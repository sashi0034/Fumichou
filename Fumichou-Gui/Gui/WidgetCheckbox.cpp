#include "stdafx.h"
#include "WidgetCheckbox.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"WidgetCheckBox." + key);
	}
}

struct WidgetCheckbox::Impl
{
	bool Update(const update_args& args)
	{
		TextureAsset(args.toggle ? U"✅" : U"⬜")
			.resized(Point::One() * (args.availableRect.size.y - 2))
			.draw(Arg::leftCenter = args.availableRect.leftCenter());
		const int textLeft = getToml<int>(U"textLeft");
		const auto textRect = FontAsset(FontKeys::ZxProto_20_Bitmap)(args.text)
			.draw(Arg::leftCenter = args.availableRect.leftCenter().movedBy(textLeft, 0), args.textColor);
		const auto lineRect = textRect.movedBy(-textLeft / 2, 0).stretched(textLeft / 2, 0).stretched(2);

		if (lineRect.intersects(Cursor::Pos()))
		{
			// マウスが選択領域内に入っているとき
			(void)lineRect.rounded(4).drawFrame(1, Palette::Darkgray);
			if (MouseL.down())
			{
				args.toggle.get() = not args.toggle;
				return true;
			}
		}
		return false;
	}
};

namespace Gui
{
	WidgetCheckbox::WidgetCheckbox() :
		p_impl(std::make_shared<Impl>())
	{
		EnsureRegisterEmoji(U"⬜"_emoji);
		EnsureRegisterEmoji(U"✅"_emoji);
	}

	bool WidgetCheckbox::Update(const update_args& args)
	{
		return p_impl->Update(args);
	}
}
