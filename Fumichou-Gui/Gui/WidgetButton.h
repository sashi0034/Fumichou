#pragma once

namespace Gui
{
	class WidgetButton
	{
	public:
		WidgetButton();

		struct update_args
		{
			RectF availableRect;
			Emoji emojiIcon;
			StringView text;
			ColorF textColor = Palette::White;
		};

		bool Update(const update_args& args);

	protected:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};

	class WidgetButton2 : WidgetButton
	{
	public:
		WidgetButton2();

		struct update_args
		{
			RectF availableRect;
			Emoji emojiIcon;
			StringView text;
			ColorF textColor = Palette::White;
			StringView itemName;
		};

		// 左を押したら -1 | 右を押したら 1
		int Update(const update_args& args);

	private:
		std::shared_ptr<Impl> p_impl;
	};
}
