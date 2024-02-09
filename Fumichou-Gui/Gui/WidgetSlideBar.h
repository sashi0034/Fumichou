#pragma once

namespace Gui
{
	class WidgetSlideBar
	{
	public:
		WidgetSlideBar();

		static RectF AvailableAtRightCenter(const SizeF& parentRegion);

		struct update_args
		{
			RectF availableRect;
			std::reference_wrapper<int> currentIndex;
			int minIndex;
			int maxIndex;
			int pageSize;
		};

		void Update(const update_args& args);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
