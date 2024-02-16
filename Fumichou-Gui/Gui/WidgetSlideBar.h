#pragma once

namespace Gui
{
	class WidgetSlideBar
	{
	public:
		WidgetSlideBar();

		static int DefaultWidth();
		static RectF AvailableAtRightCenter(const SizeF& parentRegion);
		static RectF AvailableAtLeftCenter(const SizeF& parentRegion);

		struct update_args
		{
			RectF availableRect;
			std::reference_wrapper<int> currentIndex;
			int minIndex;
			int maxIndex;
			int pageSize;
		};

		void UpdateVertical(update_args args);
		void UpdateVerticalInverted(update_args args);
		void UpdateHorizontal(update_args args);

		RectF GetTab() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
