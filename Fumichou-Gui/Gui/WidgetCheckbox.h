#pragma once

namespace Gui
{
	class WidgetCheckbox
	{
	public:
		WidgetCheckbox();

		struct update_args
		{
			RectF availableRect;
			std::reference_wrapper<bool> toggle;
			StringView text;
			ColorF textColor = Palette::White;
		};

		bool Update(const update_args& args);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
