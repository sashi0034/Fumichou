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

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
