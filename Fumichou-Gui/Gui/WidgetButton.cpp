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
	RectF m_hoverHalf{};

	template <typename Args, bool button2>
	int Update(const Args& args)
	// int Update(const update_args& args)
	{
		if constexpr (button2)
		{
			// 右半分か左半分のホバー領域描画
			if (m_hoverHalf.w != 0) m_hoverHalf.draw(getToml<ColorF>(U"hoverBack"));
		}

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

		const auto font = FontAsset(FontKeys::ZxProto_20_Bitmap);
		const auto textRect = font(args.text)
			.draw(Arg::leftCenter = args.availableRect.leftCenter().movedBy(textLeft, 0), args.textColor);
		const auto lineRect = textRect.movedBy(-textLeft / 2, 0).stretched(textLeft / 2, 0);

		if constexpr (button2)
		{
			font(args.itemName).draw(Arg::leftCenter = lineRect.rightCenter().moveBy(textLeft / 2, 0), Palette::White);
		}

		const bool isCursorIntersects = not IsClickCaptured() && lineRect.intersects(Cursor::Pos());
		if constexpr (button2)
		{
			// 左半分か右半分のホバーしてる領域を覚える
			if (isCursorIntersects)
				m_hoverHalf = RectF(
					lineRect.pos.movedBy(Cursor::Pos().x < lineRect.centerX() ? 0 : lineRect.w / 2, 0),
					lineRect.size.withX(lineRect.w / 2));
			else m_hoverHalf = {};
		}
		if (isCursorIntersects)
		{
			// マウスが選択領域内に入っているとき
			(void)lineRect.stretched(2).rounded(4).drawFrame(1, Palette::Darkgray);

			if (MouseL.down())
			{
				AcceptClickCaptured();
				m_pressedReaction = 0.1;
				if constexpr (button2) m_pressedRect = m_hoverHalf;
				else m_pressedRect = lineRect;
				return Cursor::Pos().x < lineRect.centerX() ? -1 : 1;
			}
		}
		return 0;
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
		return p_impl->Update<update_args, false>(args) != 0;
	}

	WidgetButton2::WidgetButton2() :
		p_impl(std::make_shared<Impl>())
	{
	}

	int WidgetButton2::Update(const update_args& args)
	{
		return p_impl->Update<update_args, true>(args);
	}
}
