#pragma once

namespace Gui
{
	class UiScreenOverlay
	{
	public:
		UiScreenOverlay();

		struct update_args
		{
			RectF screenRect;
			std::function<void()> clickedModeChange;
		};

		void Update(const update_args& args);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
