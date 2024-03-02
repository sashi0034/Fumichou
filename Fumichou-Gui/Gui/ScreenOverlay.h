#pragma once

namespace Gui
{
	class ScreenOverlay
	{
	public:
		ScreenOverlay();

		struct update_args
		{
			RectF screenRect;
			std::reference_wrapper<int> modeIndex;
		};

		void Update(const update_args& args);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
