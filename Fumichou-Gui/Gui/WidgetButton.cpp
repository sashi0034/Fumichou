#include "stdafx.h"
#include "WidgetButton.h"

#include "FontKeys.h"
#include "GuiForward.h"
#include "Util/TomlStyleSheet.h"

using namespace Gui;

namespace
{
	template <typename T>
	inline T getToml(const String& key)
	{
		return Util::GetTomlStyle<T>(U"WidgetButton." + key);
	}
}

struct WidgetButton::Impl
{
	double m_pressedReaction{};
	RectF m_pressedRect{};

	bool Update(const update_args& args)
	{
		int textLeft{};
		if (not args.emojiIcon.codePoints.isEmpty())
		{
			textLeft = getToml<int>(U"textLeft");
			EnsureRegisterEmoji(args.emojiIcon);
			TextureAsset(args.emojiIcon.codePoints)
				.resized(Point::One() * (args.availableRect.size.y - 2))
				.draw(Arg::leftCenter = args.availableRect.leftCenter());
		}

		if (m_pressedReaction > 0)
		{
			// ボタンを押したときの演出
			m_pressedReaction = m_pressedReaction - Scene::DeltaTime();
			m_pressedRect.rounded(4).draw(ColorF(1.0, 0.1));
		}

		const auto textRect = FontAsset(FontKeys::ZxProto_20_Bitmap)(args.text)
			.draw(Arg::leftCenter = args.availableRect.leftCenter().movedBy(textLeft, 0), args.textColor);
		const auto lineRect = textRect.movedBy(-textLeft / 2, 0).stretched(textLeft / 2, 0);

		if (not IsMouseCaptured() && lineRect.intersects(Cursor::Pos()))
		{
			// マウスが選択領域内に入っているとき
			(void)lineRect.stretched(2).rounded(4).drawFrame(1, Palette::Darkgray);
			if (MouseL.down())
			{
				AcceptMouseCaptured();
				m_pressedReaction = 0.1;
				m_pressedRect = lineRect;
				return true;
			}
		}
		return false;
	}
};

namespace Gui
{
	WidgetButton::WidgetButton() :
		p_impl(std::make_shared<Impl>())
	{
	}

	bool WidgetButton::Update(const update_args& args)
	{
		return p_impl->Update(args);
	}
}
