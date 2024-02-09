#pragma once

namespace Gui
{
	class WidgetSlideBar
	{
	public:
		void Update(const Rect& available, int& current, int min, int max);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
