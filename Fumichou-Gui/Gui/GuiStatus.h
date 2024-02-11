#pragma once

namespace Gui
{
	class GuiStatus
	{
	public:
		GuiStatus();
		void Update(const Size& availableRegion);

	private:
		struct Impl;
		std::shared_ptr<Impl> p_impl;
	};
}
