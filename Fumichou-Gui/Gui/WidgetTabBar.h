#pragma once

namespace Gui
{
	class WidgetTabBar
	{
	public:
		WidgetTabBar();

		struct update_args
		{
			RectF availableRect;
			std::reference_wrapper<int> currentIndex;
			std::span<const String> tabNames;
		};

		void Update(const update_args& args);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
